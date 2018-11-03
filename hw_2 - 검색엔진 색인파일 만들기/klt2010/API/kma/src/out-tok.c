/*
	Output test for HAM result for token-based structure.

	Two types of output are provided.

		1. S.N.U tagset defined in 'header/tag-snu.h'
		2. KONAN tagset defined in 'header/tag-etri.h'

	Basically, SNU tag is attached as a POS of morpheme.
	For KONAN tagset, tag conversion is needed.

	Written-by: Kang, Seung-Shik	1997. 8. 4.
*/

/*
	<<< 토큰기반 형태소 분석결과 해석 방법 : 'header/hamout2.h' 참조 >>>

	'token 구조'는 HAM의 분석 결과에서 불필요한 내용을 제거하고, 필요한
	항목들만 추출하여 1대 1로 변환했기 때문에 HAM 내부구조의 subset이다.

	token 구조는 형태소 분석 결과를 <품사, 토큰> 쌍으로 저장한 것이다.
	즉, 단어에서 분리된 각 형태소의 수를 ntoken에 저장하고,
	각 형태소에 대해 품사는 pos[i], 형태소 string은 token[i]에 저장된다.

	항목 ind는 함수 hamout_HAM()에서 dinf의 값을 copy한 것이고,
	score 항목은 tagging이나 모호성 해결을 위해 사용할 목적으로 추가했다.

	<<주의>> HAM 내부 구조와 token 구조의 다른 점

	1. 품사 tag : 'header/tag-snu.h' 참조

	2. 형태소 분석결과의 세분화

	기본적으로 HAM 내부구조 출력결과를 token-based 구조로 변환한다.
	그러나 전역변수 HamMode.xpnd_NV = 1이면, 체언 분석결과를
	명사/대명사/수사/의존명사에 대해 각각 분석결과를 출력한다.
	또한, 용언의 경우에도 동사/형용사/보조용언로 구분하여 각각
	분석결과를 따로 생성한다. 단, 보조동사와 보조형용사는 구분하지
	않고 모두 보조용언으로 생성한다.

	3. token의 구분

	기본적으로 HAM이 분리한 형태소를 하나의 token으로 간주한다.
	즉, HAM 내부구조('header/hamout1.h')의 분석결과를 1 대 1로 변환한다.
	그러나 아래 예와 같이 영문자나 숫자가 포함된 단어의 형태소를
	1 대 다로 변환할 수 있다.

		1997년에 ---> 1997년 + 에 ---> 1997+년+에
		1장2절은 ---> 1장2절 + 은 ---> 1 + 장 + 2 + 절 + 은

	init_ham() 호출후, 형태소 분석함수 morph_anal()을 호출하기 전에
	전역변수 HamMode.xpnd = 1로 초기화해주면, 위 예와 같이
	영문자와 숫자가 포함된 어근에 대해서 1 대 다로 변환한다.

	4. HAM 내부구조와 차이점으로 조사/어미의 변이체를 원상태로
	   복원할 수 있는 정보가 없음.
*/
#include "../header/ham-kma.h"
#include "../header/tag-etri.h"

extern void error_analysis(	/* defined at 'err-code.c' */
	HAM_SHORT errcode,
	HAM_PUCHAR word);
extern void put_tab_or_plus_mark(	/* defined at 'out-ham.c' */
	FILE *fp,	/* output file */
	HAM_SHORT flag,	/* 0: 1st morph, 1: others */
	HAM_UCHAR plusflag);	/* morph. delimeter */

extern char *dicpos[NO_POS];	/* included at 'out-ham.c' */

/*
	put dic-info of a stem, Josa/Eomi-variant information.
*/
void put_added_info(fp, pr, i)
FILE *fp;
HAM_PRESULT2 pr;
HAM_SHORT i;
{
	if (!i)	/* stem */
		fprintf(fp, "<%1s:%d>", dicpos[pr->ind], (int) pr->score);
	else {
		if (pr->pos[i] == HTAG_EOMI &&	/* A/O-missing info */
			pr->jomi->eomi && pr->jomi->eomi < 13)
			fprintf(fp, "<%d>", pr->jomi->eomi);
		if (pr->pos[i] == HTAG_POMI)	/* p-Eomi variant info */
			fprintf(fp, "<%d>", pr->jomi->pomi);
		if (pr->pos[i] == HTAG_CORP)	/* Josa 'Wi'-missing info */
			fprintf(fp, "<%d>", pr->jomi->eomi == EOMI_VAR_Wi_tal);
	}
}

/*
	Print-out one token-based result for SNU tagset.
*/
void put_token_result(fp, pr, hammode)
FILE *fp;
HAM_PRESULT2 pr;
HAM_PRUNMODE hammode;
{
	HAM_SHORT i;

	for (i = 0; i < pr->ntoken; i++) {	/* <morph, tag> pairs */
		put_tab_or_plus_mark(fp, i, hammode->outstyle.plus);	/* put '\t', ' ', or " + " */

		switch (hammode->outstyle.form) {
		case OUTSTYLE_SLASH:
			fprintf(fp, "%s/%c", pr->token[i], pr->pos[i]);
			break;
		case OUTSTYLE_PAREN:
			fprintf(fp, "(%c %s)", pr->pos[i], pr->token[i]);
			break;
		default:
			fprintf(fp, "(%c \"%s\")", pr->pos[i], pr->token[i]);
			break;
		}

		if (hammode->outstyle.info) put_added_info(fp, pr, i);
	} fprintf(fp, "\n");
}

/*
	Token-based output test for SNU tagset.
	Print-out HAM results for one sentence : SNU K-J M.T. format.

*/
void hamout_token(fp, p, hammode)
FILE *fp;
HAM_PMORES2 p;
HAM_PRUNMODE hammode;
{
	HAM_SHORT i, j;
	HAM_PWORD2 pw;
	HAM_PRESULT2 pr;

	if (hammode->echosent)	/* echo input sentence */
		fprintf(fp, "[[%s]]\n", p->phon);

	if (hammode->err_code)
		error_analysis(hammode->err_code, hammode->err_word);	/* error message */

	for (i = 0; i < p->nword; i++) {

		pw = (p->word)+i;	/* i-th word in a sentence */
		if (hammode->echoword)
			fprintf(fp, "%s\n", pw->phon);	/* echo input word */

		for (j = 0; j < pw->nresult; j++) {
			pr = (pw->result)+j;	/* j-th HAM result for word */
			put_token_result(fp, pr, hammode);

			if (hammode->outlevel == 1) break;	/* best 1 result */
			else if (j && hammode->outlevel == 2) break;	/* best 2 */
		}
	}
}

/*
	POS tag conversion from SNU tag to KONAN tag.
*/
char *snu2konan(c)
HAM_UCHAR c;
{
	switch (c) {
	case HTAG_NOUN:	return KTAG_NOUN;
	case HTAG_PNOUN:	return KTAG_PNOUN;
	case HTAG_XNOUN:	return KTAG_XNOUN;
	case HTAG_NUMER:	return KTAG_NUMER;
	case HTAG_ASC:	return KTAG_NUMER;
	case HTAG_VERB:	return KTAG_VERB;
	case HTAG_ADJ:	return KTAG_ADJ;
	case HTAG_XVERB:return KTAG_XVERB;
	case HTAG_ADV:	return KTAG_ADV;
	case HTAG_DET:	return KTAG_DET;
	case HTAG_EXCL:	return KTAG_EXCL;
	case HTAG_JOSA:	return KTAG_JOSA;
	case HTAG_CORP:	return KTAG_CORP;
	case HTAG_EOMI:	return KTAG_EOMI;
	case HTAG_POMI:	return KTAG_POMI;
	case HTAG_NSFX:
	case HTAG_VSFX:	return KTAG_SFX;
	case HTAG_PUNC:	return KTAG_PUNC;
	case HTAG_SYMB:	return KTAG_SYMB;
	default: return KTAG_ZZZ;
	}
}

void put_konan_morph(fp, tok, pos, hammode)
FILE *fp;
HAM_PUCHAR tok;
char *pos;
HAM_PRUNMODE hammode;
{
	switch (hammode->outstyle.form) {
	case OUTSTYLE_SLASH:
		fprintf(fp, "%s/%s", tok, pos);
		break;
	case OUTSTYLE_PAREN:
		fprintf(fp, "(%s %s)", pos, tok);
		break;
	default:
		fprintf(fp, "(%s \"%s\")", pos, tok);
		break;
	}
}

/*
	Print-out one result for KONAN tagset.
*/
void put_konan_result(fp, pr, stempos, hammode)
FILE *fp;
HAM_PRESULT2 pr;
char *stempos;
HAM_PRUNMODE hammode;
{
	HAM_SHORT i;

	fprintf(fp, "\t");
	put_konan_morph(fp, pr->token[0], stempos, hammode);
	if (hammode->outstyle.info)
		fprintf(fp, "<%1s:%d>", dicpos[pr->ind], (int) pr->score);

	for (i = 1; i < pr->ntoken; i++) {	/* <morph, tag> pairs */
		put_tab_or_plus_mark(fp, i, hammode->outstyle.plus);	/* put '\t', ' ', or " + " */
		put_konan_morph(fp, pr->token[i], snu2konan(pr->pos[i]), hammode);
		if (hammode->outstyle.info) put_added_info(fp, pr, i);
	} fprintf(fp, "\n");
}

/*
	Token-based output test for ETRI KONAN group tagset.

	It is very similar to 'hamout_token()'.
	So, refer to 'hamout_token()' for detailed information.
	POS tags are defined in a file 'header/tag-etri.h'.
*/
void hamout_konan(fp, p, hammode)
FILE *fp;
HAM_PMORES2 p;
HAM_PRUNMODE hammode;
{
	HAM_SHORT i, j;
	HAM_PWORD2 pw;
	HAM_PRESULT2 pr;

	if (hammode->echosent)	/* echo input sentence */
		fprintf(fp, "[[%s]]\n", p->phon);

	if (hammode->err_code)
		error_analysis(hammode->err_code, hammode->err_word);	/* error message */

	for (i = 0; i < p->nword; i++) {

		pw = (p->word)+i;	/* i-th word in a sentence */
		if (hammode->echoword)
			fprintf(fp, "%s\n", pw->phon);	/* echo input word */

		for (j = 0; j < pw->nresult; j++) {
			pr = (pw->result)+j;	/* j-th HAM result for word */
			put_konan_result(fp, pr, snu2konan(pr->pos[0]), hammode);

			if (hammode->outlevel == 1) break;	/* best 1 result */
			else if (j && hammode->outlevel == 2) break;	/* best 2 */
		}
	}
}
/*------------------------ end of out-tok.c -----------------------*/
