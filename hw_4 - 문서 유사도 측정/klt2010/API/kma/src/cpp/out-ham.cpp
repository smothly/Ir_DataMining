/*
	Output test for HAM analysis result.

	Written-by: Kang, Seung-Shik	1997. 7. 20.
*/

/*
	<<< 형태소 분석 결과 해석 방법 ---> 파일 'header/hamout1.h' 참조 >>>

	입력 단어에서 분리된 형태소들을 순서대로 출력하는 방법은 아래와 같다.
	어근은 항상 출력해야 하지만, 기타 형태소들은 분리된 경우에만 출력한다.

	1. 어근과 어근의 품사 출력

		형태소 분석 성공여부는 구조체의 dinf항목을 보면 된다.
		dinf값은 파일 'dicpos.h'에 있는 배열 dicpos[]의 index이며,
		분석사전에 수록된 형태소의 품사정보를 그대로 전달한다.

		dinf =  0 : 어근이 분석사전에 존재하지 않음 --> 분석실패 & 추정
		dinf = 10 : 어근이 분석사전에 존재하지 않으나,
			어근이 '복합명사' 또는 '7-bit 문자'를 포함한 경우

	2. 체언접미사 ('들/등/...') 출력

		항목 nsfx가 0이 아니면 체언접미사가 분리된 경우이다.
		nsfx값은 파일 'hdic2/sfx-v.h'에 있는 nsfdic[hcode]의 index이며,
		nsfdic[hcode][nsfx]는 분리된 접미사의 완성형 string이다.

	3. 서술격조사 '이'와 용언화 접미사('하-/되-/시키-/...') 출력 

		항목 vsfx가 0이 아니면 서술격 조사 or 용언화접미사가 분리된 경우이다.
		vsfx값은 파일 'hdic2/sfx-v.h'에 있는 vsfdic[hcode][]의 index이며,
		vsfdic[hcode][vsfx]는 분리된 접미사의 완성형 string이다.

		<주의> vsfx = 1이면, 서술격 조사 '이'가 분리되었으므로 품사를
		'서술격조사'로 출력한다. 그런데 이 때 jomi.preWi값이 1이면,
		'학교+에서부터+이+다'와 같이 조사 '에서부터'가 서술격조사 앞에
		오므로 조사를 먼저 출력한 후에 서술격조사를 출력한다.

	4. 보조용언 출력

		항목 xverb[0] != '\0'이면, 보조용언이 분리되었음을 의미한다.
		이 경우에 보조용언 앞에 오는 어미('아/어', '고', '게')를 먼저
		출력한 후에 보조용언을 출력한다. 보조용언의 유형은 항목
		jomi.xomitype에 저장되어 있고 보조용언 string은 'hdic2/xomi.h'에
		있는 xomidic[hcode][]이다. 따라서 xomidic[hcode][jomi.xomitype]을 출력한다.

	5. 선어말어미 '시/았/었/겠' 출력

		항목 pomi != 0이면 선어말어미가 분리된 것이며, pomi값은
		'hdic2/pomi.h'의 pomidic[]의 index이다. 따라서 pomidic[pomi]를
		출력하면 된다.

	6. 어말어미 출력

		항목 eomi[0] != '\0'이면, 어말어미가 분리되었음을 의미한다.

	7. 조사 출력

		항목 josa[0] != '\0'이면, 조사가 분리되었음을 의미한다.

	<참고1> 조사, 어말어미, 선어말어미는 변이체에 대하여 대표 조사/어미를
	출력하고 있다. 다만, 아래 2가지 경우는 원문에 나타난 형태로 출력된다.

	(1) 조사에서 '에게/께/게/한테'로 시작되는 것
		대표조사 '에게'로 출력하려면 jomi.josaAgBg값을 참조한다.
	(2) 어미의 경우에 '라고/으라고'와 같이 조음소 '으'가 탈락된 것
		대표어미로 출력하려면 '으'로 시작되는 어미들을 '으'를 제거한다.
		그 이유는 '으' 탈락 어미에서 '으'를 복원하기가 어렵기 때문이다.

	<참고2> 대표 조사/어미가 아니라 원문에 나타난 대로 출력하고자 할
	경우에는 jomi항목에 수록된 각 값들을 참조하여 복원할 수 있다.

	<참고3> 이 분석결과의 좋은 점
	조사/어미/보조용언 등 특정 형태소가 분리되었는지를 알고자 할 때
	그 항목만 검사하면 되므로 매우 편리함.
*/
#include "../header/ham-kma.h"
#include "../header/tag-etri.h"

#ifdef WINDOWS_DLL
#include "..\hdic2\dicpos.h"
#include "..\hdic2\pomi.h"
#include "..\hdic2\xomi.h"
#include "..\hdic2\sfx-n.h"
#include "..\hdic2\sfx-v.h"
#else
extern char *dicpos[];	/* POS info. in Hangul dic */
extern HAM_UCHAR nsfdic[2][N_NSFX][NSFXSIZE];	/* noun suffix list */
extern HAM_UCHAR vsfdic[2][N_VSFX][VSFXSIZE];	/* verb suffix list */
extern HAM_UCHAR pomidic[2][N_POMI][POMISIZE];	/* p-Eomi list */
extern HAM_UCHAR xomidic[2][N_XOMI][XOMISIZE];	/* Eomi list preceded by x-verb */
#endif

extern void error_analysis(	/* src/err-code.c */
	HAM_SHORT errcode,
	HAM_PUCHAR word);

/*
	Convert word pattern to a visible string.
*/
char *wordpatn(
	HAM_UCHAR patn
) {
	switch (patn) {
	case HAM_PTN_N: return "N";
	case HAM_PTN_NJ: return "NJ";
	case HAM_PTN_NSM: return "NSM";
	case HAM_PTN_NSMJ: return "NSMJ";
	case HAM_PTN_NSMXM: return "NSMXM";
	case HAM_PTN_NJCM: return "NJCM";
	case HAM_PTN_VM: return "VM";
	case HAM_PTN_VMJ: return "VMJ";
	case HAM_PTN_VMCM: return "VMCM";
	case HAM_PTN_VMXM: return "VMXM";
	case HAM_PTN_VMXMJ: return "VMXMJ";
	case HAM_PTN_AID: return "AID";
	case HAM_PTN_ADVJ: return "ADVJ";
	case HAM_PTN_NVM: return "NVM";
	case HAM_PTN_ZZZ: return "ZZZ";
	default: return "ERROR";
	}
}

void echo_input_word(
	FILE *fp,	/* output file */
	HAM_PUCHAR word,	/* input word */
	HAM_SHORT rtcode,	/* HAM return code */
	HAM_PRUNMODE mode
) {
	if (mode->outmode == OUTMODE_ALL) {
		if (rtcode == HAM_RC_OK)
			fprintf(fp, "OK: ");
		else if (rtcode == HAM_RC_GUESS)
			fprintf(fp, "GS: ");
		else fprintf(fp, "FL: ");
	}
	fprintf(fp, "%s\n", word);
}

/*
	print morpheme delimiter: tab, blank or plus
*/
void put_tab_or_plus_mark(
	FILE *fp,	/* output file */
	HAM_SHORT flag,	/* 0: 1st morph, 1: others */
	HAM_UCHAR plusflag
) {
	if (!flag)
		fprintf(fp, "\t");	/* stem: 1st morph */
	else {	/* delimiter ' ' or ' + ' */
		if (plusflag)
			fprintf(fp, " + ");
		else fprintf(fp, " ");
	}
}

/*
	print-out <pos, morpheme> pair.
	one of 3 styles:
		1. morph1/pos1 morph2/pos2 ...
		2. (pos1 morph1) + (pos2 morph2) ...
		3. (pos1 "morph1") + (pos2 "morph2") ...
*/
void put_morph(
	FILE *fp,	/* output file */
	HAM_SHORT flag,	/* 0: 1st morpheme, 1: others */
	HAM_PUCHAR morph,	/* morpheme string */
	HAM_UCHAR pos,	/* part-of-speech */
	HAM_PRUNMODE mode
) {
	put_tab_or_plus_mark(fp, flag, mode->outstyle.plus);

	switch (mode->outstyle.form) {
	case OUTSTYLE_NORMAL:
		fprintf(fp, "(%c \"%s\")", pos, morph);
		break;
	case OUTSTYLE_SLASH:	/* tagging style */
		fprintf(fp, "%s/%c", morph, pos);
		break;
	case OUTSTYLE_PAREN:
		fprintf(fp, "(%c %s)", pos, morph);
		break;
	default:
		fprintf(fp, "(%c \"%s\")", pos, morph);
		break;
	}
}

void put_stem(
	FILE *fp,	/* output file */
	HAM_PRESULT pr,	/* morph. analysis result */
	HAM_PRUNMODE mode
) {
	put_morph(fp, 0, pr->stem, pr->pos, mode);
	if (mode->outstyle.info)	/* more info. is added */
		fprintf(fp, "<%1s:%d>", dicpos[pr->dinf], pr->score);
	if (pr->patn == HAM_PTN_NJ && pr->pos2 == HAM_POS_PROPER)
		put_morph(fp, 1, (HAM_PUCHAR) "이", HAM_POS_SFX_N, mode);
}

#define PRINT_STEM \
	if (mode->outmode == OUTMODE_ALL) { /* 모든 분석후보 출력시에만 */ \
		if (pr->dinf) fprintf(fp, "#"); /* 분석 성공 표시 */ \
		fprintf(fp, "%s", wordpatn(pr->patn)); /* 단어 유형 출력 */ \
	} put_stem(fp, pr, mode)

#define PRINT_NOUN_SFX \
	put_morph(fp, 1, nsfdic[mode->hcode_out][pr->nsfx], HAM_POS_SFX_N, mode)

#define PRINT_VERBAL_SFX \
	if (pr->vsfx == 1 || pr->vsfx == 14) {	/* 서술격조사 '이' */ \
		if (pr->jomi.josa == JOSA_VAR_preWi || pr->jomi.josa == JOSA_VAR_preWi2) \
			/* 서술격조사 앞에 오는 조사 : '[에서][부터]' + '이' + ... */ \
			put_morph(fp, 1, pr->josa, HAM_POS_JOSA, mode); \
		put_morph(fp, 1, vsfdic[mode->hcode_out][pr->vsfx], HAM_POS_COPULA, mode); \
		if (mode->outstyle.info && pr->jomi.eomi == EOMI_VAR_Wi_tal)	/* '이' 탈락 */ \
			fprintf(fp, "<%d>", pr->jomi.eomi == EOMI_VAR_Wi_tal); \
	} else put_morph(fp, 1, vsfdic[mode->hcode_out][pr->vsfx], HAM_POS_SFX_V, mode)

#define PRINT_AUXVERB {	/* ... + '어/게/고' + 보조용언 + ... */ \
	if (pr->patn == HAM_PTN_NVM) { \
		put_morph(fp, 1, pr->xverb, HAM_POS_VJXV, mode); \
		if (mode->outstyle.info)	/* add more info */ \
			fprintf(fp, "<%1s>", dicpos[pr->dinf]); \
	} else { \
		put_morph(fp, 1, xomidic[mode->hcode_out][pr->jomi.xomitype], HAM_POS_EOMI, mode); \
		if (mode->outstyle.info && pr->jomi.xomi)	/* 어미 변이체 정보 */ \
			fprintf(fp, "<%d>", pr->jomi.xomi); \
		put_morph(fp, 1, pr->xverb, HAM_POS_XVERB, mode); \
	} \
}

#define PRINT_POMI { \
	put_morph(fp, 1, pomidic[mode->hcode_out][pr->pomi], HAM_POS_PEOMI, mode); \
	if (mode->outstyle.info && pr->jomi.pomi)	/* 선어말어미 변이체 정보 */ \
		fprintf(fp, "<%d>", pr->jomi.pomi); \
}

#define PRINT_EOMI { \
	put_morph(fp, 1, pr->eomi, HAM_POS_EOMI, mode); \
	if (mode->outstyle.info && pr->jomi.eomi)	/* 어미 변이체 정보 */ \
		fprintf(fp, "<%d>", pr->jomi.eomi); \
}

#define PRINT_JOSA { \
	put_morph(fp, 1, pr->josa, HAM_POS_JOSA, mode); \
	if (mode->outstyle.info) {	/* more info. is added */ \
		if (pr->jomi.josa)	/* 조사 변이체 정보 */ \
			fprintf(fp, "<%d>", pr->jomi.josa); \
		if (pr->jomi.josaAgBg)	/* '에게/께/한테' 구분 */ \
			fprintf(fp, "<%d>", pr->jomi.josaAgBg); \
	} \
}

/*
	print-out one morph. analed result.
*/
void put_morph_result(
	FILE *fp,	/* output file */
	HAM_PRESULT pr,	/* morph. analysis result */
	HAM_PRUNMODE mode
) {
	PRINT_STEM;	/* 어근 출력 */

	if (pr->nsfx) PRINT_NOUN_SFX;	/* 명사접미사 '들/등/..' */
	if (pr->vsfx) PRINT_VERBAL_SFX;	/* 용언화접미사 '이/하/되/시키/..' */
	if (pr->xverb[0]) PRINT_AUXVERB;	/* 보조용언 '가/보/..' */

	if (pr->pomi) PRINT_POMI;	/* 선어말어미 '시/었/었/겠' */
	if (pr->eomi[0]) PRINT_EOMI;	/* 어말어미 */
				
	if (pr->josa[0] && pr->jomi.josa != JOSA_VAR_preWi
		&& pr->jomi.josa != JOSA_VAR_preWi2)
		PRINT_JOSA;	/* 조사 : 서술격조사 앞에 오는 것 제외 */

	fprintf(fp, "\n");
}

/*
	Output test for HAM internal structure.
	Print-out internal HAM results for one sentence.
	If (mode->outmode != OUTMODE_ALL) then most plausible
		candidates are printed. ---> strongly recommended!!!
	mode->outmode == OUTMODE_ALL is used only for debugging.
*/
void hamout_HAM(
	FILE *fp,	/* output file */
	HAM_PMORES p,	/* HAM result for input sentence */
	HAM_PRUNMODE mode
) {
	HAM_SHORT i, j, n;
	HAM_PWORD pw;
	HAM_PRESULT pr;

	if (mode->echosent)
		fprintf(fp, "[[%s]]\n", p->phon);	/* echo input sentence */
	if (mode->err_code)
		error_analysis(mode->err_code, mode->err_word);	/* error message */

	for (i = 0; i < p->nword; i++) {
		pw = (p->word)+i;	/* i-th word in a setence */
		if (mode->echoword)	/* echo input word */
			echo_input_word(fp, pw->phon, pw->retcode, mode);

		if (pw->nresult > MAXRESULT)	/* ERROR: overflow max. HAM results */
			fprintf(stderr, "ERROR: [%s] more than maximum(%d) HAM results!!!\n",
				pw->phon, MAXRESULT);

		n = (mode->outmode == OUTMODE_ALL) ? pw->nresult : (int) pw->gr[0];
		if (mode->outlevel == BEST1_ONLY) n = 1;
		else if (mode->outlevel == BEST2_ONLY) n = (n == 1) ? 1: 2;

		for (j = 0; j < n; j++) {	/* for each HAM result */
			if (mode->outmode == OUTMODE_ALL)
				pr = (pw->result)+j;	/* j-th HAM results for word */
			else pr = (pw->result)+(pw->gr[j+1]);	/* j-best HAM results for word */
			put_morph_result(fp, pr, mode);
		}
	}
}

/*
	형태소 분석결과를 분해된 토큰 스트링으로 변환한다.
	예) "오늘은 날씨가 좋다" --> "오늘_은 날씨_가 좋_다"
*/
int hamout2tokenList(
	HAM_PMORES p,
	char *toklist,		/* 토큰으로 구분된 출력 결과 */
	char tokendelim
) {
	int i, n=0;
	int wordleng, stemleng;
	HAM_PUCHAR word;
	char *out=toklist;
	HAM_PWORD pw;
	HAM_PRESULT pr;

	for (i=0; i < p->nword; i++) {
		pw = (p->word)+i;	/* i-th word in a setence */
		if (pw->nresult > MAXRESULT)	/* ERROR: overflow max. HAM results */
			fprintf(stderr, "ERROR: [%s] more than maximum(%d) HAM results!!!\n",
				pw->phon, MAXRESULT);

		word = pw->phon;
		pr = (pw->result)+(pw->gr[1]);	/* 1st HAM results for word */
		wordleng = strlen((char*) pw->phon);
		stemleng = strlen((char*) pr->stem);
		if (stemleng > wordleng) stemleng = wordleng;

		if (i) *out++ = ' ';
		strncpy((char*) out, (char*) word, stemleng); n++;
		out += stemleng;

		if (wordleng > stemleng) {
			*out++ = tokendelim; *out = 0;
			strcat((char*) out-1, (char*) word+stemleng); n++;
			out += wordleng-stemleng;
		}
	}

	*out = '\0';

	return n;
}
/*------------------------ end of out-ham.c -----------------------*/
