/*-----------------------  ������ ����ü ����  ------------------------

	JOSA_VAR_WiAb	-- '��/��', '��/��', '��/��', '��/��', '��/��' ���� ����
		'��/��/��/��/��'�� ��쿡 �� ���� set.
	JOSA_VAR_Wz_tal	-- '��/����' ���� ���� (��) '�б���' --> '�б�'+'����'
		'��'�� Ż���Ǿ� ������ ��쿡 �� ���� set.
	JOSA_VAR_Wi_tal	-- ���� '��'�� '�̰�', '���'�� '�̶��' ���� ����
		'��'�� �����Ǿ� ������ ��쿡 �� ���� set.
	JOSA_VAR_Wg_tal	-- ���� '����'�� '��' ���� ����
		'��'�� �����Ǿ� ������ ��쿡 �� ���� set. '�б���' --> '�б�'+'����'

	JOSA_VAR_nameWi	-- �θ� �ڿ� ������ '��' �߰��Ǵ� ���
		'��'�� �м�������� ������ ���. '�½�'+'(��)��'
		<����> �� ���� �׻� JOSA_VAR_WiAb ������ �����Ѵ�.
	JOSA_VAR_preWi	-- ������ ���� '��' �տ� ���簡 ���� ���
		(��) '����/����/��������/���' + '��' + '��'
	JOSA_VAR_preWi2	-- ������ ���� '��' �տ� ���� & '��' Ż���� ���
		(��) '�б�����' --> '�б�'+'(��)��'+'(��)'+'��'

	JOSA_VAR_Ag	-- '����'�� ����ü '��'
	JOSA_VAR_Bg	-- '����'�� ����ü '��'
	JOSA_VAR_hbDtg	-- '����'�� ����ü '����'

$$$ ����, '�½�������'�� ��쿡 ������ ������� ���� ����.

-------------------------  ������ ����ü ����  ------------------------*/

/*
	Values for 'jomi.josa'.
*/
#define JOSA_VAR_WiAb	1
#define JOSA_VAR_Wz_tal	2
#define JOSA_VAR_Wi_tal	3
#define JOSA_VAR_Wg_tal	4

#define JOSA_VAR_nameWi	5
#define JOSA_VAR_preWi	6
#define JOSA_VAR_preWi2	7

/*
	Values for 'jomi.josaAgBg'.
*/
#define JOSA_VAR_Ag	1
#define JOSA_VAR_Bg	2
#define JOSA_VAR_hbDtg	3

/*---------------------  ������ ����ü ����  ----------------------

	EOMI_VAR_Wb	-- '��'
	EOMI_VAR_Wf	-- '��'
	EOMI_VAR_Wj	-- '��' : '��/��/��/��/��'
	EOMI_VAR_Wb_tal	-- '��' Ż��
	EOMI_VAR_Wf_tal	-- '��' Ż��
	EOMI_VAR_b	-- '��'
	EOMI_VAR_f	-- '��'
	EOMI_VAR_j	-- '��'
	EOMI_VAR_c	-- '��'	---> '�ؼ�', '��ż�/�Ͼ꼭' �� ��-�ұ�Ģ
	EOMI_VAR_lc	-- '��'	---> '�Ǵ�'���� ����
	EOMI_VAR_If, Ib -- '��' �ұ�Ģ�� ���

	EOMI_VAR_Wz_tal	-- ���� '��/��/��/��' �� �ʼ� '��/��'���� ���۵Ǵ� ��̿��� '��' Ż��
		<����> '��/��/��'�� ���۵Ǵ� ��̵��� '��' Ż������ �������� ����
	EOMI_VAR_Uz_tal	-- '���ϴ�'���� '��' Ż��

	EOMI_VAR_xv_Wf	-- ������� �տ� ���� ��̰� '��/��'
	EOMI_VAR_xv_Al	-- ������� �տ� ���� ��̰� '��'
	EOMI_VAR_xv_Ag	-- ������� �տ� ���� ��̰� '��'

	EOMI_VAR_Wi_tal	-- ������ ��� �ڿ��� ������ ���� '��' ����

$$$ '��/��'�� ���� ����ü�� ������� �տ� ���� '��/��'���� �����.

-----------------------  ������ ����ü ����  ----------------------*/

/*
	Values for 'jomi.eomi' or 'jomi.xomi'.
*/
#define EOMI_VAR_Wb	1
#define EOMI_VAR_Wf	2
#define EOMI_VAR_Wj	3
#define EOMI_VAR_Wb_tal	4
#define EOMI_VAR_Wf_tal	5
#define EOMI_VAR_b	6
#define EOMI_VAR_f	7
#define EOMI_VAR_j	8
#define EOMI_VAR_c	9
#define EOMI_VAR_lc	10
#define EOMI_VAR_If	11
#define EOMI_VAR_Ib	12

#define EOMI_VAR_Wz_tal	13
#define EOMI_VAR_Uz_tal	14

#define EOMI_VAR_Wi_tal	15

/*
	Values for 'jomi.xomitype'.
*/
#define EOMI_VAR_xv_Wf	0
#define EOMI_VAR_xv_Al	1
#define EOMI_VAR_xv_Ag	2
#define EOMI_VAR_xv_Xi	11

/*---------------------  �������� ����ü ����  ----------------------

	�������� ����ü ���� --- �� ���� ������ ǥ��

		1. '��' �տ� ������ '��'�� ���� ���
		2. '��/��'�� ����ü ����

	POMI_VAR_WbV	-- '��'
	POMI_VAR_WfV	-- '��'
	POMI_VAR_WjV	-- '��'
	POMI_VAR_V	-- '��'
	POMI_VAR_bV	-- '����'
	POMI_VAR_fV	-- '�ä�'
	POMI_VAR_jV	-- '�Ť�'
	POMI_VAR_cV	-- '����'	---> '�ߴ�'���� �����
	POMI_VAR_lcV	-- '�ɤ�'	---> '�ƴ�'���� �����
	POMI_VAR_IfV	-- '��' ---> '��' �ұ�Ģ�� ���
	POMI_VAR_WzUi	-- '����' & '��'
	POMI_VAR_WzUjV	-- '����' & '�Ť�', �� '����'

-----------------------  �������� ����ü ����  ----------------------*/

/*
	Values for 'jomi.pomi'.
*/
#define POMI_VAR_WbV	1
#define POMI_VAR_WfV	2
#define POMI_VAR_WjV	3
#define POMI_VAR_V	4
#define POMI_VAR_bV	5
#define POMI_VAR_fV	6
#define POMI_VAR_jV	7
#define POMI_VAR_cV	8
#define POMI_VAR_lcV	9
#define POMI_VAR_IfV	10
#define POMI_VAR_WzUi	11
#define POMI_VAR_WzUjV	12

/*---------------------  ����/��� �� ��Ÿ ���� ----------------------

	RMA_RESULT --- the result is got from 'hangul.rma'
	GUESS_ABBR --- verb stem is guessed as abbr. 'ki/kg/Zi/...'

	GUESS_CNOUN -- stem is guessed as noun + noun + ...
	GUESS_PNOUN	-- proper noun with Jongsong: articulative 'Wi' dropped.

	GUESS_NPREF -- noun stem is guessed as prefix 'Gc/Ul' + noun
	GUESS_VPREF -- verb stem is guessed as prefix 'WbD/QlU' + verb
	GUESS_NVERB -- verb stem is guessed as noun + verb + ...

-----------------------  ����/��� �� ��Ÿ ���� ----------------------*/

/*
	Values for 'jomi.zzz'.
*/
#define RMA_RESULT	1
#define GUESS_ABBR	2

#define GUESS_CNOUN	3
#define GUESS_PNOUN	4

#define GUESS_NPREF	5
#define GUESS_VPREF	6
#define GUESS_NVERB	7

/*----------------------- end of josaeomi.h -------------------------*/
