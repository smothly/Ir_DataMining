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
	<<< ��ū��� ���¼� �м���� �ؼ� ��� : 'header/hamout2.h' ���� >>>

	'token ����'�� HAM�� �м� ������� ���ʿ��� ������ �����ϰ�, �ʿ���
	�׸�鸸 �����Ͽ� 1�� 1�� ��ȯ�߱� ������ HAM ���α����� subset�̴�.

	token ������ ���¼� �м� ����� <ǰ��, ��ū> ������ ������ ���̴�.
	��, �ܾ�� �и��� �� ���¼��� ���� ntoken�� �����ϰ�,
	�� ���¼ҿ� ���� ǰ��� pos[i], ���¼� string�� token[i]�� ����ȴ�.

	�׸� ind�� �Լ� hamout_HAM()���� dinf�� ���� copy�� ���̰�,
	score �׸��� tagging�̳� ��ȣ�� �ذ��� ���� ����� �������� �߰��ߴ�.

	<<����>> HAM ���� ������ token ������ �ٸ� ��

	1. ǰ�� tag : 'header/tag-snu.h' ����

	2. ���¼� �м������ ����ȭ

	�⺻������ HAM ���α��� ��°���� token-based ������ ��ȯ�Ѵ�.
	�׷��� �������� HamMode.xpnd_NV = 1�̸�, ü�� �м������
	���/����/����/������翡 ���� ���� �м������ ����Ѵ�.
	����, ����� ��쿡�� ����/�����/�������� �����Ͽ� ����
	�м������ ���� �����Ѵ�. ��, ��������� ���������� ��������
	�ʰ� ��� ����������� �����Ѵ�.

	3. token�� ����

	�⺻������ HAM�� �и��� ���¼Ҹ� �ϳ��� token���� �����Ѵ�.
	��, HAM ���α���('header/hamout1.h')�� �м������ 1 �� 1�� ��ȯ�Ѵ�.
	�׷��� �Ʒ� ���� ���� �����ڳ� ���ڰ� ���Ե� �ܾ��� ���¼Ҹ�
	1 �� �ٷ� ��ȯ�� �� �ִ�.

		1997�⿡ ---> 1997�� + �� ---> 1997+��+��
		1��2���� ---> 1��2�� + �� ---> 1 + �� + 2 + �� + ��

	init_ham() ȣ����, ���¼� �м��Լ� morph_anal()�� ȣ���ϱ� ����
	�������� HamMode.xpnd = 1�� �ʱ�ȭ���ָ�, �� ���� ����
	�����ڿ� ���ڰ� ���Ե� ��ٿ� ���ؼ� 1 �� �ٷ� ��ȯ�Ѵ�.

	4. HAM ���α����� ���������� ����/����� ����ü�� �����·�
	   ������ �� �ִ� ������ ����.
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
