실행파일 사용법

/*========================= ham-v610c에서 수정-보완된 사항 =========================*/

  1) 자동 띄어쓰기 옵션 관련 버그 수정
	HP/AIX 등 boundary check가 엄격한 O.S.에서 segmentation fault 문제가
	발생할 수 있다는 점을 발견되어 수정함.
	(linux, Sun, Windows에서는 바로 이웃한 1바이트를 잘못 access했을 때
	그냥 넘어가기 때문에 괜찮은 것 같음)
 
  2) 형태소 분석기를 아래와 같이 사용할 경우에 불용어 사전 등 사용자 정의 사전을
	access할 때 문제 발생 --> 이 오류는 version 6.10에서만 발생함!
	(version 6.10에 추가된 기능으로 사용자 사전, 불용어 사전, 복합명사 분해 사전을 
	자동으로 소팅하여 load 하도록 수정하는 과정에서 발생한 오류를 수정하였으며,
	version 6.0 이하에서는 이 오류가 발생하지 않음)
 
      for (i = 0; i < 10; i++) {
         open_HAM_index(...);
         get_terms(...);
         close_HAM_index(...);
      }
 
     즉, 초기화를 2회 이상 할 경우에 open_HAM_index(...);에 의해 사전을 load할 때, 
     load한 개수가 2회 이상 중복 계산되어 발생한 문제를 해결하였음.
/*==================================================================================*/

정보검색용 색인어 추출기(형태소 분석기)를 사용하려면 반드시 아래 파일들이 있어야 함.

	- 실행파일 -- index.exe(또는 indexT.exe, indexQ.exe)
	  윈도용인 경우는 ham2XXX.dll 이 있어야 함.
	  
	- 사전파일 -- hdic/ham2000.ini, hdic/*.dic
	  <참고> hdic/*.* 는 실행파일이 있는 디렉토리에 있어야 하며,
	  	hdic을 다른 곳으로 이동하려면 hdic/ham2000.ini에서 경로를 지정. 

<참고> 실행파일(index.exe, indexT.exe, indexQ.exe)은 hamAPI/exe/ 디렉토리로 복사하고,
       라이브러리 파일 libindex.a(윈도용은 ham2XXX.dll)은 hamAPI/work/ 디렉토리로 복사

1. 실행 파일과 실행 방법

	index.exe  --- 어절 단위, line 단위, 혹은 문장 단위로 색인어를 추출
	indexT.exe --- 파일 단위로 색인어를 추출(빈도, 위치 계산 등)
	indexW.exe(윈도용) --- 윈도용 그래픽 사용자 인터페이스

   실행 방법: 윈도의 경우 DOS창에서 실행

   - 입력 파일 test.txt일 경우

	C> index test.txt

		==> 분석 결과가 표준출력장치(화면)에 출력됨

	C> index test.txt test.out

		==> 분석 결과를 test.out이라는 파일로 저장함

   - 대화형 모드로 실행할 경우

	C> index -2

		==> 문장을 직접 입력하고 <Enter> 키를 치면 분석을 함


2. 실행시 필요한 파일들 : hdic/*.*

  - hdic/ham2000.ini --- 실행시 option을 지정하는 기능

	. 형태소 분석기는 이 텍스트 파일에 명시된 분석 옵션에 따라 실행됨.
	. 옵션을 변경하려면 editor로 해당 옵션을 수정하면 됨.

	예) 자동 띄어쓰기 옵션을 적용할 때 ham2000.ini의
		AutoSpacing=0
	    을
		AutoSpacing=1
	    로 수정하여 실행

  - hdic/*.dic  : 이 파일들은 실행파일과 동일한 디렉토리에 있어야 함.

    다른 디렉토리로 옮기고 싶을 때는 hdic/ham2000.ini에서 사전경로를 수정.

	예) DicDirPath=/usr/kang/ham600/hdic/

3. 라이브러리 함수 사용법

3.1 라이브러리 link할 때 필요한 파일들

  - libindex.a(Unix, linux 계열), 윈도용은 ham2XXX.dll
  - header/*.h

3.2 라이브러리 link 방법

  UNIX, 리눅스 운영체제에서는 makefile(work/makefile, work/indexT.mak, work/indexQ.mak)을
	참조하여 라이브러리를 링크하고 실행 파일을 생성함.

  work/makefile --- 어절 단위, line 단위, 혹은 문장 단위로 색인어를 추출하고자 할 때
	work/src/index.c의 라이브러리 함수 사용법 참조

  work/indexT.mak --- 파일 단위로 색인어를 추출(빈도, 위치 계산 등)하고자 할 때
	work/src/indexT.c의 라이브러리 함수 사용법 참조


  윈도에서는 Project 폴더에 있는 VC++ 프로젝트를 참조하여 DLL을 링크하여 사용.


			2003년 10월 27일

국민대학교 컴퓨터학부 강승식(sskang@kookmin.ac.kr +82-2-910-4800, 017-226-4136)

<참고> 형태소 분석 및 HAM 라이브러리 사용법에 관한 구체적인 내용은 아래 책을 참고하세요.

	"한국어 형태소 분석과 정보 검색", 홍릉과학출판사, 강승식
