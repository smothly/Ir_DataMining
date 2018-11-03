/*
	���ո�� �ν�
		��� + ��� + ... + ��� = ���ո��

	2012�� 7�� ���½� nlpkang@gmail.com
	2015�� 1�� ����
*/

/*--------------------------------------------------------------------------
	File-name	: coran.c
	Description	: Example of calling Korean Morph. Analyser
			  Hangul I/O code is KSC-5601-1987
	Written-by	: Kang, Seung-Shik at Kookmin University  2001. 6.
----------------------------------------------------------------------------*/
#include <ctype.h>
#include <string.h>
#include "../header/ham-kma.h"
//#include "../../index/header/ham-ndx.h"
//#include "../../index/header/kma-api.h"

/*
	You may ignore following external functions.
	They are used for I/O options and testing of HAM library.
*/
extern int get_sent_or_line();	// read a sentence from input file

/*
	Following variable is needed for morph. analysis.
	'HAM_UCHAR' equals to 'unsigned char' ---> 'header/typedefs.h'.
*/
HAM_MORES2 HamOut2;	// optional -- ONLY for token-based morph. analysis output

/*
	Set I/O types: following 3 types are possible.
		1. stdin & stdout
		2. file input & stdout
		3. file input & file output
*/
int set_iofile_ptr(int argc, char *argv[], FILE **fpin, FILE **fpout)
{
	int i=1, nargs=argc;

	if (argc > 1 && argv[1][0] == '-') { i++; nargs--; }

	switch (nargs) {
	case 1:
		if (argc == 1) {
			//usage_coran();	// API ����ȭ�� ���� '����' ��� ����
			return 1;
		}
		break;
	case 2:
		*fpin = fopen(argv[i], "r");
		if (!*fpin) { printf("No such file <%s>\n", argv[i]); return 1; }
		break;
	case 3:
		*fpin = fopen(argv[i], "r");
		if (!*fpin) { printf("No such file <%s>\n", argv[i]); return 1; }
		if (*fpout = fopen(argv[i+1], "r")) {
			fprintf(stderr, "Overwrite output file <%s> ? ", argv[i+1]);
			if (tolower(fgetc(stdin)) != 'y') return 1;
		}
		*fpout = fopen(argv[i+1], "w");
		break;
	default:
		//usage_coran();	// API ����ȭ�� ���� '����' ��� ����
		return 1;
	}

	return 0;
}

// ���� ���
int is_single_noun(HAM_PWORD pw, HAM_PRESULT pr)
{
	return (pr->patn == HAM_PTN_N && !pr->nsfx && !pr->vsfx &&
		!pr->pomi && pr->josa[0] == '\0' && pr->eomi[0] == '\0' &&
		strlen(pw->phon) > 2);
}

int last_noun_index(HAM_PMORES p, int j, int end)
{
	int i, n_end_index=j-1;
	HAM_PWORD pw;
	HAM_PRESULT pr;

	for (i = j; i < end; i++) {	// ���ո�� �� ��ġ
		pw = (p->word)+i;
		pr = (pw->result)+(pw->gr[1]);
		if (is_single_noun(pw, pr))
			n_end_index = i;	// ���ո�� �� ��ġ ����
		else {
			if (pr->pos == HAM_POS_NPXM && strlen(pr->stem) > 2)
				n_end_index = i;
			else n_end_index = i-1;
			break;
		}
	}

	return n_end_index;
}

int set_cnoun_index(HAM_PMORES p, int start, int end, int *n_beg, int *n_end)
{
	int i;
	HAM_PWORD pw;
	HAM_PRESULT pr;

	*n_beg = *n_end = -1;

	for (i=start; i < end; i++) {	// ���ո�� ���� ��ġ
		pw = (p->word)+i;
		pr = (pw->result)+(pw->gr[1]);
		*n_beg = i;	// ���� ��ġ �ε���
		if (is_single_noun(pw, pr)) {
			*n_end = last_noun_index(p, i+1, end);
			return *n_end + 1;	// ���� start ��ġ ����
		} else {
			*n_end = i;
			return *n_end + 1;
		}
	}

	return end + 1;	// ���� start ���� ��ġ
}

/*
	���¼� �м� ����κ��� "�� ���ո��"�� �ν��Ͽ� ���
*/
void cnoun_extraction(FILE *fp, HAM_PMORES out1, HAM_PRUNMODE mode)
{
	int start=0, end=out1->nword;
	int i, n_beg, n_end;
	HAM_PWORD pw;
	HAM_PRESULT pr;
	char *p_word;
	unsigned len;

	while (start < end) {
		start = set_cnoun_index(out1, start, end, &n_beg, &n_end);

		p_word = (out1->word+n_end)->phon;
		pw = (out1->word)+n_end;	// i-th word in a setence
//		pr = ((out1->word+n_end)->result) + (out1->word+n_end)->gr[1];
		pr = (pw->result) + pw->gr[1];
		len = strlen(pr->stem);
		if (n_beg == n_end) {
			if (pr->pos == HAM_POS_NPXM) {
				if (pr->dinf == 1)
					fprintf(fp, "\tN: ");	// �ܾ��� ��� -- ������
				else if (pr->dinf == 0)
					fprintf(fp, "\tK: ");	// �ܾ��� ��� -- �̵�Ͼ�
				else if (pr->dinf == 10 && pw->retcode == HAM_RC_GUESS)	// ����������/�׿��ΰ�/��ΰ���
					fprintf(fp, "\tK: ");	// �ܾ��� ��� -- �̵�Ͼ�
				else if (pr->dinf == 10)
					fprintf(fp, "\t9: ");	// �ܾ��� ��� -- ASCII ���� ����
				else fprintf(fp, "\tn: ");	// �ܾ��� ��� -- ����, ���� ��
				fprintf(fp, "%s\n", pr->stem);

				if (strlen(p_word) > len) {
					fprintf(fp, "\t_: ");	// ���� �������¼�(����)
					fprintf(fp, "%s\n", p_word+len);
				}
			} else {
				fprintf(fp, "\t%c: ", pr->pos);	// ���, �λ�� ��
				fprintf(fp, "%s\n", p_word);
			}
		} else {
			fprintf(fp, "\tC: ");	// �پ��� ���ո��
			for (i=n_beg; i < n_end; i++)
				fprintf(fp, "%s ", (out1->word+i)->phon);
			fprintf(fp, "%s\n", pr->stem);

			if (strlen(p_word) > len) {
				fprintf(fp, "\t_: ");	// ���� �������¼�(����)
				fprintf(fp, "%s\n", p_word+len);
			}
		}
	}
}

/*
	���� �̵�Ͼ�(���?) ��� (pr->dinf == 0 �� ��)
*/
void put_unknown(HAM_PMORES pmr, FILE *fpout_K)
{
	HAM_PRESULT pr;
	int i, j;

	for (i=0; i < pmr->nword; i++) {
		j = pmr->word[i].gr[1];
		pr = pmr->word[i].result + j;
		if (pr->dinf == 0)	// ���� �̵�Ͼ�
			fprintf(fpout_K, "%s\n", pr->stem);
	}
}

extern PREFIX int morph_anal_simple2(char *sent, HAM_PMORES hamout, char *inifile);
HAM_MORES HamOut;

/*---------- INTEGRATION GUIDE of HAM to your system -----------*/
/*                                                              */
/* 0. You should include 'header/ham.h'.                        */
/*                                                              */
/* 1. Following variables are required for HAM                  */
/*       char sent[MAX];  --> one Korean sentence               */
/*       HAM_RUNMODE mode;    --> HAM running options           */
/*       HAM_PMORES  hamout1; --> frame-based result of HAM     */
/*       HAM_MORES2 HamOut2;  --> token-based result of HAM     */
/*    'HamOut2' is needed iff token-based results are required. */
/*                                                              */
/* 2. Call 'open_HAM()' before the first call of morph_anal().  */
/*       It's argument is a diretory path of 'hangul.???'.      */
/*       It initializes global var.s and opens dic. files.      */
/*       Return value --- 0(normal), 1(dic.file open failure)   */
/*                                                              */
/* 3. Call 'morph_anal(sent, &HamOut2, &mode)'.                 */
/*       Morph. analyze words in the sentence and confirm       */
/*       output results by 'hamout_HAM()', hamout_token().      */
/*                                                              */
/* 4. Call 'close_HAM()', if morph_anal() is no nonger used.    */
/*                                                              */
/* Only 3 functions are required for integration to appl. system*/
/*        open_HAM(), morph_anal(), close_HAM()                 */
/*                                                              */
/*--------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	int i, flag;	// temporary variable
	FILE *fpin=stdin, *fpout=stdout;
	FILE *fpout_K;	int flag_unknown=0;

	char *optstr=NULL;	// option string: e.g. "1aC2", "pVc"
	HAM_RUNMODE mode;	// HAM running mode: 'header/runmode.h'

	unsigned char sent[SENTSIZE];	// input sentence
	HAM_PMORES	hamout1=&HamOut;	// frame-based morph. analysis output

	if (set_iofile_ptr(argc, argv, &fpin, &fpout))
		return 0;	// I/O files open failed
	if (argc > 1 && argv[1][0] == '-') {
		optstr = argv[1]+1;
		if (*optstr == 'K') flag_unknown = 1;
	}

	/*=========================== Initialization ===========================*/
	flag = open_HAM(&mode, optstr, "./hdic/KLT2000.ini");
	if (flag == 1) {
		//error_message(mode);
		return 0;
	} else if (flag == 2)
		fprintf(stderr, "WARNING: file not found --- <KLT2000.ini>\n");
	WELCOME(VERSION_NUMBER);	// welcome message: 'header/ham-kma.h'

	if (mode.autospac) load_bigram_data("./hdic/bigram.dic");	// �ڵ� ����

	if (flag_unknown) fpout_K = fopen("unknown.txt", "w");	// �̵�Ͼ� ���

	/*--------------- MORE INTEGRATION GUIDE OF HAM ----------------*/
	/*  There are 2 types of HAM output structure,                  */
	/*     frame-based(HAM internal) and token-based output.        */
	/*  Decide one of them that fits to your applicatoin program.   */
	/*--------------------------------------------------------------*/
	while (fgets(sent, SENTSIZE-1, fpin)) {
//		if (mode.autospac) adjust_word_spacing(sent, 1);	// �ڵ� ����
		if (sent[0] == '\n') continue;

		if (mode.outmode == OUTMODE_HAM || mode.outmode == OUTMODE_ALL) {
			//hamout1 = morph_anal(sent, NULL, &mode);	// default
			i = morph_anal_simple2(sent, &HamOut, "./hdic/KLT2000.ini");	// default
		} else hamout1 = morph_anal(sent, &HamOut2, &mode);	// token-based output

		if (mode.echosent) fprintf(fpout, "[[%s]]\n", sent);	// �Է� ���� or ����
		cnoun_extraction(fpout, hamout1, &mode);

		if (flag_unknown) put_unknown(hamout1, fpout_K);	// �̵�Ͼ� ���
	}

	/*=========================== Finalize KMA ===========================*/
	if (mode.autospac) free_bigram_data();	// �ڵ� ����
	close_HAM();	// HAM is not used any more

	if (fpin != stdin) fclose(fpin);
	if (fpout != stdout) fclose(fpout);
	if (flag_unknown) {	// �̵�Ͼ� ���
		fprintf(stderr, "File <unknown.txt> was created.\n");
		fclose(fpout_K);
	}
	GOODBYE(YEAR);	// good-bye message: 'header/ham-kma.h'

	return 0;
}
/*--------------------------- end of coran.c --------------------------*/
