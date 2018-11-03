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
	{ 0xb4, 0xe1, '\0' },	/* 0:'��' */
	{ 0x89, 0xa1, '\0' },	/* 1:'��' */
	{ 0x89, 0x41, '\0' },	/* 2:'��' */

	/* followings are added for Josa/Eomi variants only */
	{ 0xb4, 0x61, '\0' },	/* 3:'��' */
	{ 0xb4, 0xa1, '\0' },	/* 4:'��' */
	{ 0x84, 0x61, '\0' },	/* 5:'��' */
	{ 0x84, 0xe1, '\0' },	/* 6:'��' */
	{ 0x85, 0x61, '\0' },	/* 7:'��' */
	{ 0x84, 0x81, '\0' },	/* 8:'��' */
	{ 0x85, 0xe1, '\0' },	/* 9:'��' */
	{ 'n', 'u', '\0' },	/* 10:'��/��' Ż�� */

	{ 0xbb, 0xa1, '\0' }	/* 11:'��' */
  },
  {
	/*
		3+1 KSC 5601 Eomi's that precede aux. verb.
	*/
	{ 0xbe, 0xee, '\0' },	/* '��' */
	{ 0xb0, 0xed, '\0' },	/* '��' */
	{ 0xb0, 0xd4, '\0' },	/* '��' */

	/* followings are added for Josa/Eomi variants only */
	{ 0xbe, 0xc6, '\0' },	/* '��' */
	{ 0xbf, 0x69, '\0' },	/* '��' */
	{ 0xa4, 0xbf, '\0' },	/* '��' */
	{ 0xa4, 0xc3, '\0' },	/* '��' */
	{ 0xa4, 0xc5, '\0' },	/* '��' */
	{ 0xa4, 0xc0, '\0' },	/* '��' */
	{ 0xa4, 0xc9, '\0' },	/* '��' */
	{ 'n', 'u', '\0' },	/* 10:'��/��' Ż�� */

	{ 0xc1, 0xf6, '\0' }	/* 11:'��' */
  }
};

