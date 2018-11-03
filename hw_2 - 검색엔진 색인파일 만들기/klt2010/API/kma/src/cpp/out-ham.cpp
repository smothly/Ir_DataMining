/*
	Output test for HAM analysis result.

	Written-by: Kang, Seung-Shik	1997. 7. 20.
*/

/*
	<<< ���¼� �м� ��� �ؼ� ��� ---> ���� 'header/hamout1.h' ���� >>>

	�Է� �ܾ�� �и��� ���¼ҵ��� ������� ����ϴ� ����� �Ʒ��� ����.
	����� �׻� ����ؾ� ������, ��Ÿ ���¼ҵ��� �и��� ��쿡�� ����Ѵ�.

	1. ��ٰ� ����� ǰ�� ���

		���¼� �м� �������δ� ����ü�� dinf�׸��� ���� �ȴ�.
		dinf���� ���� 'dicpos.h'�� �ִ� �迭 dicpos[]�� index�̸�,
		�м������� ���ϵ� ���¼��� ǰ�������� �״�� �����Ѵ�.

		dinf =  0 : ����� �м������� �������� ���� --> �м����� & ����
		dinf = 10 : ����� �м������� �������� ������,
			����� '���ո��' �Ǵ� '7-bit ����'�� ������ ���

	2. ü�����̻� ('��/��/...') ���

		�׸� nsfx�� 0�� �ƴϸ� ü�����̻簡 �и��� ����̴�.
		nsfx���� ���� 'hdic2/sfx-v.h'�� �ִ� nsfdic[hcode]�� index�̸�,
		nsfdic[hcode][nsfx]�� �и��� ���̻��� �ϼ��� string�̴�.

	3. ���������� '��'�� ���ȭ ���̻�('��-/��-/��Ű-/...') ��� 

		�׸� vsfx�� 0�� �ƴϸ� ������ ���� or ���ȭ���̻簡 �и��� ����̴�.
		vsfx���� ���� 'hdic2/sfx-v.h'�� �ִ� vsfdic[hcode][]�� index�̸�,
		vsfdic[hcode][vsfx]�� �и��� ���̻��� �ϼ��� string�̴�.

		<����> vsfx = 1�̸�, ������ ���� '��'�� �и��Ǿ����Ƿ� ǰ�縦
		'����������'�� ����Ѵ�. �׷��� �� �� jomi.preWi���� 1�̸�,
		'�б�+��������+��+��'�� ���� ���� '��������'�� ���������� �տ�
		���Ƿ� ���縦 ���� ����� �Ŀ� ���������縦 ����Ѵ�.

	4. ������� ���

		�׸� xverb[0] != '\0'�̸�, ��������� �и��Ǿ����� �ǹ��Ѵ�.
		�� ��쿡 ������� �տ� ���� ���('��/��', '��', '��')�� ����
		����� �Ŀ� ��������� ����Ѵ�. ��������� ������ �׸�
		jomi.xomitype�� ����Ǿ� �ְ� ������� string�� 'hdic2/xomi.h'��
		�ִ� xomidic[hcode][]�̴�. ���� xomidic[hcode][jomi.xomitype]�� ����Ѵ�.

	5. ������ '��/��/��/��' ���

		�׸� pomi != 0�̸� �����̰� �и��� ���̸�, pomi����
		'hdic2/pomi.h'�� pomidic[]�� index�̴�. ���� pomidic[pomi]��
		����ϸ� �ȴ�.

	6. ���� ���

		�׸� eomi[0] != '\0'�̸�, ���̰� �и��Ǿ����� �ǹ��Ѵ�.

	7. ���� ���

		�׸� josa[0] != '\0'�̸�, ���簡 �и��Ǿ����� �ǹ��Ѵ�.

	<����1> ����, ����, �����̴� ����ü�� ���Ͽ� ��ǥ ����/��̸�
	����ϰ� �ִ�. �ٸ�, �Ʒ� 2���� ���� ������ ��Ÿ�� ���·� ��µȴ�.

	(1) ���翡�� '����/��/��/����'�� ���۵Ǵ� ��
		��ǥ���� '����'�� ����Ϸ��� jomi.josaAgBg���� �����Ѵ�.
	(2) ����� ��쿡 '���/�����'�� ���� ������ '��'�� Ż���� ��
		��ǥ��̷� ����Ϸ��� '��'�� ���۵Ǵ� ��̵��� '��'�� �����Ѵ�.
		�� ������ '��' Ż�� ��̿��� '��'�� �����ϱⰡ ��Ʊ� �����̴�.

	<����2> ��ǥ ����/��̰� �ƴ϶� ������ ��Ÿ�� ��� ����ϰ��� ��
	��쿡�� jomi�׸� ���ϵ� �� ������ �����Ͽ� ������ �� �ִ�.

	<����3> �� �м������ ���� ��
	����/���/������� �� Ư�� ���¼Ұ� �и��Ǿ������� �˰��� �� ��
	�� �׸� �˻��ϸ� �ǹǷ� �ſ� ����.
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
		put_morph(fp, 1, (HAM_PUCHAR) "��", HAM_POS_SFX_N, mode);
}

#define PRINT_STEM \
	if (mode->outmode == OUTMODE_ALL) { /* ��� �м��ĺ� ��½ÿ��� */ \
		if (pr->dinf) fprintf(fp, "#"); /* �м� ���� ǥ�� */ \
		fprintf(fp, "%s", wordpatn(pr->patn)); /* �ܾ� ���� ��� */ \
	} put_stem(fp, pr, mode)

#define PRINT_NOUN_SFX \
	put_morph(fp, 1, nsfdic[mode->hcode_out][pr->nsfx], HAM_POS_SFX_N, mode)

#define PRINT_VERBAL_SFX \
	if (pr->vsfx == 1 || pr->vsfx == 14) {	/* ���������� '��' */ \
		if (pr->jomi.josa == JOSA_VAR_preWi || pr->jomi.josa == JOSA_VAR_preWi2) \
			/* ���������� �տ� ���� ���� : '[����][����]' + '��' + ... */ \
			put_morph(fp, 1, pr->josa, HAM_POS_JOSA, mode); \
		put_morph(fp, 1, vsfdic[mode->hcode_out][pr->vsfx], HAM_POS_COPULA, mode); \
		if (mode->outstyle.info && pr->jomi.eomi == EOMI_VAR_Wi_tal)	/* '��' Ż�� */ \
			fprintf(fp, "<%d>", pr->jomi.eomi == EOMI_VAR_Wi_tal); \
	} else put_morph(fp, 1, vsfdic[mode->hcode_out][pr->vsfx], HAM_POS_SFX_V, mode)

#define PRINT_AUXVERB {	/* ... + '��/��/��' + ������� + ... */ \
	if (pr->patn == HAM_PTN_NVM) { \
		put_morph(fp, 1, pr->xverb, HAM_POS_VJXV, mode); \
		if (mode->outstyle.info)	/* add more info */ \
			fprintf(fp, "<%1s>", dicpos[pr->dinf]); \
	} else { \
		put_morph(fp, 1, xomidic[mode->hcode_out][pr->jomi.xomitype], HAM_POS_EOMI, mode); \
		if (mode->outstyle.info && pr->jomi.xomi)	/* ��� ����ü ���� */ \
			fprintf(fp, "<%d>", pr->jomi.xomi); \
		put_morph(fp, 1, pr->xverb, HAM_POS_XVERB, mode); \
	} \
}

#define PRINT_POMI { \
	put_morph(fp, 1, pomidic[mode->hcode_out][pr->pomi], HAM_POS_PEOMI, mode); \
	if (mode->outstyle.info && pr->jomi.pomi)	/* ������ ����ü ���� */ \
		fprintf(fp, "<%d>", pr->jomi.pomi); \
}

#define PRINT_EOMI { \
	put_morph(fp, 1, pr->eomi, HAM_POS_EOMI, mode); \
	if (mode->outstyle.info && pr->jomi.eomi)	/* ��� ����ü ���� */ \
		fprintf(fp, "<%d>", pr->jomi.eomi); \
}

#define PRINT_JOSA { \
	put_morph(fp, 1, pr->josa, HAM_POS_JOSA, mode); \
	if (mode->outstyle.info) {	/* more info. is added */ \
		if (pr->jomi.josa)	/* ���� ����ü ���� */ \
			fprintf(fp, "<%d>", pr->jomi.josa); \
		if (pr->jomi.josaAgBg)	/* '����/��/����' ���� */ \
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
	PRINT_STEM;	/* ��� ��� */

	if (pr->nsfx) PRINT_NOUN_SFX;	/* ������̻� '��/��/..' */
	if (pr->vsfx) PRINT_VERBAL_SFX;	/* ���ȭ���̻� '��/��/��/��Ű/..' */
	if (pr->xverb[0]) PRINT_AUXVERB;	/* ������� '��/��/..' */

	if (pr->pomi) PRINT_POMI;	/* ������ '��/��/��/��' */
	if (pr->eomi[0]) PRINT_EOMI;	/* ���� */
				
	if (pr->josa[0] && pr->jomi.josa != JOSA_VAR_preWi
		&& pr->jomi.josa != JOSA_VAR_preWi2)
		PRINT_JOSA;	/* ���� : ���������� �տ� ���� �� ���� */

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
	���¼� �м������ ���ص� ��ū ��Ʈ������ ��ȯ�Ѵ�.
	��) "������ ������ ����" --> "����_�� ����_�� ��_��"
*/
int hamout2tokenList(
	HAM_PMORES p,
	char *toklist,		/* ��ū���� ���е� ��� ��� */
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
