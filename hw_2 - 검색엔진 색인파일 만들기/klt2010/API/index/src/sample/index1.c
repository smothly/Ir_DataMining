/*--------------------------------------------------------------------------
	���ξ� ���� �Լ� get_stems()�� ȣ���ϴ� ���� ���α׷�
----------------------------------------------------------------------------
HAM ���̺귯�� �ȿ� �ִ� ���ξ� ���� �Լ� get_stems() �����
�Ʒ� ������ ���� ���̺귯�� ������ test�� ���� �ٶ��ϴ�.

  1. index1.c�� libham.a�� subdirectory 'hdic'�� �ִ� ������ �ű��.

  2. index1.c�� compile�Ѵ�.
     $ cc -c index1.c

  3. HAM ���̺귯�� libham.a�� �Բ� link�Ѵ�.
     $ cc -o index1 index1.o -L. libham.a

  4. �������� 'index1'�� �����Ͽ� Ȯ���Ѵ�.

Ȥ��, ������ ã�� ���ϴ� error�� ����, �Լ� open_HAM_index()���� �������� �����
"./hdic/"�� �߸��� ���� "hdic/ham2000.ini"�� �� �� �κ��� ������ο� �������� Ȯ��.

�Լ� get_stems()�� HAM �������� ����ϱ� ������
  1. ó���� �ѹ� �ʱ�ȭ �Լ� open_HAM_index()�� ����
  2. �Լ� get_stems() ���
  3. ����� ������ �� �������� �ѹ� close_HAM_index() ȣ��
----------------------------------------------------------------------------*/

#include "../../header/ham-ndx.h"
#include "../../header/ham-api.h"
#include "../../header/keyword.h"

main()
{
	int i;
	unsigned char sent[SENTSIZE];	/* input word or sentence */
	unsigned char *keys[MAXKEYWORDS];	/* extracted keywords */
	int nkeys;	/* number of keywords in 'keys' */
	HAM_RUNMODE mode;	/* 'header/runmode.h' */
	char *option=NULL;	/* �ɼ��� hdic/KLT2000.ini���� ���� �����ϴ� ���� ���� */

	if (open_HAM_index(&mode, option, "./hdic/KLT2000.ini")) {	/* initialize HAM */
		puts("ERROR: dictionary file path in <hdic/KLT2000.ini>");	return 0;
	}

	puts("�ѱ� ������ �Է��Ͻÿ�.");
	while (gets(sent)) {
		nkeys = get_stems(sent, keys, &mode);	/* multi-threading --> get_stems_TS() ��� */
		for (i = 0; i < nkeys; i++) printf("\t%s\n", keys[i]);

		puts("�ѱ� ������ �Է��Ͻÿ�.");
	}

	close_HAM_index(&mode);	/* HAM is not used any more */

	return 0;
}
/*----------------------- end of index1.c -------------------------*/
