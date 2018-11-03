/*
	Eomi's that precede aux. verb.

		xomidic[0][i][j] : KSSM Hangul code
		xomidic[1][i][j] : KS C 5601-1987 Hangul code

		N_XOMI = 12, XOMISIZE = 3 --> defined at 'header/hdics.h'
*/

HAM_UCHAR xomidic[2][N_XOMI][XOMISIZE] = {
  {
	/*
		3+1 KSSM Eomi's that precede aux. verb.
	*/
	{ 0xb4, 0xe1, '\0' },	/* 0:'어' */
	{ 0x89, 0xa1, '\0' },	/* 1:'고' */
	{ 0x89, 0x41, '\0' },	/* 2:'게' */

	/* followings are added for Josa/Eomi variants only */
	{ 0xb4, 0x61, '\0' },	/* 3:'아' */
	{ 0xb4, 0xa1, '\0' },	/* 4:'여' */
	{ 0x84, 0x61, '\0' },	/* 5:'ㅏ' */
	{ 0x84, 0xe1, '\0' },	/* 6:'ㅓ' */
	{ 0x85, 0x61, '\0' },	/* 7:'ㅕ' */
	{ 0x84, 0x81, '\0' },	/* 8:'ㅐ' */
	{ 0x85, 0xe1, '\0' },	/* 9:'ㅙ' */
	{ 'n', 'u', '\0' },	/* 10:'아/어' 탈락 */

	{ 0xbb, 0xa1, '\0' }	/* 11:'지' */
  },
  {
	/*
		3+1 KSC 5601 Eomi's that precede aux. verb.
	*/
	{ 0xbe, 0xee, '\0' },	/* '어' */
	{ 0xb0, 0xed, '\0' },	/* '고' */
	{ 0xb0, 0xd4, '\0' },	/* '게' */

	/* followings are added for Josa/Eomi variants only */
	{ 0xbe, 0xc6, '\0' },	/* '아' */
	{ 0xbf, 0x69, '\0' },	/* '여' */
	{ 0xa4, 0xbf, '\0' },	/* 'ㅏ' */
	{ 0xa4, 0xc3, '\0' },	/* 'ㅓ' */
	{ 0xa4, 0xc5, '\0' },	/* 'ㅕ' */
	{ 0xa4, 0xc0, '\0' },	/* 'ㅐ' */
	{ 0xa4, 0xc9, '\0' },	/* 'ㅙ' */
	{ 'n', 'u', '\0' },	/* 10:'아/어' 탈락 */

	{ 0xc1, 0xf6, '\0' }	/* 11:'지' */
  }
};

