/*--------------------------------------------------------------------------
	File-name	: index.c
	Description	: Example of calling Korean automatic indexing module.
			  Hangul I/O code is KSC-5601-1987 or KSSM Hangul code.
	Written-by	: Kang, Seung-Shik at Kookmin University  2001. 6.
--------------------------------------------------------------------------*/
#include "../header/ham-ndx.h"

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
		if (argc == 1) { return 1; }
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
		return 1;
	}

	return 0;
}

extern PREFIX int get_stems_1sent(		/* 어절 or 문장 단위 인터페이스 */
	char *sent,	/* KSC5601 input sentence */
	char *termlist,	/* ptr. array of keywords */
	char *filename_KLT2000_ini);	/* running mode of HAM */

main(argc, argv)
int argc; char *argv[];
{
	FILE *fpin=stdin, *fpout=stdout;
	unsigned char sent[SENTSIZE];	/* input word or sentence */
	int n;	/* number of extracted terms */
	unsigned char termlist[SENTSIZE];	// 추출된 term list를 저장하기 위한 문자 배열

	if (set_iofile_ptr(argc, argv, &fpin, &fpout))
		return 0;   /* I/O files open failed */

	//WELCOME(mode.version);	/* welcome message: 'header/ham-ndx.h' */
	while (fscanf(fpin, "%s", sent)) {	if (feof(fpin)) break;
		sent[SENTSIZE-1] = '\0';

		n = get_stems_1sent(sent, termlist, "./hdic/KLT2000.ini");	/* It is NOT thread-safe!!! */
		printf("[[%s]]\n", sent);	// 입력 문장 or 어절 1개
		if (n) printf("\t%s\n", termlist);	// list of terms -- tab 문자로 구분 
	}
	/*======================================================================*/

	if (fpin != stdin) fclose(fpin); if (fpout != stdout) fclose(fpout);
	GOODBYE(2012);	/* good-bye message: 'header/ham-ndx.h' */
	return 0;
}
/*------------------------------ end of index.c ----------------------------*/
