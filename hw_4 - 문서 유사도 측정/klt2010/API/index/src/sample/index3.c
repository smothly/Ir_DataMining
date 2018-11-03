/*--------------------------------------------------------------------------
	문서 단위의 색인어 추출 함수 get_stems_TEXT()를 호출하는 예제 프로그램
----------------------------------------------------------------------------
HAM 라이브러리 안에 있는 색인어 추출 함수 get_stems_TEXT() 사용방법
아래 순서에 의해 라이브러러 사용법을 test해 보기 바랍니다.

  1. index3.c와 libham.a를 subdirectory 'hdic'이 있는 곳으로 옮긴다.

  2. index3.c를 compile한다.
     $ cc -c index3.c

  3. HAM 라이브러리 libham.a와 함께 link한다.
     $ cc -o index3 index3.o -L. libham.a

  4. 실행파일 'index3'를 실행하여 확인한다.

혹시, 사전을 찾지 못하는 error가 나면, 함수 open_HAM_index()에서 사전파일 경로인
"./hdic/"이 잘못된 경우로 "hdic/ham2000.ini"의 맨 앞 부분의 사전경로에 사전파일 확인.

함수 get_stems_TEXT()는 HAM 사전들을 사용하기 때문에
  1. 처음에 한번 초기화 함수 open_HAM_index()를 실행
  2. 함수 get_stems_TEXT() 사용
  3. 사용이 끝났을 때 마지막에 한번 close_HAM_index() 호출
----------------------------------------------------------------------------*/

#include "../../header/ham-ndx.h"
#include "../../header/ham-api.h"

/*========================== Global Variable ===========================*/
HAM_TERMLIST Term[MAX_TERMS_DOC];	/* extracted terms */
HAM_TERMMEMORY TM;	/* termtext.h: memories needed for term extraction */
/*======================================================================*/

#define MAXTEXTSIZE	9999

main()
{
	int i, j, nlocs;
	unsigned char text[MAXTEXTSIZE];	/* input text */
	int n, n2=0;	/* number of keywords in 'keylist' */
	HAM_RUNMODE mode;	/* 'header/runmode.h' */
	char *option=NULL;	/* 옵션은 hdic/KLT2000.ini에서 값을 수정하는 것이 좋음 */

	if (open_HAM_index(&mode, option, "./hdic/KLT2000.ini")) {	/* initialize HAM */
		puts("ERROR: dictionary file path in <hdic/KLT2000.ini>"); return 0;
	}
	
	puts("한글 문서를 입력하시오.");
	while (gets(text)) {
		n = get_terms_text(text, Term, &TM, &mode, n2, 3, 1);

		for (i=0; i < n; i++) {
			printf("%3d:%5u %6u  %s", i+1,
				Term[i].tf,			/* 용어의 빈도수 */
				Term[i].score,		/* 용어의 가중치 */
				TM.memTermString+Term[i].offset);	/* ts+term[i].offset : 용어 스트링 */

			nlocs = (Term[i].tf > MAX_LOCS_PER_TERM ? MAX_LOCS_PER_TERM : Term[i].tf);
			for (j = 0; j < nlocs; j++)	/* 용어의 위치정보 최대 MAX_LOCS_PER_TERM 개 */
				printf("\t%5u", Term[i].loc[j]);
			printf("\n");
		}

		puts("한글 문서를 입력하시오.");
	}

	close_HAM_index(&mode);	/* HAM is not used any more */

	return 0;
}
/*----------------------- end of index3.c -------------------------*/
