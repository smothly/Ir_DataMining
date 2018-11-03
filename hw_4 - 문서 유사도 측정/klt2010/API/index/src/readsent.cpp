/*
	File name: readsent.c
	Description: Functions for input, output and initialization
	Created by: Kang, Seung-Shik		04/13/1993
*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>	/* declared only for 'randomize_file_ptr()' */
#include <time.h>	/* declared only for 'randomize_file_ptr()' */

#include "../header/sizedefs.h"

/*
	Similar to 'fgets()', but it works well for Unicode.
	Return value: number of characters
*/
int fgets_unicode(
	unsigned char *line,
	int n,	/* MAX characters in 'line' */
	FILE *fp
) {
	int i, ch;

	for (i=0; i < n-1; ) {
		ch = fgetc(fp);
		if (ch == EOF) break;
		if (ch == '\n') {
			if (!i) continue;
			else break;
		}

		line[i++] = ch;
		if (ch & 0x80) line[i++] = fgetc(fp);
	}

	line[i] = '\0';
	return i;	/* number of characters in 'line' */
}

/*
	Similar to 'fputs()', but it works well for Unicode.
*/
void fputs_unicode(
	unsigned char *line,
	FILE *fp
) {
	int i;

	for (i=0; line[i]; i++) {
		fputc(line[i], fp);
		if (line[i] & 0x80) fputc(line[++i], fp);
	}
}

/*
	check sentence ending marks
*/
int sent_ending_mark(
	unsigned char c
) {
	return (c == '.' || c == '?' || c == '!');
}

/*
	KSC 5601 sentence ending marks.
*/
int sent_ending_mark_ksc(
	unsigned char c,
	unsigned char d
) {
	return ((c == 0xA3 && d == 0xAE) ||	/* '.' */
		(c == 0xA3 && d == 0xBF) ||	/* '?' */
		(c == 0xA3 && d == 0xA1) ||	/* '!' */
		(c == 0xA1 && d == 0xA3) ||	/* circle '.' */
		(c == 0xA1 && d == 0xA6) ||	/* '...' */
		(c == 0xA1 && d == 0xBB));	/* right bracket */
}

/*
	Sentence ending mark './?/!' --- Ascii char.
*/
int sent_ending_1(
	unsigned char c1,
	unsigned char c2
) {
	return ((sent_ending_mark(c1) || c1 == '\n') &&
		(c2 == '\0' || isspace(c2)));
}

/*
	Sentence ending mark './?/!' --- KSC 5601 char.
*/
int sent_ending_2(
	unsigned char c1,
	unsigned char c2,
	unsigned char c3
) {
	return ((c3 == '\0' || isspace(c3)) &&
		sent_ending_mark_ksc(c1, c2));
}

/*
	Sentence ending mark './?/!' --- KSC 5601 char.
*/
int sent_ending_3(
	unsigned char c1,
	unsigned char c2,
	unsigned char c3,
	unsigned char c4
) {
	return (c3 == 0xA1 && c4 == 0xA1 &&	/* KSC 5601 blank */
		sent_ending_mark_ksc(c1, c2));
}

/*
	Get one sentence.

	If a sentence mark or (new-line and word-space) char. appears,
	then stop reading a sentence from input file.

	Rule 1. Maximum input words are MAXWORDS-1.
	Rule 2. Total bytes should be less than SENTSIZE.
	Rule 3. No loss of original characters.
	Rule 4. Maximum 10 lines are input at a time.

	Rule 3 is needed only for special purpose of using byte offset.
*/
int read_sentence(
	FILE *fp,
	unsigned char *p
) {
	int i, c;
	int hflag = 0;
	int nlines = 0;
	int nwords = 0;	/* rough # of words */
	unsigned char *q = p;	/* start address of 'p' */

	if (feof(fp)) return EOF;

	c = fgetc(fp) & 0xff;
	while (isspace(c)) c = fgetc(fp) & 0xff;

	*q++ = c;
	for (i = 1; nwords < MAXWORDS-1; i++) {
		if (*(q-1) & 0x80)	/* double-byte code: Hangul/Hanja */
			hflag = hflag ? 0 : 1;
		else hflag = 0;
		if (i >= SENTSIZE-3) {
			if (hflag)	/* needed low-byte Hangul */
				*q++ = fgetc(fp);
			break;
		}
		c = fgetc(fp);
		if (c == EOF || sent_ending_1(*(q-1), c & 0xff))
			break;
		else if (q-p > 1 && sent_ending_2(*(q-2), *(q-1), c & 0xff))
			break;
		else if (q-p > 2 && sent_ending_3(*(q-3), *(q-2), *(q-1), c & 0xff))
			break;
		else *q++ = c & 0xff;

		if (c == '\n' && nlines++ > 10)
			break;	/* maximum 10 lines are allowed */
		if ((isspace(c) || ispunct(c)) && !isspace(*(q-2)))
			nwords++;
	}
	*q = '\0';

	return (nwords ? nwords : c);
}

/*
	Input sentence.
	Return value: 1 -- O.K., 0 -- FAIL
*/
int get_sent_or_line(
	FILE *fp,
	unsigned char *sent,
	unsigned char inputmode
) {
	if (inputmode) {	/* line-by-line input */
		if (fgets_unicode(sent, SENTSIZE-1, fp))
			return 1;
		else return 0;
	} else return (read_sentence(fp, sent) != EOF);
}

/*
	Invalid characters like
		1. non-printable Ascii characters : control/graphic char.s
		2. incomplete Hangul syllables : Ja-Eum, Mo-Eum
		3. KSC 5601 2-byte symbols
	are replaced with blank.

	If you want to remove all Ascii symbols,
	change '!isprint(*s)' to '!isalnum(*s)'.
	In this case, be careful for 'Vitamin-A', 'a.m.'.
*/
void removeSpecialChar(
	unsigned char *s
) {
	unsigned char c, d;

	while (*s) {
		c = *s; d = *(s+1);
		if (c & 0x80) {
			if (c < 0xb0 || !(d & 0x80)) {
				/* non-Hangul, non-Hanja --> blank */
				*s = ' '; *(s+1) = ' ';
			}
			s += 2;
		} else {	/* non-printable char --> blank */
			if (!isprint(*s)) *s = ' ';
			s++;
		}
	}
}

/* select random sentence from input file : only for testing */
void randomize_file_ptr(
	FILE *fp
) {
	double i;
	int ch;
	static long offset, max;
	time_t tm;

	fseek(fp, 0, 2);
	max = ftell(fp);
	max = (max < 0) ? -max : max;
loop:
	time(&tm);
	srand(tm % 1000);
	i = (double) rand() + offset;

	offset = ((long) i < 0) ? (long) -i: (long) i;
	offset = offset % max;

	if (offset < 0) goto loop;

	fseek(fp, (long) offset, 0);

	if (offset < 30) fseek(fp, 0, 0);
	else {
		while ((ch = fgetc(fp)) != '\n')
			if (ch == EOF) goto loop;
	}
}
/*------------------- end of readsent.c -------------------*/
