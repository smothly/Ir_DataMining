#include "../header/ham-kma.h"

void error_analysis(
	HAM_SHORT errcode,
	HAM_PUCHAR word
) {
	switch (errcode) {
	case HANDIC_FILE_OPEN_FAIL:
		fprintf(stderr, "ERROR: HAM dic. -- no such file <hangul.dic>!!!\n");
		break;

	case HAM_RESULT_AREA_FULL:
		fprintf(stderr, "WARNING: [%s] more than maximum(%d) HAM results!!!\n",
			word, MAXRESULT);
		break;

	case LOW_BYTE_HANGUL_MISSED:
		fprintf(stderr, "WARNING: incomplete syllable -- dropped low byte Hangul!!!\n");
		break;

	case HAM_MORE_THAN_MAXWORDS:
		fprintf(stderr, "WARNING: more than maximum(%d) words in a sentence (%s ...)!!!\n",
		MAXWORDS, word);
		break;
	case HAM_WORD_SIZE_TOO_LONG:
		fprintf(stderr, "WARNING: [%s] maximum(%d) word size overflow!!!\n",
			word, WORDSIZE-1);
		break;
	case HAM_STEM_SIZE_TOO_LONG:
		fprintf(stderr, "WARNING: [%s] maximum(%d) stem size overflow!!!\n",
			word, STEMSIZE-1);
		break;
	case HAM_INVALID_INPUT_CHAR:
		fprintf(stderr, "WARNING: control character(s) are found in input!!!\n");
		break;

	case HAM_SENT_SIZE_TOO_LONG:	/* not used yet */
		fprintf(stderr, "WARNING: maximum(%d) sentence size overflow!!!\n", SENTSIZE);
		break;

	case USERDIC_FILE_OPEN_FAIL:
		fprintf(stderr, "ERROR: user dic -- no such file <hangul.usr>!!!\n");
		break;
	case USERDIC_SORTING_ERROR:
		fprintf(stderr, "ERROR: user dic -- sorting order violation for [%s]!!!\n", word);
		break;
	case USERDIC_TOO_MANY_WORDS:
		fprintf(stderr, "WARNING: user dic -- more than %d words!!!\n", MAXUSERWORDS);
		break;
	case USERDIC_AREA_OVERFLOW:
		fprintf(stderr, "ERROR: user dic -- loading area overflow!!!\n");
		break;
	case USERDIC_DIC_POS_WRONG:
		fprintf(stderr, "WARNING: user dic -- invalid POS string of [%s]!!!\n", word);
		break;

	case RMADIC_FILE_OPEN_FAIL:
		fprintf(stderr, "ERROR: RMA dic -- no such file <hangul.rma>!!!\n");
		break;
	case RMADIC_SORTING_ERROR:
		fprintf(stderr, "ERROR: RMA dic -- sorting order violation for [%s]!!!\n", word);
		break;
	case RMADIC_TOO_MANY_WORDS:
		fprintf(stderr, "WARNING: RMA dic -- more than %d words!!!\n", MAXUSERWORDS);
		break;
	case RMADIC_AREA_OVERFLOW:
		fprintf(stderr, "ERROR: RMA dic -- loading area overflow!!!\n");
		break;
	case RMADIC_NSFX_CONV_FAIL:
		fprintf(stderr, "WARNING: RMA dic -- invalid tag for noun suffix [%s]!!!\n", word);
		break;
	case RMADIC_VSFX_CONV_FAIL:
		fprintf(stderr, "WARNING: RMA dic -- invalid tag for verb suffix [%s]!!!\n", word);
		break;
	case RMADIC_POMI_CONV_FAIL:
		fprintf(stderr, "WARNING: RMA dic -- invalid format for p-Eomi [%s]!!!\n", word);
		break;
	case RMADIC_STEM_DPOS_FAIL:
		fprintf(stderr, "WARNING: RMA dic -- invalid POS tag for [%s]!!!\n", word);
		break;
	case RMADIC_POS_TAG_WRONG:
		fprintf(stderr, "WARNING: RMA dic -- invalid POS tag for Josa/Eomi/... [%s]!!!\n", word);
		break;

	case STOPDIC_FILE_OPEN_FAIL:
		fprintf(stderr, "ERROR: stop dic -- no such file <hangul.top>!!!\n");
		break;
	case STOPDIC_SORTING_ERROR:
		fprintf(stderr, "WARNING: stop dic -- sorting order violation [%s]!!!\n", word);
		break;
	case STOPDIC_TOO_MANY_WORDS:
		fprintf(stderr, "WARNING: stop dic -- more than %d words!!!\n", MAXSTOPWORDS);
		break;
	case STOPDIC_AREA_OVERFLOW:
		fprintf(stderr, "ERROR: stop dic -- loading area overflow!!!\n");
		break;
	}
}
/*--------------------------- end of err-code.c -----------------------*/
