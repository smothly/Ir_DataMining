형태소 분석 라이브러리 함수 link할 때 platform에 따라 아래 header 파일을 수정

	- header/ham-api.h 또는 header/ham-kma.h

의 첫 부분에 있는 아래 #define 문을 아래와 같이 정의합니다.

1. Windows XP/NT/2000

   - C언어 인터페이스인 경우

	#define WINDOWS_DLL

   - C++ 인터페이스인 경우

	#define WINDOWS_DLL_CPP

2. Linux/Unix 등 유닉스 계열에서는 comment 처리

	// #define WINDOWS_DLL_CPP

   - Linux/Unix 등의 C++ 인터페이스인 경우

	#define UNIX_CPP

<참고> header/ham-api-CPP.h (윈도 C++용), header/ham-api-CPP2.h (linux/unix C++용)

3. KLT version 2.3.0의 주요 기능

  UTF8 입출력 지원
  hdic/KLT2000.ini에서 입출력 옵션을 변경하면 됨.

<참고> 리눅스/유닉스용은 아래 연락처로 문의하기 바람.

2012년 2월 1일 강승식 nlpkang@gmail.com, 010-9929-4800
