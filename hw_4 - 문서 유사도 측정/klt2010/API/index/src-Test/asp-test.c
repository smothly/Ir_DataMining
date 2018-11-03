/*--------------------------------------------------------------------------
	File-name	: index.c
	Description	: Example of calling Korean automatic indexing module.
			  Hangul I/O code is KSC-5601-1987 or KSSM Hangul code.
	Written-by	: Kang, Seung-Shik at Kookmin University  2001. 6.

  어절 단위 입출력 인터페이스 -- 어절 단위 처리 test 용 등
--------------------------------------------------------------------------*/
#include "../header/ham-ndx.h"
#include "../header/asp-api.h"

/*
	Set HAM options at the command-line argument.
*/
void usage_index()
{
//	WELCOME(VERSION_NUMBER);
	puts("usage: index [-options] [input.txt] [output.txt]\n");
//	GOODBYE(2005);	/* goodbye message: 'header/ham-ndx.h' */
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

int is_include_ascii(HAM_PUCHAR sent) {
	int i, n=strlen(sent);

	for (i=0; i < n; i+=2) {
		if (!(sent[i] & 0x80)) return 1;
		if (sent[i] < 0xB0 || sent[i] > 0xC8 || sent[i+1] < 0xA1 || sent[i+1] == 0xFF) return 1;
	}

	return 0;
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

	/*========================= 2. Extract Terms ===========================*/
	while (fscanf(fpin, "%s", sent)) {	if (feof(fpin)) break;
		if (ispunct(sent[strlen(sent)-1])) sent[strlen(sent)-1] = 0;	/* 문장부호 제거 */

		adjust_word_spacing(sent, 1);	/* 자동 띄어쓰기 -- 1: sent의 blank 유지, 0: sent의 blank 무시 */
//		n = adjust_word_spacing_nsyl(sent, sent2, SENTSIZE, mode.autospac);	/* 자동 띄어쓰기 -- 1: sent의 blank 유지, 0: sent의 blank 무시 */
		printf("%s\n", sent);

//		n = get_stems(sent, term, &mode);	/* It is NOT thread-safe!!! */
//		put_stems(fpout, sent, term, n, &mode);
	}
	/*======================================================================*/

	/*======================== 3. Finalize Indexer =========================*/
	close_HAM_index(&mode);	/* HAM is not used any more */
	/*======================================================================*/

	if (fpin != stdin) fclose(fpin); if (fpout != stdout) fclose(fpout);
	GOODBYE(2005);	/* good-bye message: 'header/ham-ndx.h' */
	return 0;
}
/*------------------------------ end of index.c ----------------------------*/
