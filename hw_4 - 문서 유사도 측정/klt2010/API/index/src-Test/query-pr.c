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
*/
#include "../header/ham-ndx.h"
#include "../header/ham-api.h"

#include "../header/kma-api.h"
#include "../header/asp-api.h"
#include "../header/cnoun-api.h"

#define MAXSIZ	1024

extern PREFIX int acc_noun_dic_KSC(char *word);

/* Remove blanks and count the number of blanks */
int remove_blanks(char *s, char *t)
{
	int nblanks=0;

	while (*s) {
		if (*s == ' ') {
			if (*(s+1) != ' ')
				nblanks++;
			else
				while (*(s+1) == ' ') memmove(s, s+1, strlen(s));	// ���ӵ� blank
		} else *t++ = *s;
		s++;
	}

	if (*(t-1) == '\n') *(t-1) = 0;
	else *t = 0;

	return nblanks;
}

/*
	����� ���Ǿ� ���� --> automatic word spacing
*/
int query_proc_autospac(char line[], char line2[], int flag_asp_nonblanks)
{
	int nblanks;

	nblanks = remove_blanks(line, line2);
	if ((unsigned)nblanks >= strlen(line2) / 2 - 1) {
		strcpy(line, line2);	/* 1������ �� ���Ǿ� -> ���� ���� */
		nblanks = -nblanks;
	}
	if (nblanks < 0)
		adjust_word_spacing_nsyl((unsigned char *)line, (unsigned char *)line2, MAXSIZ, 4);
	else if (nblanks == 0 && flag_asp_nonblanks)
		adjust_word_spacing_nsyl((unsigned char *)line, (unsigned char *)line2, MAXSIZ, 4);
	else strcpy(line2, line);

	return nblanks;
}

/* 'word'�� ������/���� �� �ƽ�Ű�ڵ尡 ���ԵǾ����� �˻� */
int is_exist_SBCS(char *word)
{
	int i, n=strlen(word);

	for (i=0; i < n; i++) {
		if (word[i] & 0x80) i++;
		else return 1;
	}

	return 0;
}

/*
	���ո�� 'word'�� ����� ����
		- separator -- ���ص� ������ ������ ' ', '_', '+', '\t', '\0' ��
*/
int cnoun2nounlist(char word[], char nounlist[], char separator)
{
	int n, n2=100, cnndic, puncflag;	/* 2nd �����ĺ� ���� index */
	int flag_nonHangul;

	flag_nonHangul = is_exist_SBCS(word);

	if (flag_nonHangul)
		n = genNounCompKSC_alnum((unsigned char *)word, (unsigned char *)nounlist, separator, &cnndic, &puncflag);
	else	/* n2 -- 2nd ���� �ĺ� ���� index */
		n = genNounCompKSC((unsigned char *)word, (unsigned char *)nounlist, &n2, separator, &cnndic);

	if (!n) { strcpy(nounlist, word); n = 1; }	// ���� �� �� ���� �������

	return ((n < n2) ? n : n2);
}

/*
	NN : ���ո�� -- ������ ���ϵ� ��������θ� ����
	NN1: ���ո�� -- ������ ���ϵ��� ���� ������� ����
	NN2: ������ ���ϵ��� ���� ��簡 ���Ե� ������ ����
	NN3: 3���� �̵�Ͼ�
*/
enum QUERY_TYPE { NN, NN1, NN2, NN3, NN4, JOSA, NSFX, ZZZ, VERB };

int query_type(HAM_PRESULT pr, int *q_type)
{
	if (*q_type < JOSA && pr->josa[0])
		*q_type = JOSA;
	if (*q_type < NSFX && pr->nsfx)
		*q_type = NSFX;
	if (*q_type < ZZZ && pr->pos == 'Z')
		*q_type = ZZZ;
	if (*q_type < VERB && pr->pos == 'V')
		*q_type = VERB;

	return *q_type;
}

/*
	����� ���Ǿ ���վ� ����, �ڵ� ���⿡ ���� query list ����
		- query -- ����ڰ� �Է��� ���Ǿ�
		- flag  -- 0: ���� ����, 1: ���� ����
		- separator -- ���ص� ������ ������ ' ', '_', '+', '\t', '\0' ��
		- query_type -- ���Ǿ� ����
		- return value -- query list�� ������ ���Ǿ� ����
*/
int query_proc(char *query, char *nounlist, int *q_type, int josaflag, char separator, int nblanks, HAM_RUNMODE *mode)
{
	int i, j, k, n;
	int nq=0, leng;
	char cnnlist[512], *p=nounlist, *q;

	HAM_MORES *hamout1;	/* frame-based morph. analysis output */
	HAM_WORD *pw; HAM_RESULT *pr;

	hamout1 = morph_anal((unsigned char *)query, NULL, mode);

	*q_type = NN;
	for (i=k=0; i < hamout1->nword; i++) {
		pw = hamout1->word + i;
		pr = pw->result + pw->gr[1];

		*q_type = query_type(pr, q_type);

		if (pr->patn < 10 && pw->retcode != 'S')	/* ü�� �м� && �̵�Ͼ� ���� */
			n = cnoun2nounlist((char *)pr->stem, cnnlist, '\0');	/* ���ո�� ���� */
		else if (pr->patn == 31)	/*HAM_PTN_NVM*/
			n = cnoun2nounlist((char *)pw->phon, cnnlist, '\0');	/* ���ո�� ���� */
		else n = 0;

		if (n >= 2) {	/* ���ո�� ���� ��� ���� */
			for (j=0, q=cnnlist; j < n; j++) {
				strcpy(p+k, q); k += strlen(q);

				if (*q_type == NN && (*q & 0x80) && (strlen(q) < 4 || !acc_noun_dic_KSC(q))) {
					*q_type = NN1;	/* 1���� or �̵�� ������� ���� */

/*					if (strlen(q) < 4)
						p[k++] = '*';	// 1���� ��� ǥ��
					else p[k++] = '#';	// �̵�� ������� ǥ��
*/
				}

				p[k++] = separator; nq++;
				q += strlen(q) + 1;
			}

			if (josaflag && pr->patn == 2) {	/* ���ո��+���� ������ '����' �߰� */
				strcpy(p+k, (char *)pr->josa); k += strlen((char *)pr->josa);
				p[k++] = separator; nq++;
			}

			if (pr->patn != 2 && pr->patn != 31 &&
				strlen((char *)pr->stem) != strlen((char *)pw->phon)) {
				j = strlen((char *)pr->stem);
				strcpy(p+k-1, (char *)pw->phon + j);	/* ��� �̿��� �κ��� �߰�: '����Ʈ���̾�' */
				k += strlen((char *)pw->phon) - j;
				p[k-1] = separator;
			}
		} else {	/* ���Ͼ� �м� ��� */
			if (hamout1->nword == 1 && !pr->dinf)
				*q_type = NN3;	/* �̵�Ͼ� -- 1 term & 3���� ���� */

			if (pr->patn == 2) {
				if (hamout1->nword == 1 && !pr->dinf &&
					strlen((char *)pw->phon) == 8 && strlen((char *)pr->josa) == 2 &&
					acc_noun_dic_KSC((char *)pw->phon + 4)) {	/* 4�������� �� 2������ ����� ��� */
					strcpy(p+k, (char *)pw->phon);
					k += 4;
					p[k++] = separator; nq++;
					strcpy(p+k, (char *)pw->phon+4);
					k += 4;
					p[k++] = separator; nq++;
				} else {
					strcpy(p+k, (char *)pr->stem);	/* ���+���� ������ '���' �κ� */
					leng = strlen((char *)pr->stem);
					k += leng;
					if (i < hamout1->nword-1 ||	/* �� ������ ����� �ٿ���: '~��/��' ���� */
						i == hamout1->nword-1 && (strcmp((char *)pr->josa, "��") || strcmp((char *)pr->josa, "��")))
						p[k++] = separator; nq++;

					if (josaflag && pr->patn == 2) {	/* ���+���� ������ '����' �߰� */
						strcpy(p+k, (char *)pw->phon+leng);	/* not pr->josa -- '(��)��' */
						k += strlen((char *)pw->phon+leng);
						p[k++] = separator; nq++;
					}
				}
			} else if (pr->patn == 3 && pr->jomi.eomi != 15) {	/* '�����̶�', '�����ϸ�': '��' Ż�� ���� */
				strcpy(p+k, (char *)pr->stem);
				leng = strlen((char *)pr->stem);
				k += leng;
				p[k++] = separator; nq++;

				strcpy(p+k, (char *)pw->phon+leng);
				k += strlen((char *)pw->phon+leng);
				p[k++] = separator; nq++;
			} else {
				strcpy(p+k, (char *)pw->phon);
				leng = strlen((char *)pw->phon);
				k += leng;

				if (k-4 > 0 && (unsigned char)p[k-4] == 0xB4u && (unsigned char)p[k-3] == 0xC2u) {	/* '��' */
					memmove(p+k-1, p+k-2, strlen(p+k-2)+1);
					p[k-2] = separator; nq++;
					k++;
				}

				if (*q_type == NN && (leng == 2 || !pr->dinf || pr->dinf == 10)) {
					if (leng <= 6 && nblanks > 0 &&// *(query+k-leng-1) == ' ' && 
						(*(query+k) == ' ' || *(query+k) == '\0'))
						;	/* ���� �Է¿� ����ڰ� ���� ���� or �� �� ��� */
					else {
						*q_type = NN2;	/* �̵�Ͼ� -- 1���� or �̵�� ������� */
/*
						if (!(*(pw->phon) & 0x80))
							;
						else if (strlen((char *)pw->phon) == 2)
							p[k++] = '*';	// 1���� ��� ǥ��
						else p[k++] = '#';	// �̵�� ������� ǥ��
*/
					}
				}

				p[k++] = separator; nq++;
			}
		}
	}
	if (p[k-1] == ' ') p[k-1] = '\0'; else p[k] = '\0';

	return nq;
}

int is_Hangul(unsigned char c, unsigned char d)
{
	return ((c & 0x0FFu) >= 0xB0u && (c & 0x0FFu) <= 0xC8u &&
		(d & 0x0FFu) >= 0xA1u && (d & 0x0FFu) <= 0xFEu);
}

int is_JosaEomi(char *s)
{
	char s1[3]={0}, s2[4]={0}, s3[5]={0};

	strncpy(s1, s, 2);
	strncpy(s2, s, 3);
	strncpy(s3, s, 4);

	if (!strcmp(s1, "��") || !strcmp(s1, "��") || !strcmp(s1, "��") ||
		!strcmp(s1, "��") || !strcmp(s1, "��") || !strcmp(s1, "��") || !strcmp(s1, "��") ||
		!strcmp(s1, "��") || !strcmp(s1, "��") ||
		!strcmp(s1, "��") || !strcmp(s1, "��") ||
		!strcmp(s1, "��") || !strcmp(s1, "��") || !strcmp(s1, "��") || !strcmp(s1, "��")
		) return 1;
	if (!strcmp(s2, "�� ") || !strcmp(s2, "�� ") ||
		!strcmp(s2, "�� ") || !strcmp(s2, "�� ") ||
		!strcmp(s2, "�� ") || !strcmp(s2, "�� ") ||
		!strcmp(s2, "�� ") || !strcmp(s2, "�� ")) return 1;

	return 0;
}

/*
	NN : ���ո�� -- ������ ���ϵ� ��������θ� ����
	NN1: ���ո�� -- ������ ���ϵ��� ���� ������� ����
	NN2: ������ ���ϵ��� ���� ��簡 ���Ե� ������ ����
	NN3: 3���� �̵�Ͼ�
*/
enum { /*NN, NN1, NN2, NN3, NN4*/ NJ=5, NUM, ENG, PUNC, DBCS };

int word_type(char *s)
{
	int eng, num, punc, dbcs, joem;	// flag

	eng=num=punc=dbcs=joem = 0;
	do {
		if (*s & 0x80) {
			if (!is_Hangul((unsigned char)*s, (unsigned char)*(s+1)))
				dbcs = DBCS;
//			else if (is_JosaEomi(s))
//				joem = NJ;
			s += 2;
		} else {
			if (isspace(*s)) ;
			else if (isalpha(*s)) eng = ENG;	// english
			else if (isdigit(*s)) num = NUM;	// ����
			else if (ispunct(*s)) punc= PUNC;
			s++;
		}
	} while (*s);

	if (dbcs) return dbcs;
	else if (punc) return punc;
	else if (num) return num;
	else if (eng) return eng;
//	else if (joem) return joem;
	else return NN;
}

int query_processing(char *query, char *nounlist, int josaflag, char separator, HAM_RUNMODE *mode, int *q_type)
{
	int i, n, n2, nblanks, q_type2;
	char query2[MAXSIZ], nounlist2[MAXSIZ];

	i = word_type(query);

	nblanks = query_proc_autospac(query, query2, 0);	// 0: �ڵ� ���� ����
	n = query_proc(query, nounlist, q_type, josaflag, separator, nblanks, mode);

	if (/*i == NN &&*/ *q_type == NN)
		;	/* ���ո�� ���ذ�� ���� ���� ����θ� ���� */
	else {	/* ������� ��쿡 �ڵ� ���� ������ ���� ���� */
		/*nblanks =*/ query_proc_autospac(query, query2, 1);	// 1: �ڵ� ����
		n2 = query_proc(query2, nounlist2, &q_type2, josaflag, separator, nblanks, mode);
		if (*q_type != NN2) {
			strncpy(nounlist, nounlist2, MAXSIZ);
			n = n2;
			*q_type = q_type2;
		}
	}

	if (i == NN && *q_type)	/* NNN �̿��� ���Ǿ� ����: ����/���̻�/���/�λ� �� */
		*q_type = (*q_type >= NJ) ? NJ : *q_type;
	else *q_type = i;	// NN -> NN, NN1, NN2, NN3, NJ �� ����

	if (*q_type == NN3 && (n > 1 || strlen(query) >= 8)) {
		*q_type = NN4;	/* NN3 ������ NN4�� �и� */
		n = cnoun2nounlist(query, nounlist, separator);	/* ���ո�� ���� */

/* �Ʒ� �ڵ带 �� '���ո�� ����'�� ����Ͽ���!!!
		p = nounlist + strlen(nounlist) - 3;
		if (!strcmp(p, " ��") || !strcmp(p, " ��") || !strcmp(p, " ��") ||
			!strcmp(p, " ��") ||
			!strcmp(p, " ��") || !strcmp(p, " ��") || !strcmp(p, " ��") ||
			!strcmp(p, " ��") || !strcmp(p, " ��") || !strcmp(p, " ��") ||
			!strcmp(p, " ��") || !strcmp(p, " ��") || !strcmp(p, " ��") ||
			!strcmp(p, " ��") || !strcmp(p, " ��") || !strcmp(p, " ��") ||
			!strcmp(p, " ��") || !strcmp(p, " ��") || !strcmp(p, " ��"))
			memmove(p, p+1, 4);	// blank ����
		else if (!strcmp(p-2, " �̱�") || !strcmp(p-2, " ����") || !strcmp(p-2, " �߱�"))
			memmove(p-2, p-1, 6);	// blank ����
*/
	}

	return n;
}