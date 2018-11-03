/*
	웹 질의어 처리 -- 복합명사 분해, 명사구 처리 등

	1. 띄어쓰기가 되어 있는 것(blank가 1개 이상) : 복합명사, 명사구, 문장
		- 조사/어미가 분리된 것은 별도의 파일로...
		- 4음절 이상인 것은 복합명사 분해

	2. 띄어쓰기가 안 되어 있는 것(blank 없음) : 복합명사, 명사구, 문장
		- 자동 띄어쓰기 적용

	3. 1음절씩 띄어쓴 것 --> 모두 붙인 후에 2.를 적용

	4. 기타 -- 3음절 이하의 띄어쓰기 오류 예
		'모든것', '하는법'

	입력 파일을 단어/line 유형에 따라 별도의 파일로 출력
		- 영문자가 포함된 것
		- 숫자가 포함된 것
		- 2 바이트 특수문자가 포함된 것

		- 한글 중 조사/어미가 포함된 것
		- 한글 중 순수한 명사/복합명사 들만 추출
*/

#include "../header/ham-ndx.h"
#include "../header/ham-api.h"
#include "../header/asp-api.h"

extern int query_processing(char *query, char *nounlist, int josaflag, char separator,
	HAM_RUNMODE *mode, int *q_type);

#define MAXSIZ	1024
#define N_FILES	10

/* Count the number of blanks in 'line' */
int count_blanks(unsigned char *line)
{
	int n=0;
	unsigned char *p = line;

	while (*p) {
		if (*p == ' ') n++;
		p++;
	}

	return n;
}

/* get next line/word */
int get_next_input(FILE *fp, char line[], char inputType)
{
	int len;

	if (inputType == 'l')	// line
		fgets(line, MAXSIZ-100, fp);
	else fscanf(fp, "%s", line);	// word
	len = strlen(line);
	if (line[len-1] == '\n') line[len-1] = 0;

	return (!feof(fp));
}

void put_statistic_data(int count[])
{
	int i, total=0;

	for (i=0; i < N_FILES; i++) {		// 유형별 질의어 개수 통계 출력
		if (i) printf(" + ");
		printf("%d", count[i]);
		total += count[i];
	}	printf(" = %d\n", total);
}

int main(int argc, char *argv[]) {
	FILE *fpin=stdin, *fp[N_FILES];
	char *fname[] = { "Knn.txt", "Knn1.txt", "Knn2.txt", "Knn3.txt", "Knn4.txt", "Knj.txt", "Knum.txt", "Keng.txt", "Kpunc.txt", "Kdbcs.txt" };
	int q_type;	/* 질의어 유형 : 명사/조사/용언/부사 등 */

	HAM_RUNMODE mode;	/* running mode of HAM: 'header/runmode.h' */

	int i, n, nargs=argc, count[N_FILES]={0};
	char inputType='l';	// 'w' -- word, 'l' -- line
	char line[MAXSIZ], nounlist[MAXSIZ];

	int josaflag=1;
	char separator='/';

	if (argc >= 2 && argv[1][0] =='-') {
		inputType = argv[1][1];
		nargs--;
	}

	if (nargs == 2) {
		fpin = fopen(argv[argc-1], "r");
		if (!fpin) { printf("Error file open : <%s>\n", argv[argc-1]); return 1; }
	}

//	for (i=0; i < N_FILES; i++) fp[i] = fopen(fname[i], "w");

	if (open_HAM_index(&mode, NULL, "./hdic/KLT2000.ini")) {	/* initialize HAM */
		puts("ERROR: dictionary file path in cn-test.c");
		return 0;
	}

	if (!mode.autospac)
		load_bigram_dpath((char*) mode.dicpath, "bigram.dic");
	mode.autospac = 4;	// 자동 띄어쓰기 옵션 조절 -- 4음절 이상으로... 
	mode.jomi_var = 0;	// 조사/어이의 원형 복원 안함 

	while (1) {
		if (!get_next_input(fpin, line, inputType)) break;
		n = query_processing(line, nounlist, josaflag, separator, &mode, &q_type);

		count[q_type]++;
//		fprintf(fp[q_type], "%s\t: %s\n", line, nounlist);
		fprintf(stdout, "%s\t: %s\n", line, nounlist);
	}

//	put_statistic_data(count);	// 질의어 유형별 개수 통계 출력
//	for (i=0; i < N_FILES; i++) fclose(fp[i]);

	close_HAM_index(&mode);	/* HAM is not used any more */
	return 0;
}
