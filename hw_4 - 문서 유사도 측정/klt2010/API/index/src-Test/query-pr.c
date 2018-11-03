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
				while (*(s+1) == ' ') memmove(s, s+1, strlen(s));	// 연속된 blank
		} else *t++ = *s;
		s++;
	}

	if (*(t-1) == '\n') *(t-1) = 0;
	else *t = 0;

	return nblanks;
}

/*
	사용자 질의어 가공 --> automatic word spacing
*/
int query_proc_autospac(char line[], char line2[], int flag_asp_nonblanks)
{
	int nblanks;

	nblanks = remove_blanks(line, line2);
	if ((unsigned)nblanks >= strlen(line2) / 2 - 1) {
		strcpy(line, line2);	/* 1음절씩 띄어쓴 질의어 -> 공백 제거 */
		nblanks = -nblanks;
	}
	if (nblanks < 0)
		adjust_word_spacing_nsyl((unsigned char *)line, (unsigned char *)line2, MAXSIZ, 4);
	else if (nblanks == 0 && flag_asp_nonblanks)
		adjust_word_spacing_nsyl((unsigned char *)line, (unsigned char *)line2, MAXSIZ, 4);
	else strcpy(line2, line);

	return nblanks;
}

/* 'word'에 영문자/숫자 등 아스키코드가 포함되었는지 검사 */
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
	복합명사 'word'를 명사들로 분해
		- separator -- 분해된 명사들의 구분자 ' ', '_', '+', '\t', '\0' 등
*/
int cnoun2nounlist(char word[], char nounlist[], char separator)
{
	int n, n2=100, cnndic, puncflag;	/* 2nd 분해후보 시작 index */
	int flag_nonHangul;

	flag_nonHangul = is_exist_SBCS(word);

	if (flag_nonHangul)
		n = genNounCompKSC_alnum((unsigned char *)word, (unsigned char *)nounlist, separator, &cnndic, &puncflag);
	else	/* n2 -- 2nd 분해 후보 시작 index */
		n = genNounCompKSC((unsigned char *)word, (unsigned char *)nounlist, &n2, separator, &cnndic);

	if (!n) { strcpy(nounlist, word); n = 1; }	// 분해 안 된 것은 원래대로

	return ((n < n2) ? n : n2);
}

/*
	NN : 복합명사 -- 사전에 수록된 단위명사들로만 구성
	NN1: 복합명사 -- 사전에 수록되지 않은 단위명사 포함
	NN2: 사전에 수록되지 않은 명사가 포함된 어절이 있음
	NN3: 3음절 미등록어
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
	사용자 질의어를 복합어 분해, 자동 띄어쓰기에 의해 query list 생성
		- query -- 사용자가 입력한 질의어
		- flag  -- 0: 조사 제거, 1: 조사 포함
		- separator -- 분해된 명사들의 구분자 ' ', '_', '+', '\t', '\0' 등
		- query_type -- 질의어 유형
		- return value -- query list로 생성된 질의어 개수
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

		if (pr->patn < 10 && pw->retcode != 'S')	/* 체언 분석 && 미등록어 추정 */
			n = cnoun2nounlist((char *)pr->stem, cnnlist, '\0');	/* 복합명사 분해 */
		else if (pr->patn == 31)	/*HAM_PTN_NVM*/
			n = cnoun2nounlist((char *)pw->phon, cnnlist, '\0');	/* 복합명사 분해 */
		else n = 0;

		if (n >= 2) {	/* 복합명사 분해 결과 생성 */
			for (j=0, q=cnnlist; j < n; j++) {
				strcpy(p+k, q); k += strlen(q);

				if (*q_type == NN && (*q & 0x80) && (strlen(q) < 4 || !acc_noun_dic_KSC(q))) {
					*q_type = NN1;	/* 1음절 or 미등록 단위명사 포함 */

/*					if (strlen(q) < 4)
						p[k++] = '*';	// 1음절 명사 표시
					else p[k++] = '#';	// 미등록 단위명사 표시
*/
				}

				p[k++] = separator; nq++;
				q += strlen(q) + 1;
			}

			if (josaflag && pr->patn == 2) {	/* 복합명사+조사 유형의 '조사' 추가 */
				strcpy(p+k, (char *)pr->josa); k += strlen((char *)pr->josa);
				p[k++] = separator; nq++;
			}

			if (pr->patn != 2 && pr->patn != 31 &&
				strlen((char *)pr->stem) != strlen((char *)pw->phon)) {
				j = strlen((char *)pr->stem);
				strcpy(p+k-1, (char *)pw->phon + j);	/* 명사 이외의 부분을 추가: '나이트파이어' */
				k += strlen((char *)pw->phon) - j;
				p[k-1] = separator;
			}
		} else {	/* 단일어 분석 결과 */
			if (hamout1->nword == 1 && !pr->dinf)
				*q_type = NN3;	/* 미등록어 -- 1 term & 3음절 이하 */

			if (pr->patn == 2) {
				if (hamout1->nword == 1 && !pr->dinf &&
					strlen((char *)pw->phon) == 8 && strlen((char *)pr->josa) == 2 &&
					acc_noun_dic_KSC((char *)pw->phon + 4)) {	/* 4음절어의 끝 2음절이 명사인 경우 */
					strcpy(p+k, (char *)pw->phon);
					k += 4;
					p[k++] = separator; nq++;
					strcpy(p+k, (char *)pw->phon+4);
					k += 4;
					p[k++] = separator; nq++;
				} else {
					strcpy(p+k, (char *)pr->stem);	/* 명사+조사 유형의 '명사' 부분 */
					leng = strlen((char *)pr->stem);
					k += leng;
					if (i < hamout1->nword-1 ||	/* 끝 어절의 조사는 붙여씀: '~는/은' 제외 */
						i == hamout1->nword-1 && (strcmp((char *)pr->josa, "은") || strcmp((char *)pr->josa, "는")))
						p[k++] = separator; nq++;

					if (josaflag && pr->patn == 2) {	/* 명사+조사 유형의 '조사' 추가 */
						strcpy(p+k, (char *)pw->phon+leng);	/* not pr->josa -- '(에)서' */
						k += strlen((char *)pw->phon+leng);
						p[k++] = separator; nq++;
					}
				}
			} else if (pr->patn == 3 && pr->jomi.eomi != 15) {	/* '가격이란', '공부하며': '이' 탈락 제외 */
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

				if (k-4 > 0 && (unsigned char)p[k-4] == 0xB4u && (unsigned char)p[k-3] == 0xC2u) {	/* '는' */
					memmove(p+k-1, p+k-2, strlen(p+k-2)+1);
					p[k-2] = separator; nq++;
					k++;
				}

				if (*q_type == NN && (leng == 2 || !pr->dinf || pr->dinf == 10)) {
					if (leng <= 6 && nblanks > 0 &&// *(query+k-leng-1) == ' ' && 
						(*(query+k) == ' ' || *(query+k) == '\0'))
						;	/* 원래 입력에 사용자가 공백 삽입 or 맨 끝 명사 */
					else {
						*q_type = NN2;	/* 미등록어 -- 1음절 or 미등록 단위명사 */
/*
						if (!(*(pw->phon) & 0x80))
							;
						else if (strlen((char *)pw->phon) == 2)
							p[k++] = '*';	// 1음절 명사 표시
						else p[k++] = '#';	// 미등록 단위명사 표시
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

	if (!strcmp(s1, "는") || !strcmp(s1, "를") || !strcmp(s1, "며") ||
		!strcmp(s1, "을") || !strcmp(s1, "에") || !strcmp(s1, "라") || !strcmp(s1, "으") ||
		!strcmp(s1, "와") || !strcmp(s1, "과") ||
		!strcmp(s1, "로") || !strcmp(s1, "의") ||
		!strcmp(s1, "가") || !strcmp(s1, "이") || !strcmp(s1, "도") || !strcmp(s1, "고")
		) return 1;
	if (!strcmp(s2, "의 ") || !strcmp(s2, "은 ") ||
		!strcmp(s2, "이 ") || !strcmp(s2, "가 ") ||
		!strcmp(s2, "고 ") || !strcmp(s2, "지 ") ||
		!strcmp(s2, "도 ") || !strcmp(s2, "만 ")) return 1;

	return 0;
}

/*
	NN : 복합명사 -- 사전에 수록된 단위명사들로만 구성
	NN1: 복합명사 -- 사전에 수록되지 않은 단위명사 포함
	NN2: 사전에 수록되지 않은 명사가 포함된 어절이 있음
	NN3: 3음절 미등록어
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
			else if (isdigit(*s)) num = NUM;	// 숫자
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

	nblanks = query_proc_autospac(query, query2, 0);	// 0: 자동 띄어쓰기 안함
	n = query_proc(query, nounlist, q_type, josaflag, separator, nblanks, mode);

	if (/*i == NN &&*/ *q_type == NN)
		;	/* 복합명사 분해결과 사전 수록 명사들로만 구성 */
	else {	/* 공백없는 경우에 자동 띄어쓰기 적용후 명사들 추출 */
		/*nblanks =*/ query_proc_autospac(query, query2, 1);	// 1: 자동 띄어쓰기
		n2 = query_proc(query2, nounlist2, &q_type2, josaflag, separator, nblanks, mode);
		if (*q_type != NN2) {
			strncpy(nounlist, nounlist2, MAXSIZ);
			n = n2;
			*q_type = q_type2;
		}
	}

	if (i == NN && *q_type)	/* NNN 이외의 질의어 유형: 조사/접미사/용언/부사 등 */
		*q_type = (*q_type >= NJ) ? NJ : *q_type;
	else *q_type = i;	// NN -> NN, NN1, NN2, NN3, NJ 로 구분

	if (*q_type == NN3 && (n > 1 || strlen(query) >= 8)) {
		*q_type = NN4;	/* NN3 유형을 NN4로 분리 */
		n = cnoun2nounlist(query, nounlist, separator);	/* 복합명사 분해 */

/* 아래 코드를 위 '복합명사 분해'로 대신하였음!!!
		p = nounlist + strlen(nounlist) - 3;
		if (!strcmp(p, " 가") || !strcmp(p, " 나") || !strcmp(p, " 와") ||
			!strcmp(p, " 이") ||
			!strcmp(p, " 로") || !strcmp(p, " 에") || !strcmp(p, " 엔") ||
			!strcmp(p, " 기") || !strcmp(p, " 도") || !strcmp(p, " 지") ||
			!strcmp(p, " 야") || !strcmp(p, " 여") || !strcmp(p, " 서") ||
			!strcmp(p, " 인") || !strcmp(p, " 임") || !strcmp(p, " 일") ||
			!strcmp(p, " 한") || !strcmp(p, " 함") || !strcmp(p, " 해"))
			memmove(p, p+1, 4);	// blank 삭제
		else if (!strcmp(p-2, " 이기") || !strcmp(p-2, " 이지") || !strcmp(p-2, " 야기"))
			memmove(p-2, p-1, 6);	// blank 삭제
*/
	}

	return n;
}