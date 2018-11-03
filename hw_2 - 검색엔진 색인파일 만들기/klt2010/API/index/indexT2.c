/*--------------------------------------------------------------------------
	File-name	: index-text.c
	Description	: Example of calling Korean automatic indexing module.
			  Hangul I/O code is KSC-5601-1987 or KSSM Hangul code.
	Written-by	: Kang, Seung-Shik at Kookmin University  2001. 6.
--------------------------------------------------------------------------*/
#include <ctype.h>
#include <stdlib.h>

#include "../header/ham-ndx.h"
#include "../header/ham-api.h"

/*========================== Global Variable ===========================*/
HAM_TERMLIST Term[MAX_TERMS_DOC];	/* extracted terms */
HAM_TERMMEMORY TM;	/* termtext.h: memories needed for term extraction */
/*======================================================================*/

/* 텍스트 파일 전체를 memory로 load */
unsigned char *load_text_file(fname)
char *fname;
{
	FILE *fp;
	long n;
	unsigned char *p;

	if (!(fp=fopen(fname, "rb"))) return NULL;

	fseek(fp, 0L, 2);
	n = ftell(fp);	/* n: byte size of file 'fp' */

	fseek(fp, 0L, 0);
	p = (unsigned char *) malloc(n+1);	/* memory allocation */
	if (p == NULL) return NULL;

	fread(p, sizeof(unsigned char), n, fp);	/* read 'fp' to 'p' */
	*(p+n) = '\0';

	fclose(fp);
	return p;
}

/* 추출된 용어 출력 */
void put_terms_invFile(fp, term, n, ts, docid)
FILE *fp;
HAM_TERMLIST term[];/* 추출된 용어들의 정보 테이블 */
int n;				/* 추출된 용어 개수 */
unsigned char ts[];	/* 용어들이 NULL 문자 '\0'으로 구분되어 저장되어 있는 memory pool */
		/* ts[] --> "term1<null>term2<null>term3<null>...termN<null>" */
		/* term[i].offset --> 용어 i의 ts[] 저장 위치를 가리키는 정수 */
int docid;	/* 문서 ID --- 각 문서의 이름을 0~n 사이의 정수값으로 */
{
	int i, j, nlocs;
/*
	fprintf(fp, "------------------------------------------------------\n");
	fprintf(fp, " No: Term  DocID  Score\t\t Loc1\t Loc2\t Loc3\n");
	fprintf(fp, "------------------------------------------------------\n");
*/
	for (i = 0; i < n; i++) {
		fprintf(fp, "%s", ts+term[i].offset);	/* ts+term[i].offset : 용어 스트링 */
		if (strlen(ts+term[i].offset) <= 7) fprintf(fp, "\t");	/* 열 맞춤 */

		fprintf(fp, "\t%7d", docid);	/* 문서 id */
		fprintf(fp, "\t%5u", term[i].score / 10);	/* 용어의 가중치 */

		nlocs = (term[i].tf > MAX_LOCS_PER_TERM ? MAX_LOCS_PER_TERM : term[i].tf);
		for (j = 0; j < nlocs; j++)	/* 용어의 위치정보 최대 MAX_LOCS_PER_TERM 개 */
			fprintf(fp, "\t%5u", term[i].loc[j]);
		fprintf(fp, "\n");
	}

//	fprintf(fp, "------------------------------------------------------\n");
}

void usage_index()	/* HAM options in command-line argument */
{
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

void error_message(mode)	/* cannot load dictionary files */
HAM_RUNMODE mode;
{
	fprintf(stderr, "ERROR: cannot load dictionary files in <%s>. Error code %d in <header/err-code.h>\n",
		mode.dicpath, mode.err_code);
	fprintf(stderr, "Directory path for dictionaries should be correctly positioned. Or\n");
	fprintf(stderr, "you should modify <src/text.c> and <hdic/ham2000.ini>.\n");
	fprintf(stderr, "1. Check or modify the value <DicDirPath> in <hdic/ham2000.ini>.\n");
	fprintf(stderr, "2. Check or modify <src/text.c> --> function open_HAM() --> argument <hdic/ham2000.ini>\n");
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
	FILE *fp, *fpout=stdout;

	char *optstr=NULL;	/* option string: e.g. "1apVC", "pVc" */
	HAM_RUNMODE mode;	/* HAM running mode: 'header/runmode.h' */

	unsigned char *text;	/* input text: 입력 파일 전체 */
	int n=0;	/* 추출된 용어수: 명령행 인자로 용어의 최대 개수 지정 */
	int docid=0;

	char filename[99];	/* file name in which doc. file names are listed */
	char fname[999] = "d:\\R\\Ham6\\Release6\\Ham600-IR\\";
	char *p_fname=fname+strlen(fname);

	if (argc > 1 && argv[1][0] == '-')
		n = atoi(argv[1]+1);	/* 출력할 term의 최대 개수 지정 */
	/*if (argc > 1 && argv[1][0] == '-') optstr = argv[1]+1;*/

	/*=========================== Initialization ===========================*/
	if (open_HAM_index(&mode, optstr, "./hdic/ham2000.ini")) {
		error_message(mode); return 0;
	}
	/*======================================================================*/

	fprintf(stderr, "File name of doc. file list, ex. DocNames.lst : "); scanf("%s", filename);
	if (!(fp = fopen(filename, "r"))) {
		fprintf(stderr, "File not found: <%s>", filename); return 1; }
	fprintf(stderr, "Doc. file directory path, ex. /usr/kang/text/ : "); scanf("%s", fname);
	p_fname = fname + strlen(fname);	/* 경로를 제외한 순수한 파일이름 시작 위치 */

	while (fgets(p_fname, 999, fp)) {
		if (fname[strlen(fname)-1] == '\n' || fname[strlen(fname)-1] == '\r')
			fname[strlen(fname)-1] = 0;
		if (fname[strlen(fname)-1] == '\n' || fname[strlen(fname)-1] == '\r')
			fname[strlen(fname)-1] = 0;
		if ((text = load_text_file(fname)) == NULL) {
			fprintf(stderr, "No such file : [%s]\n", fname);
			continue;
		}

		/*=========================== Extract Terms ============================*/
		n = get_terms_text(text, Term, &TM, &mode, n, 3, 0);
		/*------------------------------------------------------------------------
		- 다섯번째 인자 -- 0/n/-n(최대로 추출되는 용어수)
			n: 최대 추출 용어수, -n: 빈도 n이상(or n 퍼센트), 0: 모든 용어 추출
			[참고] n개 선택 기준: sortflag(소팅 방식)에 따라 '가중치' or '소팅 옵션'
			  - sortflag가 양수 1/2/3/4 -- 가중치 순으로 n개 선택
			  - sortflag가 음수 -1/-2/-3/-4 -- 소팅 방식에 따라 소팅 후 n개 선택
			[참고] n <= -10 이면, -n 퍼센트만큼 용어수를 출력함.

		- 여섯번째 인자 -- 0/1/2/3/4(소팅 방식)
			0: 소팅 안함, 1: '가나다'순, 2: 빈도순, 3: 가중치, 4: 어절위치
			[참고] n개 선택 기준을 '소팅 방식'에 따라 선택하려면 음수로 줌

		- 마지막 인자 -- 0/1(어절위치 numbering 방식)
			0: 어절순서로, 1: 문장별로 100부터 시작
		========================================================================*/

//		docid = get_docid(fname);	/* 문서 ID: 0부터 시작하여 1씩 증가? */
		put_terms_invFile(fpout, Term, n, TM.memTermString, docid++);

		free(text);	/* malloc() in 'load_text()' */
	}

	fprintf(stderr, "Total <%d> files are processed.\n", docid);
	fclose(fp);
	if (fpout != stdout) fclose(fpout);
	/*========================= Finalize Indexer ===========================*/
	close_HAM_index(&mode);	/* HAM is not used any more */
	/*======================================================================*/

	return 0;
}
/*---------------------------- end of index-text.c --------------------------*/
