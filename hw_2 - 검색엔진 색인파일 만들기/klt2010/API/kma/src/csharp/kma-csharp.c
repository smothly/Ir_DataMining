/*--------------------------------------------------------------------------
	File-name	: coran.c
	Description	: Example of calling Korean Morph. Analyser
			  Hangul I/O code is KSC-5601-1987
	Written-by	: Kang, Seung-Shik, 2013. 08.
----------------------------------------------------------------------------*/
#include "../header/ham-kma.h"

extern void hamout_HAM(FILE *fp, HAM_PMORES p, HAM_PRUNMODE mode);	// out_ham_csharp.c

/*
	Set I/O types: following 3 types are possible.
		1. stdin & stdout
		2. file input & stdout
		3. file input & file output
*/
int set_iofile_ptr(argc, argv, fpin, fpout)
int argc;
char *argv[];
FILE **fpin;
FILE **fpout;
{
	int i=1, nargs=argc;

	if (argc > 1 && argv[1][0] == '-') { i++; nargs--; }

	switch (nargs) {
	case 1:
		if (argc == 1) { return 1; }
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
		*fpout = fopen(argv[i+1], "wb");
		break;
	default:
		return 1;
	}

	return 0;
}

main_ORG(argc, argv)
int argc;
char *argv[];
{
	FILE *fpin=stdin, *fpout=stdout;
	HAM_RUNMODE mode;	/* HAM running mode: 'header/runmode.h' */
	int flag;

	unsigned char sent[SENTSIZE];	/* input sentence */
	HAM_PMORES hamout1;	/* frame-based morph. analysis output */

	if (set_iofile_ptr(argc, argv, &fpin, &fpout))
		return 0;	/* I/O files open failed */

	flag = open_HAM(&mode, NULL, "./hdic/KLT2000.ini");
	while (fscanf(fpin, "%s", sent) == 1) {
		hamout1 = morph_anal(sent, NULL, &mode);
		hamout_HAM(fpout, hamout1, &mode);
	}

	if (fpin != stdin) fclose(fpin);
	if (fpout != stdout) fclose(fpout);
	GOODBYE(2013);	/* good-bye message: 'header/ham-kma.h' */
	
	return 0;
}

HAM_MORES KmaResult;
extern PREFIX int morph_anal_simple2(char *sent, HAM_PMORES hamout, char *inifile);

main_Csharp_TEST(argc, argv)
int argc;
char *argv[];
{
	FILE *fpin=stdin, *fpout=stdout;
	HAM_RUNMODE mode;	/* HAM running mode: 'header/runmode.h' */
	int flag;
	unsigned char sent[SENTSIZE];	/* input sentence */

	if (set_iofile_ptr(argc, argv, &fpin, &fpout))
		return 0;	/* I/O files open failed */

	while (fscanf(fpin, "%s", sent) == 1) {
		flag = morph_anal_simple2(sent, &KmaResult, "./hdic/KLT2000.ini");
		if (flag == 0) continue;
		mode.outmode = mode.echosent = mode.echoword = mode.hcode_out = 1;	// 'mode'가 초기화되어 있지 않음 -- 임시로 사용
		hamout_HAM(fpout, &KmaResult, &mode);
	}

	if (fpin != stdin) fclose(fpin);
	if (fpout != stdout) fclose(fpout);
	GOODBYE(2013);	/* good-bye message: 'header/ham-kma.h' */
	
	return 0;
}

extern PREFIX HAM_MORES morph_anal_simple(char *sent, char *inifile);

main(argc, argv)
int argc;
char *argv[];
{
	FILE *fpin=stdin, *fpout=stdout;
	HAM_RUNMODE mode;	/* HAM running mode: 'header/runmode.h' */
	unsigned char sent[SENTSIZE];	/* input sentence */

	if (set_iofile_ptr(argc, argv, &fpin, &fpout))
		return 0;	/* I/O files open failed */

	while (fscanf(fpin, "%s", sent) == 1) {
		KmaResult = morph_anal_simple(sent, "./hdic/KLT2000.ini");
		mode.outmode = mode.echosent = mode.echoword = mode.hcode_out = 1;	// 'mode'가 초기화되어 있지 않음 -- 임시로 사용
		hamout_HAM(fpout, &KmaResult, &mode);
		// <주의> i번째 어절에 대한 분석 결과 개수 KmaResult.word[i].gr[0] == 0 이면 분석결과 없음
	}

	if (fpin != stdin) fclose(fpin);
	if (fpout != stdout) fclose(fpout);
	GOODBYE(2013);	/* good-bye message: 'header/ham-kma.h' */
	
	return 0;
}
/*--------------------------- end of kma.c --------------------------*/
