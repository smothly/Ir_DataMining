/*--------------------------------------------------------------------------
	File-name	: indexS-simple.c
	Description	: Example of calling Korean automatic indexing module.
			  Hangul I/O code is KSC-5601-1987 or KSSM Hangul code.
	Written-by	: Kang, Seung-Shik, 2015�� 1��

<<���ǻ���>> Linux/Unix���� �������� �ȵ� �� -- �Ʒ� ������ commemtó��(header/ham-api.h)
	//#define WINDOWS_DLL
--------------------------------------------------------------------------*/
#include "../header/ham-ndx.h"

//#define WINDOWS_DLL
#ifdef WINDOWS_DLL			// DLL export
#define PREFIX	_declspec(dllimport)
#elif defined(WINDOWS_DLL_CPP)	// ���� VC++�� C++ �������̽�
#define PREFIX	"C" _declspec(dllimport)
#else						// Linux/Unix �� ���н� �迭
#define PREFIX
#endif

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
		if (argc == 1) { puts("C> run.exe in.txt out.txt"); return 1; }
		break;
	case 2:
		*fpin = fopen(argv[i], "r");
		if (!*fpin) { fprintf(stderr, "No such file <%s>\n", argv[i]); return 1; }
		break;
	case 3:
		*fpin = fopen(argv[i], "r");
		if (!*fpin) { fprintf(stderr, "No such file <%s>\n", argv[i]); return 1; }
		*fpout = fopen(argv[i+1], "r");
		if (*fpout) {
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

/*---------------------------------------------------------------------------
���ο� �������̽� -- �ʱ�ȭ/�����Լ� open_HAM_index(), close_HAM_index() �ʿ� ����
---------------------------------------------------------------------------*/
extern PREFIX int get_stems_simple(		// ���� or ���� ���� �������̽�
	char *sent,	// KSC5601 input sentence
	char *toklist,	// list of keywords -- �� ���ڷ� ����
	char *optstr,	// option string -- NULL for default options
	char *inifile);	// option file name
extern PREFIX int get_stems_simple2(		// ���� or ���� ���� �������̽�
	char *sent,	// KSC5601 input sentence
	char *toklist,	// list of keywords -- �� ���ڷ� ����
	char *optstr,	// option string -- NULL for default options
	char *inifile);	// option file name

extern PREFIX char *check_status(char *p);	// ���� �ε� ���� Ȯ��
extern PREFIX int set_option_keyword(int i);	// ���ξ� ���� �ɼ� ����

extern int get_sent_or_line(FILE *fp, unsigned char *sent, unsigned char inputmode);	// readsent.c

int main(int argc, char *argv[])
{
	FILE *fpin=stdin, *fpout=stdout;
	char *optstr=NULL;	// option string: e.g. "1aC2", "pVc -- NULL for default options

	char sent[SENTSIZE];	// input word or sentence
	int len;
	int n;	// number of extracted terms
	char toklist[SENTSIZE];	// ����� term list�� �����ϱ� ���� ���� �迭

	if (set_iofile_ptr(argc, argv, &fpin, &fpout)) return 0;   // I/O files open failed
	WELCOME(VERSION_NUMBER);	// welcome message -- 'header/ham-ndx.h'

//	while (fscanf(fpin, "%s", sent)) {	if (feof(fpin)) break;
//	while (get_sent_or_line(fpin, sent, 0)) {	if (feof(fpin)) break;
	while (fgets(sent, SENTSIZE-1, fpin)) {	if (feof(fpin)) break;
		sent[SENTSIZE-1] = '\0';
		len = strlen(sent);
		if (sent[len-1] == '\n' || sent[len-1] == '\r') { len--; sent[len] = 0; }
		if (len == 0) continue;

		fprintf(fpout, "[[%s]]\n", sent);	// �Է� -- ���� or ���� 1��

		//n = get_stems_simple(sent, toklist, optstr, "./hdic/KLT2000.ini");
		n = get_stems_simple2(sent, toklist, optstr, "./hdic/KLT2000.ini");

		fprintf(fpout, "Number of keywords = %d (followed by TAB)\n", n);
		if (n) fprintf(fpout, "\t%s\n", toklist);	// list of terms -- tab ���ڷ� ���� 
	}
	/*======================================================================*/
//	puts(check_status(sent)); set_option_keyword(4); puts(check_status(sent));

	if (fpin != stdin) fclose(fpin); if (fpout != stdout) fclose(fpout);

	GOODBYE(YEAR);	// good-bye message -- 'header/ham-ndx.h'
	return 0;
}
/*------------------------------ end of index.c ----------------------------*/
