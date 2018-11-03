/*--------------------------------------------------------------------------
	색인어 추출 함수 get_stems_TS()를 호출하는 예제 프로그램
----------------------------------------------------------------------------
HAM 라이브러리 안에 있는 색인어 추출 함수 get_stems_TS() 사용방법
아래 순서에 의해 라이브러러 사용법을 test해 보기 바랍니다.

  1. index2.c와 libham.a를 subdirectory 'hdic'이 있는 곳으로 옮긴다.

  2. index2.c를 compile한다.
     $ cc -c index2.c

  3. HAM 라이브러리 libham.a와 함께 link한다.
     $ cc -o index2 index2.o -L. libham.a

  4. 실행파일 'index2'를 실행하여 확인한다.

혹시, 사전을 찾지 못하는 error가 나면, 함수 open_HAM_index()에서 사전파일 경로인
"./hdic/"이 잘못된 경우로 "hdic/ham2000.ini"의 맨 앞 부분의 사전경로에 사전파일 확인.

함수 get_stems_TS()는 HAM 사전들을 사용하기 때문에
  1. 처음에 한번 초기화 함수 open_HAM_index()를 실행
  2. 함수 get_stems_TS() 사용
  3. 사용이 끝났을 때 마지막에 한번 close_HAM_index() 호출
----------------------------------------------------------------------------*/

#include "../../header/ham-ndx.h"
#include "../../header/ham-api.h"
#include "../../header/keyword.h"

/*==============================================================================
	Following 2 variables are required for get_stems_TS() -- multi-threading.
	i.e. They are not needless when you use get_stems().
==============================================================================*/
char KeyMem[MAXKEYBYTES];	/* TEMPORARY MEMORY for keyword string */
HAM_MORES HamOut;	/* TEMPORARY MEMORY for morph. analysis */

main()
{
	int i;
	unsigned char sent[SENTSIZE];	/* input word or sentence */
	unsigned char *keys[MAXKEYWORDS];	/* extracted keywords */
	int nkeys;	/* number of keywords in 'keys' */
	HAM_RUNMODE mode;	/* 'header/runmode.h' */
	char *option=NULL;	/* 옵션은 hdic/KLT2000.ini에서 값을 수정하는 것이 좋음 */

	if (open_HAM_index(&mode, option, "./hdic/KLT2000.ini")) {	/* initialize HAM */
		puts("ERROR: dictionary file path in <hdic/KLT2000.ini>"); return 0;
	}

	puts("한글 문장을 입력하시오.");
	while (gets(sent)) {
		nkeys = get_stems_TS(sent, keys, KeyMem, &HamOut, &mode);
		for (i = 0; i < nkeys; i++) printf("\t%s\n", keys[i]);

		puts("한글 문장을 입력하시오.");
	}

	close_HAM_index(&mode);	/* HAM is not used any more */

	return 0;
}
/*----------------------- end of index2.c -------------------------*/
