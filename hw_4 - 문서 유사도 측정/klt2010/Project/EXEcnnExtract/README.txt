형태소 분석기를 이용한 복합명사 인식 프로그램

1. 실행 -- data 폴더에 데이터 파일이 몇 개 있음

	C> getcnoun <input-file.txt>

	C> getcnoun data\hiron.txt


2. 출력 결과에 1바이트 코드 정의

	pos-patn.h 에 각 유형에 대한 설명이 있음

3. getcnoun.exe 만드는 방법

	coran-cnoun2.c를 KLT2000.lib을 링크하여 build하면 됨

2012년 7월 30일 강승식
