/*
	File name: pos-patn.h
	Description: Definition of sentence types and parts of speech
	Written by: Kang, Seung-Shik	04/03/1993
*/

/*                                                          */
/*         CLASSIFICATION OF SENTENCE PATTERNS              */
/*                                                          */
#define HAM_SPTN_DECL      'D'       /* declarative sentence */
#define HAM_SPTN_QUES      'Q'       /* question sentence    */
#define HAM_SPTN_IMPR      'I'       /* imperative sentence  */
#define HAM_SPTN_TITL      'T'       /* title of a paragraph */

/*                                                          */
/*          CLASSIFICATION OF PARTS OF SPEECH               */
/*                                                          */

/*
	3(basic) + 2(special) types of stem for 'pos'
*/
#define HAM_POS_NPXM     'N'       /* noun, pnoun, xn, nume */
#define HAM_POS_VJXV     'V'       /* verb, adj, xverb      */
#define HAM_POS_AID      'Z'       /* adv, det, excl        */

#define HAM_POS_PUNC     'q'       /* punctuation mark:./,/( */
#define HAM_POS_SYMB     'Q'       /* special symbols       */

/*
	normal types of stem for 'pos2'.
	Only some of following symbols are used.
*/
#define HAM_POS_NOUN     'N'       /* noun                  */
#define HAM_POS_PNOUN    'P'       /* pronoun               */
#define HAM_POS_XNOUN    'U'       /* dependent noun        */
#define HAM_POS_NUMERAL  'M'       /* numeral               */

#define HAM_POS_PROPER   'O'       /* proper noun: NOT USED */

#define HAM_POS_CNOUN    'C'       /* compound noun guessed */
#define HAM_POS_NOUNK    'u'       /* guessed as noun       */

#define HAM_POS_ASCall   '@'       /* all alphanumeric chars*/
#define HAM_POS_ASCend   '$'       /* end with alphanumeric */
#define HAM_POS_ASCmid   '*'       /* ..+alphanumeric+Hangul*/

/* defined for numeral to digit conversion */
#define HAM_POS_digits   '1'       /* digit-string */
#define HAM_POS_digitH   '2'       /* digit-string + Hangul*/

#define HAM_POS_VERB     'V'       /* verb                  */
#define HAM_POS_ADJ      'J'       /* adjective             */
#define HAM_POS_XVERB    'W'       /* auxiliary verb        */
#define HAM_POS_XADJ     'K'       /* NOT USED YET          */

#define HAM_POS_ADV      'B'       /* adverb                */
#define HAM_POS_DET      'D'       /* determiner            */
#define HAM_POS_EXCL     'L'       /* exclamation           */

#define HAM_POS_JOSA     'j'       /* Korean Josa           */
#define HAM_POS_COPULA   'c'       /* copula '-Wi-'         */
#define HAM_POS_EOMI     'e'       /* final Ending          */
#define HAM_POS_PEOMI    'f'       /* prefinal Ending       */
#define HAM_POS_NEOMI    'n'       /* nominalizing Eomi     */

#define HAM_POS_PREFIX   'p'       /* prefixes              */
#define HAM_POS_SFX_N    's'       /* noun suffixes: '들/적'*/
#define HAM_POS_SFX_V    't'       /* verb suffixes: '하/되'*/

#define HAM_POS_ETC      'Z'       /* not decided yet       */

/* ASCII stem may be classified as follows: NOT USED YET    */
#define HAM_POS_ALPHA    'A'       /* English alphabet      */
#define HAM_POS_NUMBER   '#'       /* Arabic numbers        */
#define HAM_POS_SMARK    'R'       /* sentence markers      */

#define HAM_POS_NVERBK   'Y'       /* guessed as noun+verb  */

#define HAM_POS_SQUOTE   's'       /* single quotation      */
#define HAM_POS_DQUOTE   'd'       /* double quotation      */
#define HAM_POS_LPAREN   'l'       /* left parenthesis      */
#define HAM_POS_RPAREN   'r'       /* right parenthesis     */
/*------------------- end of pos-patn.h  -------------------*/
