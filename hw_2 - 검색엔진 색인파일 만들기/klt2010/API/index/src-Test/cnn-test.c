/*--------------------------------------------------------------------------
	File-name	: coran.c
	Description	: Example of calling Korean Morph. Analyser
			  Hangul I/O code is KSC-5601-1987
	Written-by	: Kang, Seung-Shik at Kookmin University  2001. 6.
----------------------------------------------------------------------------*/
#include "../header/ham-ndx.h"
#include "../header/cnoun-api.h"

/*
	Set HAM options at the command-line argument.
*/
void usage_coran()
{
	puts("usage: coran [-options] [input.txt] [output.txt]\n");
	puts("    -x: output is a list of nouns");
	puts("    -n: output is a list of numbers(syllables or bytes)");
//	GOODBYE(2005);	/* goodbye message: 'header/ham-kma.h' */
}

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
		if (argc == 1) { usage_coran(); return 1; }
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
		usage_coran(); return 1;
	}

	return 0;
}

void error_message(mode)	/* cannot load dictionary files */
HAM_RUNMODE mode;
{
	fprintf(stderr, "ERROR: cannot load dictionary files in <%s>. Error code %d in <header/err-code.h>\n",
		mode.dicpath, mode.err_code);
	fprintf(stderr, "Directory path for dictionaries should be correctly positioned. Or\n");
	fprintf(stderr, "you should modify <src/coran.c> and <hdic/KLT2000.ini>.\n");
	fprintf(stderr, "1. Check or modify the value <DicDirPath> in <hdic/KLT2000.ini>.\n");
	fprintf(stderr, "2. Check or modify <src/coran.c> --> function open_HAM() --> argument <hdic/KLT2000.ini>\n");
}

int K2_or_E1(unsigned char *p)
{
	while (*p) {
		if (!(*p & 0x80)) return 1;
		p += 2;
	}

	return 2;
}

// 복합명사를 구성하는 음절수(or 바이트수) 출려
void put_result_nnn(FILE* fp, char *word, HAM_CNOUN cnoun[], int n_cnoun)
{
	int i, j, nbytes, syl, len;

	len = strlen(word);
	syl = K2_or_E1(word);	/* 1: 아스키문자 포함, 2: 순수 한글 */

	for (i=0; i < n_cnoun; i++) {	// 분해 결과 확인
		fprintf(fp, "\t%2d. ", i+1); 

		for (j=0, nbytes=0; cnoun[i].nnn[j]; j++) {
			fprintf(fp, "%c", cnoun[i].nnn[j] / syl + '0');
			nbytes += cnoun[i].nnn[j];
		}
		for (j; j < 8; j++) fprintf(fp, " ");

		fprintf(fp, ": %s\t-- %5d\n", cnoun[i].dic, cnoun[i].score);
		if (nbytes != len) fprintf(fp, "\tError -- total bytes NOT matched.\n");
	}
}

char *insert_blank(const char *word, char nnn[], char *word2)
{
	int i, n=0;
	char *p=word2, noun_delimiter=' ';

	for (i=0; nnn[i]; i++) {
		strncpy(p, word+n, (int)nnn[i]);
		n += (int)nnn[i];

		p += (int)nnn[i];
		*p++ = noun_delimiter;
	}
	if (i) *(p-1) = '\0';

	return word2;
}

// 단위명사들로 띄어쓴 결과를 출력
void put_result_nouns(FILE* fp, char *word, HAM_CNOUN cnoun[], int n_cnoun)
{
	char result[512];
	int i, syl, len, n=0;

	len = strlen(word);
	syl = K2_or_E1(word);	/* 1: 아스키문자 포함, 2: 순수 한글 */

	for (i=0; i < n_cnoun; i++) {	// 분해 결과 확인
		fprintf(fp, "\t%2d. ", i+1); 

		insert_blank(word, cnoun[i].nnn, result);
		fprintf(fp, "%s", result);
		//for (j; j < 8; j++) fprintf(fp, " ");

		fprintf(fp, "\t: %s\t-- %5d\n", cnoun[i].dic, cnoun[i].score);
	}
}

main(argc, argv)
int argc;
char *argv[];
{
	int flag;	/* temporary variable */
	FILE *fpin=stdin, *fpout=stdout;

	char *optstr=NULL;	/* option string: e.g. "1aC2", "pVc" */
	HAM_RUNMODE mode;	/* HAM running mode: 'header/runmode.h' */

	char word[SENTSIZE];	/* input sentence */
	HAM_CNOUN cnoun[MAXCAND];
	int len, n_cnoun;

	if (set_iofile_ptr(argc, argv, &fpin, &fpout))
		return 0;	/* I/O files open failed */
	if (argc > 1 && argv[1][0] == '-') optstr = argv[1]+1;

	/*=========================== Initialization ===========================*/
	flag = open_HAM_index(&mode, optstr, "./hdic/KLT2000.ini");
	if (flag == 1) {
		error_message(mode); return 0;
	} else if (flag == 2)
		fprintf(stderr, "WARNING: file not found --- <KLT2000.ini>\n");
//	WELCOME(mode.version);	/* welcome message: 'header/ham-kma.h' */

	/*--------------- MORE INTEGRATION GUIDE OF HAM ----------------*/
	/*  There are 2 types of HAM output structure,                  */
	/*     frame-based(HAM internal) and token-based output.        */
	/*  Decide one of them that fits to your applicatoin program.   */
	/*--------------------------------------------------------------*/
	while (fgets(word, 256, fpin)) {
		len = strlen(word);
		if (word[len-1] == 0x0A) word[--len] = '\0';
		printf("%s\n", word);

		n_cnoun = cnoun_decomposition(word, cnoun);	// 복합명사 분해
		if (optstr && *optstr == 'n')	// "C> run.exe -n" 으로 실행
			put_result_nnn(fpout, word, cnoun, n_cnoun);
		else put_result_nouns(fpout, word, cnoun, n_cnoun);
	}

	/*=========================== Finalize KMA ===========================*/
	close_HAM_index(&mode);	/* HAM is not used any more */

	if (fpin != stdin) fclose(fpin);
	if (fpout != stdout) fclose(fpout);
//	GOODBYE(YEAR);	/* good-bye message: 'header/ham-kma.h' */
	
	return 0;
}
/*--------------------------- end of coran.c --------------------------*/
