/*--------------------------------------------------------------------------
	���ξ� ���� �Լ� get_stems_TS()�� ȣ���ϴ� ���� ���α׷�
----------------------------------------------------------------------------
HAM ���̺귯�� �ȿ� �ִ� ���ξ� ���� �Լ� get_stems_TS() �����
�Ʒ� ������ ���� ���̺귯�� ������ test�� ���� �ٶ��ϴ�.

  1. index2.c�� libham.a�� subdirectory 'hdic'�� �ִ� ������ �ű��.

  2. index2.c�� compile�Ѵ�.
     $ cc -c index2.c

  3. HAM ���̺귯�� libham.a�� �Բ� link�Ѵ�.
     $ cc -o index2 index2.o -L. libham.a

  4. �������� 'index2'�� �����Ͽ� Ȯ���Ѵ�.

Ȥ��, ������ ã�� ���ϴ� error�� ����, �Լ� open_HAM_index()���� �������� �����
"./hdic/"�� �߸��� ���� "hdic/ham2000.ini"�� �� �� �κ��� ������ο� �������� Ȯ��.

�Լ� get_stems_TS()�� HAM �������� ����ϱ� ������
  1. ó���� �ѹ� �ʱ�ȭ �Լ� open_HAM_index()�� ����
  2. �Լ� get_stems_TS() ���
  3. ����� ������ �� �������� �ѹ� close_HAM_index() ȣ��
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
	char *option=NULL;	/* �ɼ��� hdic/KLT2000.ini���� ���� �����ϴ� ���� ���� */

	if (open_HAM_index(&mode, option, "./hdic/KLT2000.ini")) {	/* initialize HAM */
		puts("ERROR: dictionary file path in <hdic/KLT2000.ini>"); return 0;
	}

	puts("�ѱ� ������ �Է��Ͻÿ�.");
	while (gets(sent)) {
		nkeys = get_stems_TS(sent, keys, KeyMem, &HamOut, &mode);
		for (i = 0; i < nkeys; i++) printf("\t%s\n", keys[i]);

		puts("�ѱ� ������ �Է��Ͻÿ�.");
	}

	close_HAM_index(&mode);	/* HAM is not used any more */

	return 0;
}
/*----------------------- end of index2.c -------------------------*/
