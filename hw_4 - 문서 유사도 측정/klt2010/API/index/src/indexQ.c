/*--------------------------------------------------------------------------
	File-name	: query.c
	Description	: query sentence analysis
	Written-by	: Kang, Seung-Shik at Kookmin University  2002. 6.
--------------------------------------------------------------------------*/
#include <stdlib.h>

#include "../header/ham-ndx.h"
#include "../header/ham-api.h"

#ifdef WINDOWS
extern _declspec(dllimport) int load_term_dics(unsigned char *, HAM_TERMMEMORY *);
#else
extern int load_term_dics();
#endif

/*========================== Global Variable ===========================*/
HAM_TERMLIST Term[MAX_TERMS_SENT];	/* extracted terms */
HAM_TERMMEMORY TM;	/* termtext.h: memories needed for term extraction */
/*======================================================================*/

void put_terms(fp, term, n, ts, cnnflag)
FILE *fp;
HAM_TERMLIST term[];/* 추출된 용어들의 정보 테이블 */
int n;				/* 추출된 용어 개수 */
unsigned char ts[];	/* 용어 문자열들이 delimeter '\0'으로 구분된 문자열 */
		/* ts[] --> "term1<null>term2<null>term3<null>...termN<null>" */
		/* term[i].offset --> 용어 i의 ts[] 저장 위치를 가리키는 정수 */
int cnnflag;	/* 복합명사 분해결과 출력 -- 0: 다른용어와 동일, 1: 번호 생략 */
{
	int i, j, nlocs, count=1;

	fprintf(fp, "------------------------------------------------------\n");
	fprintf(fp, " No: Freq  Score  Term\t\t Loc1\t Loc2\t Loc3\n");
	fprintf(fp, "------------------------------------------------------\n");

	for (i = 0; i < n; i++) {
		if (term[i].pos == 'P')		/* 복합명사 분해 결과 검사 */
			fprintf(fp, "    ");	/* numbering 생략 */
		else fprintf(fp, "%3d:",  count++);

		fprintf(fp, "%5u %6.2f  %s",
			term[i].tf,				/* 용어의 빈도수 */
			term[i].score / 100.,	/* 용어의 가중치 */
			ts+term[i].offset);		/* ts+term[i].offset : 용어 스트링 */
			if (strlen(ts+term[i].offset) < 6) fprintf(fp, "\t");	/* 열 맞춤 */

		nlocs = (term[i].tf > MAX_LOCS_PER_TERM ? MAX_LOCS_PER_TERM : term[i].tf);
		for (j = 0; j < nlocs; j++)	/* 용어의 위치정보 최대 MAX_LOCS_PER_TERM 개 */
			fprintf(fp, "\t%5u", term[i].loc[j]);
		fprintf(fp, "\n");
	}

	fprintf(fp, "------------------------------------------------------\n");
}

/* 용어 추출결과를 한 line으로 출력 */
void put_terms_line(fp, term, n, ts)
FILE *fp;
HAM_TERMLIST term[];/* 추출된 용어들의 정보 테이블 */
int n;				/* 추출된 용어 개수 */
unsigned char ts[];	/* 용어 문자열들이 delimeter '\0'으로 구분된 문자열 */
		/* ts[] --> "term1<null>term2<null>term3<null>...termN<null>" */
		/* term[i].offset --> 용어 i의 ts[] 저장 위치를 가리키는 정수 */
{
	int i, cnnflag = 0;	/* 복합명사 분해결과 출력 중? */

	fprintf(fp, "==>");
	for (i = 0; i < n; i++) {
		if (term[i].pos == 'P') {
			if (!cnnflag) fprintf(fp, " (");	/* 복합명사 분해된 것 첫번째 */
			cnnflag = 1;
		} else if (cnnflag) {
			fprintf(fp, " )");	/* 복합명사 분해된 것 마지막 */
			cnnflag = 0;
		}

		fprintf(fp, " %s_%.2f", ts+term[i].offset, (double) term[i].score / 100.);
	}

	if (cnnflag) fprintf(fp, " )");
	fprintf(fp, "\n\n");
}

void usage_index()	/* HAM options in command-line argument */
{
	WELCOME;
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
	GOODBYE;	/* goodbye message: 'header/ham-ndx.h' */
}

/*
	Set I/O types: following 3 types are possible.
		1. stdin & stdout
		2. file input & stdout
		3. file input & file output
*/
int set_iofile_ptr(argc, argv, fpin, fpout)
int argc; char *argv[];
FILE **fpin, **fpout;
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
		*fpin = fopen(argv[i], "r");
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

void error_message(mode)	/* cannot load dictionary files */
HAM_RUNMODE mode;
{
	fprintf(stderr, "ERROR: cannot load dictionary files in <%s>. Error code %d in <header/err-code.h>\n",
		mode.dicpath, mode.err_code);
	fprintf(stderr, "Directory path for dictionaries should be correctly positioned. Or\n");
	fprintf(stderr, "you should modify <src/index.c> and <%s>.\n", INI_FILENAME);
	fprintf(stderr, "1. Check or modify the value <DicDirPath> in <%s>.\n", INI_FILENAME);
	fprintf(stderr, "2. Check or modify <src/index.c> --> function open_HAM() --> argument <%s>\n", INI_FILENAME);
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

	char *optstr="1ap";	/* option string: e.g. "1apVC", "pVc" */
	HAM_RUNMODE mode;	/* HAM running mode: 'header/runmode.h' */

	unsigned char sentence[999];	/* input word or sentence */
	int n;	/* number of extracted keywords */

	if (set_iofile_ptr(argc, argv, &fpin, &fpout))
		return 0;   /* I/O files open failed */
	if (argc > 1 && argv[1][0] == '-') optstr = argv[1]+1;

	/*=========================== Initialization ===========================*/
	if (open_HAM_index(&mode, optstr, "./hdic/KLT2000.ini")) {
		error_message(mode); return 0;
	} else WELCOME;	/* welcome message: 'header/ham-ndx.h' */
	/*======================================================================*/

	/* 옵션파일 "KLT2000.ini"가 적용되는지를 확인하는 방법 */
	/*if (set_options_from_file("/home/kang/KLT2000.ini", &mode))
		fprintf(stderr, "WARNING: file not found --- <KLT2000.ini>\n");
	*/

	if (load_term_dics(mode.dicpath, &TM)) { puts("Error: loading term dic"); return 0; }
	/*=========================== Extract Terms ============================*/
	while (fgets(sentence, 999, fpin)) {
/*		printf("질의어 문장 : ");*/
		fputs(sentence, fpout);
		if (!strcmp(sentence, "quit\n") || !strcmp(sentence, "끝\n")) break;

		n = get_terms_sent(sentence, Term, &TM, &mode, 1);	/* 1: term weighting */
/*		put_terms(fpout, Term, n, TM.memTermString, 1);*/
		put_terms_line(fpout, Term, n, TM.memTermString);
	}
	/*======================================================================*/

	/*========================= Finalize Indexer ===========================*/
	close_HAM_index(&mode);	/* HAM is not used any more */
	/*======================================================================*/

	if (fpin != stdin) fclose(fpin);if (fpout != stdout) fclose(fpout);
	GOODBYE;	/* good-bye message: 'header/ham-ndx.h' */
	return 0;
}
/*---------------------------- end of index.c --------------------------*/
