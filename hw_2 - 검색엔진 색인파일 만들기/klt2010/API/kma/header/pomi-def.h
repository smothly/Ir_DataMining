/*
	File name: pomi-def.h
	Description: Definition of prefinal Eomi field.
	Written by: Kang, Seung-Shik	04/11/1997
*/

/*
	Definition for 'pomi' field of result structure
	1-byte is used for 'Ui/WbV/WfV/AgV'.
	Each bit-potistion is as follows.

		+-----------------+-----+-----+-----+-----+
		| 4 bits(not used)|  Ui | WbV | WfV | AgV |
		+-----------------+-----+-----+-----+-----+

	If Ui-bit is set to 1, 'Ui' is found.
*/
#define POMI_AgV	0x01
#define POMI_WfV	0x02
#define POMI_WbV	0x04
#define POMI_Ui		0x08

/*------------------ end of pomi-def.h  -------------------*/
