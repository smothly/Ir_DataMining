/*--------------------------------------------------------------------------
	File-name	: index.c
	Description	: Example of calling Korean automatic indexing module.
			  Hangul I/O code is KSC-5601-1987 or KSSM Hangul code.
	Written-by	: Kang, Seung-Shik at Kookmin University  2001. 6.
--------------------------------------------------------------------------*/
#include "../header/ham-ndx.h"

extern int get_sent_or_line();	/* src/readsent.c */

/*
	Set HAM options at the command-line argument.
*/
void usage_index()
{
	WELCOME(VERSION_NUMBER);
	puts("usage: index [-options] [input.txt] [output.txt]\n");
	puts("    no options & I/O files --> default options applied");
	puts("\t-x: analysis results: default options");
	puts("\t-k: exclude SOME SINGLE NOUNS IN DIC.");
	puts("\t-K: exclude MOST SINGLE NOUNS IN DIC.");
	puts("\t-j: normalize Arabic/Hangul numerals\n");

	puts("\t-A: automatic spacing for input sentence");
/*
	puts("\t-T: general auto. spacing: NOT titles\n");
*/
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
	puts("\t-I: extract input string itself & not duplicated");
	puts("\t-q: extract longest stem only for query processing\n");
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
		*fpin = fopen(argv[i], "r");
		if (!*fpin) { fprintf(stderr, "No such file <%s>\n", argv[i]); return 1; }
		break;
	case 3:
		*fpin = fopen(argv[i], "r");
		if (!*fpin) { fprintf(stderr, "No such file <%s>\n", argv[i]); return 1; }
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

/*	Output test for 'get_stems()'.
		*(term[i]-2) --- word id. at a sentence: 0~255
		*(term[i]-1) --- term type(defined in 'header/keyword.h')
		term[i]      --- term string
*/
void put_stems(fp, sent, term, n, mode)
FILE *fp;
unsigned char *sent;	/* input sentence */
unsigned char *term[];	/* list of keywords */
int n;	/* number of keywords at 'keywords[]' */
HAM_RUNMODE *mode;	/* HAM running mode */
{
	int i;

	if (mode->echosent) fprintf(fp, "[[%s]]\n", sent);

	/* 출력형식-1. 어절당 한 line씩 -- 여러 개 용어를 한 줄에 출력 */
	if (mode->outmode == 1) {
		for (i = 0; i < n; i++) {
			fprintf(fp, "\t%s", term[i]);
			if (i == n-1 || *(term[i]-2) != *(term[i+1]-2)) fprintf(fp, "\n");
		}
	} else if (mode->outmode == 2) { /* 출력형식-2. 한 line에 term을 1개씩 출력 */
		for (i = 0; i < n; i++)
			fprintf(fp, "\t%3d:%c:%s\n", *(term[i]-2), *(term[i]-1), term[i]);
	}
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

/* 'KeyMem', 'HamOut' is needed ONLY for THREAD-SAFE running */
unsigned char KeyMem[MAXKEYBYTES];	/* TEMPORARY MEMORY: keyword string */
HAM_MORES HamOut;	/* TEMPORARY MEMORY: morph. analysis result */

/*---------- INTEGRATION GUIDE of HAM to app. system -----------*/
/*                                                              */
/* 0. You should include 'header/ham.h'.                        */
/*                                                              */
/* 1. Following variables are required for HAM                  */
/*       char sent[MAX];  -->  KSC5601 input Korean sentence    */
/*       char *keys[MAXKEYWORDS]; -->  ptr. array of keywords   */
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
/* 3. Call 'get_stems(sent, keys, mode)' or                     */
/*       'get_stems_TS(sent, keys, KeyMem, &HamOut, mode)'.     */
/*       Extracted keywords(nouns) from the sentence are        */
/*       confirmed by 'put_stems(fpout, sent, keys, nkeys)'.    */
/*                                                              */
/* 4. Call 'close_HAM()', if get_stems_?() is no nonger used.   */
/*                                                              */
/* Only 3 functions are required for integration to appl. system*/
/*        open_HAM_index(), get_stems_?(), close_HAM()          */
/*                                                              */
/*--------------------------------------------------------------*/
main(argc, argv)
int argc; char *argv[];
{
	FILE *fpin=stdin, *fpout=stdout;

	char *optstr=NULL;	/* option string: e.g. "1aC2", "pVc" */
	HAM_RUNMODE mode;	/* HAM running mode: 'header/runmode.h' */

	unsigned char sent[SENTSIZE];	/* input word or sentence */
	int n;	/* number of extracted terms */
	unsigned char *term[MAXKEYWORDS];	/* extracted terms */

	if (set_iofile_ptr(argc, argv, &fpin, &fpout))
		return 0;   /* I/O files open failed */
	if (argc > 1 && argv[1][0] == '-') optstr = argv[1]+1;

	/*========================= 1. Initialization ==========================*/
	if (open_HAM_index(&mode, optstr, "./hdic/KLT2000.ini")) {
		error_message(mode); return 0;
	} else WELCOME(mode.version);	/* welcome message: 'header/ham-ndx.h' */
	/*======================================================================*/

	/* 옵션파일 "KLT2000.ini"가 적용되는지를 확인하는 방법 */
/*	if (set_options_from_file("/home/kang/KLT2000.ini", &mode))
		fprintf(stderr, "WARNING: file not found --- <KLT2000.ini>\n");
*/

	/*========================= 2. Extract Terms ===========================*/
	while (fscanf(fpin, "%s", sent)) {	if (feof(fpin)) break;
		sent[SENTSIZE-1] = '\0';
//		if (mode.autospac) adjust_word_spacing(sent, 1);	/* 자동 띄어쓰기 -- 1: sent의 blank 유지, 0: sent의 blank 무시 */

		n = get_stems(sent, term, &mode);	/* It is NOT thread-safe!!! */
//		n = get_stems_TS(sent, term, KeyMem, &HamOut, &mode);	/* thread-safe version of 'get_stems()' */

		put_stems(fpout, sent, term, n, &mode);
	}
	/*======================================================================*/

	/*======================== 3. Finalize Indexer =========================*/
	close_HAM_index(&mode);	/* HAM is not used any more */
	/*======================================================================*/

	if (fpin != stdin) fclose(fpin); if (fpout != stdout) fclose(fpout);
	GOODBYE(YEAR);	/* good-bye message: 'header/ham-ndx.h' */
	return 0;
}
/*------------------------------ end of index.c ----------------------------*/
