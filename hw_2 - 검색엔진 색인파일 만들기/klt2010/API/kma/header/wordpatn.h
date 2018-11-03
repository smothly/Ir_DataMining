/*
	File Name: wordpatn.h
	Description: Hangul word patterns for HAM
	Written by: Kang, Seung-Shik		04/11/1997
*/

#define HAM_PTN_N      1  /* ü�� : N/PN/NM/XN/CN/UN/AS/HJ/ET */
#define HAM_PTN_NJ     2  /* ü�� + ���� */
#define HAM_PTN_NSM    3  /* ü�� + ���ȭ���̻� + ��� */
#define HAM_PTN_NSMJ   4  /* ü�� + ���ȭ���̻� + '��/��' + ���� */
#define HAM_PTN_NSMXM  5  /* ü�� + ���ȭ���̻� + '��/��' + ������� + ��� */
#define HAM_PTN_NJCM   6  /* ü�� + '����/����/��������' + '��' + ��� */

#define HAM_PTN_VM    11  /* ��� + ��� */
#define HAM_PTN_VMJ   12  /* ��� + '��/��' + ���� */
#define HAM_PTN_VMCM  13  /* ��� + '��/��' + '��' + ��� */
#define HAM_PTN_VMXM  14  /* ��� + '��/��' + ������� + ��� */
#define HAM_PTN_VMXMJ 15  /* ��� + '��/��' + ������� + '��/��' + ���� */

#define HAM_PTN_AID   21  /* ���Ͼ� : �λ�, ������, ��ź�� */
#define HAM_PTN_ADVJ  22  /* �λ� + ���� : '������' */

#define HAM_PTN_NVM   31  /* ü�� + ���� + ��� */

#define HAM_PTN_ZZZ   35  /* �����ȣ, KS �ϼ��� ��ȣ��, �ܵ�����/��� */

/*-------------------- end of wordpatn.h ---------------------*/
