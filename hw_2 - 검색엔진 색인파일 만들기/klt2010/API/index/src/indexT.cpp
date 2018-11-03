/*--------------------------------------------------------------------------
	File-name	: index-text.c
	Description	: Example of calling Korean automatic indexing module.
			  Hangul I/O code is KSC-5601-1987 or KSSM Hangul code.
	Written-by	: Kang, Seung-Shik at Kookmin University  2001. 6.
--------------------------------------------------------------------------*/
#include <ctype.h>
#include <stdlib.h>

#include "../header/ham-ndx-CPP2.h"

/*========================== Global Variable ===========================*/
HAM_TERMLIST Term[MAX_TERMS_DOC];	/* extracted terms */
HAM_TERMMEMORY TM;	/* termtext.h: memories needed for term extraction */
/*======================================================================*/

/* �Է� ���� ��ü�� memory�� load */
char *load_text(
FILE *fp
) {
	long n;
	char *p;

	fseek(fp, 0L, 2);
	n = ftell(fp);	/* n: byte size of file 'fp' */

	fseek(fp, 0L, 0);
	p = (char *) malloc(n+1);	/* memory allocation */
	if (p == NULL) return NULL;

	fread(p, sizeof(char), n, fp);	/* read 'fp' to 'p' */
	*(p+n) = '\0';

	return p;
}

/* ����� ��� ��� */
void put_terms(
FILE *fp,
HAM_TERMLIST term[],/* ����� ������ ���� ���̺� */
int n,				/* ����� ��� ���� */
unsigned char ts[]	/* ������ NULL ���� '\0'���� ���еǾ� ����Ǿ� �ִ� memory pool */
		/* ts[] --> "term1<null>term2<null>term3<null>...termN<null>" */
		/* term[i].offset --> ��� i�� ts[] ���� ��ġ�� ����Ű�� ���� */
) {
	int i, j, nlocs;

	fprintf(fp, "----------------------------------------------------------------------\n");
	fprintf(fp, " No: Freq  Score  Term\t\t Loc1\t Loc2\t Loc3\t Loc4\t Loc5\n");
	fprintf(fp, "----------------------------------------------------------------------\n");

	for (i = 0; i < n; i++) {
		fprintf(fp, "%3d:%5u %6u  %s", i+1,

			term[i].tf,			/* ����� �󵵼� */

			term[i].score,		/* ����� ����ġ */

			ts+term[i].offset);	/* ts+term[i].offset : ��� ��Ʈ�� */

			if (strlen((char*)ts+term[i].offset) < 6) fprintf(fp, "\t");	/* �� ���� */

		nlocs = (term[i].tf > MAX_LOCS_PER_TERM ? MAX_LOCS_PER_TERM : term[i].tf);
		for (j = 0; j < nlocs; j++)	/* ����� ��ġ���� �ִ� MAX_LOCS_PER_TERM �� */
			fprintf(fp, "\t%5u", term[i].loc[j]);
		fprintf(fp, "\n");
	}

	fprintf(fp, "----------------------------------------------------------------------\n");
}

void usage_index()	/* HAM options in command-line argument */
{
	WELCOME(VERSION_NUMBER);
	puts("usage: index [-options] [input.txt] [output.txt]\n");
	puts("    no options & I/O files --> default options applied");
	puts("\t-x: analysis results: default options");
	puts("\t-k: exclude SOME SINGLE NOUNS IN DIC.");
	puts("\t-K: exclude MOST SINGLE NOUNS IN DIC.");
	puts("\t-j: normalize Arabic/Hangul numerals\n");
	puts("\t-A: automatic spacing for input sentence");

	puts("\t-1: include 1-syllable nouns");
	puts("\t-9: ignore more than 9-syllable nouns");
	puts("\t-a: include all nouns with alphanumerics");
	puts("\t-V: include VERBs, ADVs, DETs, at. al.");
	puts("\t-u: include secondary candidate for unknown");
	puts("\t-v: include unknown guessed for analed word");
	puts("\t-c: exclude c-noun components");
	puts("\t-C: include 2 adjacent c-noun components");
	puts("\t-d: append c-noun components for max 2 candidates ");
	puts("\t-p: don't apply stopword list\n");
	puts("\t-i: extract input string itself as index-term");
	puts("\t-I: extract input string itself & not duplicated\n");
	puts("\t-2: 2nd type of output format");
	puts("\t-s: don't echo-back input sentence itself");
	puts("\t-w: don't echo-back input word itself");
	puts("\t-l: input mode: line-by-line --> sentence mark\n");
	puts("    Options may be combined like -xa, -x19a or -xsw0");
	puts("    WITH OPTION & NO I/O FILES SPECIFIED --> INTERACTIVE TESTING");
	GOODBYE(YEAR);	/* goodbye message: 'header/ham-ndx.h' */
}

/*
	Set I/O types: following 3 types are possible.
		1. stdin & stdout
		2. file input & stdout
		3. file input & file output
*/
int set_iofile_ptr(
int argc, char *argv[],
FILE **fpin, FILE **fpout)
{
	int i=1, nargs=argc;

	if (argc > 1 && argv[1][0] == '-') { i++; nargs--; }

	switch (nargs) {
	case 1:
		if (argc == 1) { usage_index(); return 1; }
		break;
	case 2:
		*fpin = fopen(argv[i], "rb");
		if (!*fpin) { printf("No such file <%s>\n", argv[i]); return 1; }
		break;
	case 3:
		*fpin = fopen(argv[i], "rb");
		if (!*fpin) { printf("No such file <%s>\n", argv[i]); return 1; }
		if (*fpout = fopen(argv[i+1], "r")) {
			fprintf(stderr, "Overwrite output file <%s> ? ", argv[i+1]);
			if (tolower(fgetc(stdin)) != 'y') return 1;
		}
		*fpout = fopen(argv[i+1], "w");
		break;
	default:
		usage_index(); return 1;
	}

	return 0;
}

void error_message(	/* cannot load dictionary files */
HAM_RUNMODE mode
) {
	fprintf(stderr, "ERROR: cannot load dictionary files in <%s>. Error code %d in <header/err-code.h>\n",
		mode.dicpath, mode.err_code);
	fprintf(stderr, "Directory path for dictionaries should be correctly positioned. Or\n");
	fprintf(stderr, "you should modify <src/text.c> and <%s>.\n", INI_FILENAME);
	fprintf(stderr, "1. Check or modify the value <DicDirPath> in <%s>.\n", INI_FILENAME);
	fprintf(stderr, "2. Check or modify <src/text.c> --> function open_HAM() --> argument <%s>\n", INI_FILENAME);
}

/*
	Example of calling keyword extraction routine.

	Arguments of 'get_stems()'
		char *sent;   --> input sentence(KSC 5601 Hangul code)
		char *keys[]; --> pointer array of keywords.

	Two more arguments are needed in 'get_stems_TS()'
		char *KeyMem; --> keywords saving area.
		HAM_MORES HamOut; --> morph. analysis result

	Return value of 'get_stems' and 'get_stems_TS()'
		is the number of keywords in 'keys[]'.

	You may use 'get_stems()' or 'get_stems_TS()' as you want.
	Only the difference is the number of arguments & Thread-Safe.
*/

/*---------- INTEGRATION GUIDE of HAM to app. system -----------*/
/*                                                              */
/* 0. You should include 'header/ham-ndx.h'.                    */
/*                                                              */
/* 1. Following variables are required for HAM                  */
/*       char text[MAX];  -->  KSC5601 input Korean text        */
/*       char *terms[MAX_TERMS]; -->  ptr. array of terms       */
/*       HAM_RUNMODE mode  -->  running mode of HAM             */
/*                                                              */
/*       char KeyMem[MAXKEYBYTES]; -->  keyword saving area     */
/*       HAM_MORES HamOut;  -->  morph. analysis result         */
/*                                                              */
/* 2. Call 'open_HAM_index(&mode, option_string, filename)'     */
/*       before 1st call get_stems() or get_stems_TS().         */
/*       'userdic' is a user-dic name: default "ham-usr.dic".   */
/*       It's argument is a directory path of 'hangul.???'.     */
/*       It initializes global var.s and opens dic. files.      */
/*       Return value --- 0(normal), 1(dic.file open failure)   */
/*                                                              */
/* 3. Call 'get_terms(text, terms, mode)'.                      */
/*       Extracted keywords(nouns) from the sentence are        */
/*       confirmed by 'put_terms(fpout, sent, keys, nkeys)'.    */
/*                                                              */
/* 4. Call 'close_HAM()', if get_terms?() is no nonger used.    */
/*                                                              */
/* Only 3 functions are required for integration to appl. system*/
/*        open_HAM_index(), get_terms(), close_HAM()            */
/*                                                              */
/*--------------------------------------------------------------*/
main(int argc, char *argv[])
{
	FILE *fpin=stdin, *fpout=stdout;

	char *optstr=NULL;	/* option string: e.g. "1apVC", "pVc" */
	HAM_RUNMODE mode;	/* HAM running mode: 'header/runmode.h' */

	char *text;	/* input text: �Է� ���� ��ü */
	int n, n2=0;	/* ����� ����: ����� ���ڷ� ����� �ִ� ���� ���� */
	
	if (argc > 1 && argv[1][0] == '-')
		n = atoi(argv[1]+1);	/* ����� term�� �ִ� ���� ���� */
	/*if (argc > 1 && argv[1][0] == '-') optstr = argv[1]+1;*/

	if (set_iofile_ptr(argc, argv, &fpin, &fpout))
		return 0;   /* I/O files open failed */

	/*=========================== Initialization ===========================*/
	if (open_HAM_index(&mode, optstr, "./hdic/KLT2000.ini")) {
		error_message(mode); return 0;
	} else WELCOME(VERSION_NUMBER);	/* welcome message: 'header/ham-ndx.h' */
	/*======================================================================*/

	/*if (set_options_from_file("/home/kang/KLT2000.ini", &mode))
		fprintf(stderr, "WARNING: file not found --- <KLT2000.ini>\n");
	*/	/* �ɼ����� "KLT2000.ini"�� ����Ǿ������� Ȯ���ϴ� ��� */

	/*=========================== Extract Terms ============================*/
	if ((text=load_text(fpin)) == NULL)	/* [����] 'fpin'�� "rb"�� open */
		return -1;	/* text file --> 'text' */
	n = get_terms_text((unsigned char *)text, Term, &TM, &mode, n2, 3, 1);
	/*------------------------------------------------------------------------
		- �ټ���° ���� -- 0/n/-n(�ִ�� ����Ǵ� ����)
			n: �ִ� ���� ����, -n: �� n�̻�(or n �ۼ�Ʈ), 0: ��� ��� ����
			[����] n�� ���� ����: sortflag(���� ���)�� ���� '����ġ' or '���� �ɼ�'
			  - sortflag�� ��� 1/2/3/4 -- ����ġ ������ n�� ����
			  - sortflag�� ���� -1/-2/-3/-4 -- ���� ��Ŀ� ���� ���� �� n�� ����
			[����] n <= -10 �̸�, -n �ۼ�Ʈ��ŭ ������ �����.

		- ������° ���� -- 0/1/2/3/4(���� ���)
			0: ���� ����, 1: '������'��, 2: �󵵼�, 3: ����ġ, 4: ������ġ
			[����] n�� ���� ������ '���� ���'�� ���� �����Ϸ��� ������ ��

		- ������ ���� -- 0/1(������ġ numbering ���)
			0: ����������, 1: ���庰�� 100���� ����
	========================================================================*/

	put_terms(fpout, Term, n, TM.memTermString);

	/*========================= Finalize Indexer ===========================*/
	close_HAM_index(&mode);	/* HAM is not used any more */
	/*======================================================================*/

	free(text);	/* malloc() in 'load_text()' */
	if (fpin != stdin) fclose(fpin); if (fpout != stdout) fclose(fpout);
	GOODBYE(YEAR);	/* good-bye message: 'header/ham-ndx.h' */
	return 0;
}
/*---------------------------- end of index-text.c --------------------------*/
