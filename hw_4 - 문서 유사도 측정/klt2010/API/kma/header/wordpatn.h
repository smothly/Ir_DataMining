/*
	File Name: wordpatn.h
	Description: Hangul word patterns for HAM
	Written by: Kang, Seung-Shik		04/11/1997
*/

#define HAM_PTN_N      1  /* 체언 : N/PN/NM/XN/CN/UN/AS/HJ/ET */
#define HAM_PTN_NJ     2  /* 체언 + 조사 */
#define HAM_PTN_NSM    3  /* 체언 + 용언화접미사 + 어미 */
#define HAM_PTN_NSMJ   4  /* 체언 + 용언화접미사 + '음/기' + 조사 */
#define HAM_PTN_NSMXM  5  /* 체언 + 용언화접미사 + '아/어' + 보조용언 + 어미 */
#define HAM_PTN_NJCM   6  /* 체언 + '에서/부터/에서부터' + '이' + 어미 */

#define HAM_PTN_VM    11  /* 용언 + 어미 */
#define HAM_PTN_VMJ   12  /* 용언 + '음/기' + 조사 */
#define HAM_PTN_VMCM  13  /* 용언 + '음/기' + '이' + 어미 */
#define HAM_PTN_VMXM  14  /* 용언 + '아/어' + 보조용언 + 어미 */
#define HAM_PTN_VMXMJ 15  /* 용언 + '아/어' + 보조용언 + '음/기' + 조사 */

#define HAM_PTN_AID   21  /* 단일어 : 부사, 관형사, 감탄사 */
#define HAM_PTN_ADVJ  22  /* 부사 + 조사 : '빨리도' */

#define HAM_PTN_NVM   31  /* 체언 + 동사 + 어미 */

#define HAM_PTN_ZZZ   35  /* 문장부호, KS 완성형 기호열, 단독조사/어미 */

/*-------------------- end of wordpatn.h ---------------------*/
