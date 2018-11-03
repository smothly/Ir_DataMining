실행파일 사용법

1. 실행 파일과 실행 방법

	index.exe  --- 어절 단위, line 단위, 혹은 문장 단위로 색인어를 추출
	indexT.exe --- 파일 단위로 색인어를 추출(빈도, 위치 계산 등)
	indexW.exe(윈도용) --- 윈도용 그래픽 사용자 인터페이스

   실행 방법: 윈도의 경우 cmd 창에서 실행. 마우스 더블 클릭으로 실행하는 것이 아니라...

   - 입력 파일 test.txt일 경우

	C> index.exe test.txt

		==> 분석 결과가 표준출력장치(화면)에 출력됨

	C> index.exe test.txt test.out

		==> 분석 결과를 test.out이라는 파일로 저장함

   - 대화형 모드로 실행할 경우

	C> index.exe -2

		==> 문장을 직접 입력하고 <Enter> 키를 치면 분석을 함

2. DOS 창에서 index.exe를 실행하는 방법

  - 화면 좌측하단의 시작-실행(R)-열기(O)에 "cmd" 입력하면 까만색 DOS창이 생성됨.

  - DOS창에서 cd 명령어를 이용하여 아래 예와 같이 압축 파일이 설치된 폴더로 이동해야 함.

     C> cd "C:\Documents and Settings\sskang\Desktop\KLT2010-TestVersion\EXE"

   <참고> 형태소 분석기 폴더로 이동이 어려우면 압축을 "C:\" 에 풀어서 아래와 같이 찾아감
	C> cd \
	C> cd KLT2010-TestVersion


3. 실행시 필요한 파일들 : hdic/*.*

- hdic/KLT2000.ini -- 실행 option을 지정하는 기능

	. 형태소 분석기는 이 텍스트 파일에 명시된 분석 옵션에 따라 실행됨.
	. 옵션을 변경하려면 editor로 해당 옵션을 수정하면 됨.

	예) 자동 띄어쓰기 옵션을 적용할 때 klt2000.ini의
		AutoSpacing=0
	    을
		AutoSpacing=1
	    로 수정하여 실행

- UTF8 입출력 옵션 -- hdic/KLT2000.ini의 아래 옵션 값을 2로 셋팅
;----------------------------------------------------------------------------
; 입출력 한글코드를 지정할 수 있는 옵션
; Default --> KS완성형(KS C 5601-1987)
;	'1' --> KSC5601(default)
;	'2' --> UTF8 인코딩
HangulCodeInput=2
HangulCodeOutput=2
;----------------------------------------------------------------------------

  <참고> UTF8 입출력 테스트 -- indexS.c 대신에 indexS-utf8.c 를 사용하여 실행파일 만듦.

- hdic/*.dic  : 이 파일은 실행파일과 동일한 디렉토리에 있어야 함.

    다른 디렉토리로 옮기고 싶을 때는 KLT2000.ini에서 사전경로를 수정하면 됨.

	예) DicDirPath=/usr/kang/KLT210/EXE/hdic/

    <<다른 방법>> open_HAM_index() 대신에 open_HAM_index_dicpath()를 사용하면 됨.

4. 라이브러리 함수 사용법

4.1 라이브러리 link할 때 필요한 파일들

  - libindex-TEST.a (KLT2000-TEST.dll --> windows95/98/NT)
  - header/*.h

4.2 라이브러리 link 방법

  UNIX, 리눅스 운영체제에서는 makefile(API/Makefile, API/indexT.mak, API/kma.mak 등)을
	참조하여 라이브러리를 링크하고 실행 파일을 생성함.

  API/Makefile --- 어절 단위, line 단위, 혹은 문장 단위로 색인어를 추출하고자 할 때
	work/src/indexS.c의 라이브러리 함수 사용법 참조

  API/indexT.mak --- 파일 단위로 색인어를 추출(빈도, 위치 계산 등)하고자 할 때
	API/src/indexT.c의 라이브러리 함수 사용법 참조


  윈도에서는 Project 폴더에 있는 VC++ 프로젝트를 참조하여 DLL을 링크하여 사용.


			2012년 2월 1일

국민대학교 강승식(nlpkang@gmail.com +82-10-9929-4800)

<참고> 형태소 분석 및 HAM 라이브러리 사용법에 관한 구체적인 내용은 아래 책을 참고하세요.

	"한국어 형태소 분석과 정보 검색", 홍릉과학출판사, 강승식

