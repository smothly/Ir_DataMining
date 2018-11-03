/*
	�� ���Ǿ� ó�� -- ���ո�� ����, ��籸 ó�� ��

	1. ���Ⱑ �Ǿ� �ִ� ��(blank�� 1�� �̻�) : ���ո��, ��籸, ����
		- ����/��̰� �и��� ���� ������ ���Ϸ�...
		- 4���� �̻��� ���� ���ո�� ����

	2. ���Ⱑ �� �Ǿ� �ִ� ��(blank ����) : ���ո��, ��籸, ����
		- �ڵ� ���� ����

	3. 1������ �� �� --> ��� ���� �Ŀ� 2.�� ����

	4. ��Ÿ -- 3���� ������ ���� ���� ��
		'����', '�ϴ¹�'

	�Է� ������ �ܾ�/line ������ ���� ������ ���Ϸ� ���
		- �����ڰ� ���Ե� ��
		- ���ڰ� ���Ե� ��
		- 2 ����Ʈ Ư�����ڰ� ���Ե� ��

		- �ѱ� �� ����/��̰� ���Ե� ��
		- �ѱ� �� ������ ���/���ո�� �鸸 ����
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

	for (i=0; i < N_FILES; i++) {		// ������ ���Ǿ� ���� ��� ���
		if (i) printf(" + ");
		printf("%d", count[i]);
		total += count[i];
	}	printf(" = %d\n", total);
}

int main(int argc, char *argv[]) {
	FILE *fpin=stdin, *fp[N_FILES];
	char *fname[] = { "Knn.txt", "Knn1.txt", "Knn2.txt", "Knn3.txt", "Knn4.txt", "Knj.txt", "Knum.txt", "Keng.txt", "Kpunc.txt", "Kdbcs.txt" };
	int q_type;	/* ���Ǿ� ���� : ���/����/���/�λ� �� */

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
	mode.autospac = 4;	// �ڵ� ���� �ɼ� ���� -- 4���� �̻�����... 
	mode.jomi_var = 0;	// ����/������ ���� ���� ���� 

	while (1) {
		if (!get_next_input(fpin, line, inputType)) break;
		n = query_processing(line, nounlist, josaflag, separator, &mode, &q_type);

		count[q_type]++;
//		fprintf(fp[q_type], "%s\t: %s\n", line, nounlist);
		fprintf(stdout, "%s\t: %s\n", line, nounlist);
	}

//	put_statistic_data(count);	// ���Ǿ� ������ ���� ��� ���
//	for (i=0; i < N_FILES; i++) fclose(fp[i]);

	close_HAM_index(&mode);	/* HAM is not used any more */
	return 0;
}
