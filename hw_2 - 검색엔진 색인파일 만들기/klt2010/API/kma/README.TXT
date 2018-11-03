1. 실행시 필요한 파일들 : hdic/KLT2000.ini, hdic/*.dic

  - hdic/KLT2000.ini

    다른 디렉토리로 옮기고 싶을 때는 src/coran.c에서 open_HAM()를
    호출한 후에 아래와 같이 set_options_from_file()을 호출.

       open_HAM(...);
       set_options_from_file("./hdic/KLT2000.ini", &mode);

  - hdic/*.dic  : 이 파일은 실행파일이 있는 디렉토리에 있어야 함.

    다른 디렉토리로 옮기고 싶을 때는 KLT2000.ini에서 사전경로를 수정하면 됨.

2. 라이브러리 link할 때 필요한 파일들

  - libindex.a
  - header/*.h
  - hdic2/*.h

			2004년 8월 9일

국민대학교 컴퓨터학부 강승식(sskang@kookmin.ac.kr +82-2-910-4800, 017-226-4136)
