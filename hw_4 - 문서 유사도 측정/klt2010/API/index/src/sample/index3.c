/*--------------------------------------------------------------------------
	���� ������ ���ξ� ���� �Լ� get_stems_TEXT()�� ȣ���ϴ� ���� ���α׷�
----------------------------------------------------------------------------
HAM ���̺귯�� �ȿ� �ִ� ���ξ� ���� �Լ� get_stems_TEXT() �����
�Ʒ� ������ ���� ���̺귯�� ������ test�� ���� �ٶ��ϴ�.

  1. index3.c�� libham.a�� subdirectory 'hdic'�� �ִ� ������ �ű��.

  2. index3.c�� compile�Ѵ�.
     $ cc -c index3.c

  3. HAM ���̺귯�� libham.a�� �Բ� link�Ѵ�.
     $ cc -o index3 index3.o -L. libham.a

  4. �������� 'index3'�� �����Ͽ� Ȯ���Ѵ�.

Ȥ��, ������ ã�� ���ϴ� error�� ����, �Լ� open_HAM_index()���� �������� �����
"./hdic/"�� �߸��� ���� "hdic/ham2000.ini"�� �� �� �κ��� ������ο� �������� Ȯ��.

�Լ� get_stems_TEXT()�� HAM �������� ����ϱ� ������
  1. ó���� �ѹ� �ʱ�ȭ �Լ� open_HAM_index()�� ����
  2. �Լ� get_stems_TEXT() ���
  3. ����� ������ �� �������� �ѹ� close_HAM_index() ȣ��
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
	char *option=NULL;	/* �ɼ��� hdic/KLT2000.ini���� ���� �����ϴ� ���� ���� */

	if (open_HAM_index(&mode, option, "./hdic/KLT2000.ini")) {	/* initialize HAM */
		puts("ERROR: dictionary file path in <hdic/KLT2000.ini>"); return 0;
	}
	
	puts("�ѱ� ������ �Է��Ͻÿ�.");
	while (gets(text)) {
		n = get_terms_text(text, Term, &TM, &mode, n2, 3, 1);

		for (i=0; i < n; i++) {
			printf("%3d:%5u %6u  %s", i+1,
				Term[i].tf,			/* ����� �󵵼� */
				Term[i].score,		/* ����� ����ġ */
				TM.memTermString+Term[i].offset);	/* ts+term[i].offset : ��� ��Ʈ�� */

			nlocs = (Term[i].tf > MAX_LOCS_PER_TERM ? MAX_LOCS_PER_TERM : Term[i].tf);
			for (j = 0; j < nlocs; j++)	/* ����� ��ġ���� �ִ� MAX_LOCS_PER_TERM �� */
				printf("\t%5u", Term[i].loc[j]);
			printf("\n");
		}

		puts("�ѱ� ������ �Է��Ͻÿ�.");
	}

	close_HAM_index(&mode);	/* HAM is not used any more */

	return 0;
}
/*----------------------- end of index3.c -------------------------*/
