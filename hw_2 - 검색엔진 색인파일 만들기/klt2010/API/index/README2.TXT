형태소 분석(색인어 추출) 라이브러리 사용법

1. 새버전에 포함된 실행파일 4개

	- index.exe --- 예전 버전과 사용법이 거의 같음
	- indexT.exe --- file 단위로 처리하고 빈도계산, 위치정보 출력
	- indexW.exe --- indexT.exe를 윈도 인터페이스로 실행
	- indexQ.exe --- 문장으로 된 질의어에서 추출된 용어에 대한 가중치 계산

2. index.exe와 indexT.exe의 차이점

	- index.exe --- "한 어절", "한 문장", "한 line" 단위로 용어만 추출
	- indexT.exe --- 파일 전체에 대한 용어추출 및 빈도계산, 위치정보(최대5개) 추출

index.exe는 100 어절 이내의 입력 어절들로부터 용어를 추출하는데 비해
indexT.exe는 텍스트 파일 전체에 대해 색인시에 필요한 용어 및 관련 정보들을
추출해 주기 때문에 문서 단위로 색인할 때 매우 편리함.

주의 1. indexT.exe의 함수인자로 전달되는 입력스트링 길이는 약 10Kbytes 이하로 제한됨.
주의 2. indexT.exe의 라이브러리 함수에서 추출되는 위치정보는 최대 5개로 제한됨.

한 어절이나 한 문장, 또는 한 line 정도의 입력에 대해 용어만 추출할 때는
index.exe를 사용하고, 파일전체를 색인할 때는 indexT.exe를 사용하면 편리함.

3. index.exe와 indexT.exe에서 라이브러리 함수 사용법(C 인터페이스)

index.exe의 기능에 대한 라이브러리 함수 사용법은 exe/src/index.c를 참고.
indexT.exe의 기능에 대한  라이브러리 함수 사용법은 exe/src/indexT.c를 참고.

실행파일 index.exe를 만들 때는 exe/src 에 있는

	index.c, readsent.c, out-key.c

를 VC++ 프로젝트에 포함시키고 ham2002.lib을 link하여 만든 것임.
(압축파일에 포함되어 있는 Project를 참고)

3.2 실행파일 indexT.exe를 만들 때는 index.c 대신에 indexT.c로 교체하여 컴파일.

4. indexT.exe에 대한 C++ 인터페이스

C++ 프로그램에서 형태소 분석기 DLL을 링크할 때는 형태소 분석기도 C++ interface로 사용.
(exe/src-CPP/test.c 참조)

C++ 인터페이스를 사용할 때는 exe/header/ham-api.h의

	#define WINDOWS
를
	#define WINDOWS_CPP

로 수정한 후에 컴파일 해야 함.


			2003년 10월 27일

국민대학교 컴퓨터학부 강승식(sskang@kookmin.ac.kr +82-2-910-4800, 017-226-4136)
