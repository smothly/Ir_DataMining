/*--------------------------------------------------------------------------
	File-name	: index.c
	Description	: Example of calling Korean automatic indexing module.
			  Hangul I/O code is KSC-5601-1987 or KSSM Hangul code.
	Written-by	: Kang, Seung-Shik, 2013년 8월

<<주의사항>>
Linux/Unix에서 컴파일이 안될 때 -- 아래 라인을 commemt처리(header/ham-api.h)
	//#define WINDOWS_DLL
--------------------------------------------------------------------------*/
#include "../header/ham-ndx.h"

//#define WINDOWS_DLL
#ifdef WINDOWS_DLL			// DLL export
#define PREFIX	_declspec(dllimport)
#elif defined(WINDOWS_DLL_CPP)	// 윈도 VC++의 C++ 인터페이스
#define PREFIX	"C" _declspec(dllimport)
#else						// Linux/Unix 등 유닉스 계열
#define PREFIX
#endif

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
	case 4:
		*fpin = fopen(argv[i], "r");
		if (!*fpin) { fprintf(stderr, "No such file <%s>\n", argv[i]); return 1; }
		*fpout = fopen(argv[i+1], "r");
		if (*fpout) {
			fprintf(stderr, "File <%s> exists. Overwrite it? ", argv[i+1]);
			if (tolower(fgetc(stdin)) != 'y') return 1;
		}
		*fpout = fopen(argv[i+1], "w");
		break;
	default:
		usage_index();
		return 1;
	}

	return 0;
}

/*
	char *sent; -- input sentence
	unsigned char *term[];	//list of keywords
	int n;	// number of keywords at 'keywords[]'
	int outmode;	// output mode: 1 - 한 라인에 여러 개, 2: 한 라인에 1개씩 출력
*/
void put_terms(FILE *fp, char *sent, char *term[], int n, int outmode)
{
	int i;

	// 출력형식-1. 어절당 한 line씩 -- 여러 개 용어를 한 줄에 출력
	if (outmode == 1) {
		for (i = 0; i < n; i++) {
			fprintf(fp, "\t%s", term[i]);
			if (i == n-1 || *(term[i]-2) != *(term[i+1]-2)) fprintf(fp, "\n");
		}
	} else if (outmode == 2) { // 출력형식-2. 한 line에 term을 1개씩 출력
		for (i = 0; i < n; i++)
			fprintf(fp, "\t%3d:%c:%s\n", *(term[i]-2), *(term[i]-1), term[i]);
	}
}

/*---------------------------------------------------------------------------
 2013년도 새로운 인터페이스
 - 초기화/종료함수 open_HAM_index(), close_HAM_index() 필요 없음
---------------------------------------------------------------------------*/
extern PREFIX int get_stems_TS_simple(	// 어절 or 문장 단위 & multi-thread
	char *sent,	// KSC5601 input sentence
	char *keys[],	// ptr. array of keywords
	char *keylist,	// keyword saving area
	HAM_PMORES hamout,	// morph. analysis result
	char *optstr,	// option string -- NULL for default options
	char *inifile);	// running mode of HAM
extern PREFIX int get_stems_simple(		// 어절 or 문장 단위 인터페이스
	char *sent,	// KSC5601 input sentence
	char *toklist,	// list of keywords -- 탭 문자로 구분
	char *optstr,	// option string -- NULL for default options
	char *inifile);	// option file name
extern PREFIX int get_stems_simple2(		// 어절 or 문장 단위 인터페이스
	char *sent,	// KSC5601 input sentence
	char *toklist,	// list of keywords -- 탭 문자로 구분
	char *optstr,	// option string -- NULL for default options
	char *inifile);	// option file name

extern PREFIX char *check_status(char *p);	// 사전 로딩 상태 확인
extern PREFIX int set_option_keyword(int i);	// 색인어 추출 옵션 셋팅

// 'KeyMem', 'HamOut' is needed ONLY for THREAD-SAFE running
unsigned char KeyMem[MAXKEYBYTES];	// TEMPORARY MEMORY: keyword string
HAM_MORES HamOut;	// TEMPORARY MEMORY: morph. analysis result

// Defualt -- "./hdic/KLT2000.ini"
// 2018/02/08 sskang
int set_option_filename(int argc, char *argv[], char *fname_INI)
{
	FILE *fp;
	char *p;

	strcpy(fname_INI, argv[0]);
	p = strstr(fname_INI, "INDEX");
	if (!p) p = strstr(fname_INI, "index");
	if (p) *p = 0;
	strcat(fname_INI, "hdic/KLT2000.ini");

	if (argc < 2) return argc;
	if (argv[argc-1][0] == '-' && argv[argc-1][1] == 'O') {	// "-Oc:\KLT2000-TestVersion\EXE\hdic\KLT2000.ini"
		strcpy(fname_INI, argv[argc-1]+2);
		argc--;
	}

	fp = fopen(fname_INI, "r");
	if (fp == NULL)
		fprintf(stderr, "No such file <%s>. Check the file name and dir. path.\n", fname_INI);
	else fclose(fp);
	return argc;
}

void replace_tab_by_LF(char *toklist)
{
	char *p=toklist;

	while (*p) {
		if (*p == '\t') *p = '\n';
		p++;
	}
}

int main(int argc, char *argv[])
{
	FILE *fpin=stdin, *fpout=stdout;
	char *optstr="NONE";	// option string: e.g. "1aC2", "pVc -- NULL for default options
	int outmode=2;	// 1: 한 라인에 여러 개, 2: 한 라인에 1개씩 출력

	char sent[SENTSIZE];	// input word or sentence
	int len;
	int n;	// number of extracted terms
	char toklist[SENTSIZE];	// 추출된 term list를 저장하기 위한 문자 배열
	char *term[MAXKEYWORDS];	// extracted terms

	char fname_INI[1024]; // Default option file name KLT2000.ini -- 2018/02/08 sskang
	argc = set_option_filename(argc, argv, fname_INI);	// Defualt -- "./hdic/KLT2000.ini"

	if (set_iofile_ptr(argc, argv, &fpin, &fpout))
		return 0;   // I/O files open failed
	if (argc > 1 && argv[1][0] == '-') optstr = argv[1]+1;

//	WELCOME(VERSION_NUMBER);	// welcome message: 'header/ham-ndx.h'
//	while (fscanf(fpin, "%s", sent)) {	if (feof(fpin)) break;
	while (fgets(sent, SENTSIZE-1, fpin)) {	if (feof(fpin)) break;
		sent[SENTSIZE-1] = '\0';
		len = strlen(sent);
		if (sent[len-1] == '\n' || sent[len-1] == '\r') { len--; sent[len] = 0; }
		if (len == 0) continue;

		if (optstr[0] == 'S') fprintf(fpout, "[[%s]]\n", sent);	// 입력 문장 or 어절 1개

#ifdef THREAD_SAFE
		n = get_stems_TS_simple(sent, term, KeyMem, &HamOut, fname_INI);	// thread-safe version of 'get_stems()'
		put_terms(fpout, sent, term, n, outmode);	// outmode -- 1: 한 라인에 여러 개, 2: 한 라인에 1개씩 출력
#else
		if (outmode == 1)
			n = get_stems_simple(sent, toklist, optstr, fname_INI);
		else n = get_stems_simple2(sent, toklist, optstr, fname_INI);
		if (n) {
			if (!strcmp(argv[argc-1], "-Wordcloud"))
				replace_tab_by_LF(toklist);
			fprintf(fpout, "%s\n", toklist);	// list of terms -- tab 문자로 구분 
		}
#endif
	}
	/*======================================================================*/
//	puts(check_status(sent)); set_option_keyword(4); puts(check_status(sent));

	if (fpin != stdin) fclose(fpin); if (fpout != stdout) fclose(fpout);
//	GOODBYE(YEAR);	/* good-bye message: 'header/ham-ndx.h' */
	return 0;
}
/*------------------------------ end of index.c ----------------------------*/
