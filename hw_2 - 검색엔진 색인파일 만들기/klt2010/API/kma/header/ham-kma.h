/*
	File Name: ham-kma.h
	Description: header file collections for HAM
	Written by: Kang, Seung-Shik	Apr.1997 - Mar.2005
*/
/*-------------------------------------------------------------
	Compile options
	  modify/remove the following line for Windows/Unix(linux)
-------------------------------------------------------------*/
#define WINDOWS_DLL		/* -> delete or modify this line!!! */

#ifdef WINDOWS_DLL				/* Windows DLL C interface */
#define PREFIX	_declspec(dllimport)
#elif defined(WINDOWS_DLL_CPP)	/* Windows DLL C++ interface */
#define PREFIX	"C" _declspec(dllimport)
#else							/* Linux/Unix 등 유닉스 계열 */
#define PREFIX
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "typedefs.h"
#include "sizedefs.h"
#include "runmode.h"

#include "hdics.h"
#include "io-defs.h"

#include "hamout1.h"
#include "hamout2.h"

#include "err-code.h"

#include "pos-patn.h"
#include "wordpatn.h"
#include "josaeomi.h"

//#include "cnoun.h"

extern PREFIX int open_HAM(HAM_RUNMODE *, char *, char *);
extern PREFIX HAM_PMORES morph_anal(
	HAM_PUCHAR sent,	/* input sentence: KSC-5601 code */
	HAM_PMORES2 hamout2,	/* token-based morph. analysis result */
	HAM_PRUNMODE mode);	/* running mode of HAM */
extern PREFIX HAM_PMORES morph_anal_TS(
	HAM_PUCHAR sent,	/* input sentence: KSC-5601 code */
	HAM_PMORES hamout1,	/* frame-based morph. analysis result */
	HAM_PMORES2 hamout2,	/* token-based morph. analysis result */
	HAM_PRUNMODE mode);	/* running mode of HAM */
extern PREFIX void close_HAM(void);

/* USED ONLY FOR 'ADJUST_WORD_SPACING' OF INPUT SENTENCE */
extern PREFIX int load_bigram_data(char *filename);
extern PREFIX void adjust_word_spacing(unsigned char sent[], int);
extern PREFIX void free_bigram_data();

#define WELCOME(version) \
	fprintf(stderr, "------------------------------------------------------------------\n"); \
	fprintf(stderr, "      Welcome to KLT(Korean Language Technology) version %s\n", version); \
	fprintf(stderr, "   <<< BEFORE USING IT, YOU SHALL READ THE FILE LICENSE.TXT >>>\n"); \
	fprintf(stderr, "------------------------------------------------------------------\n")

#define GOODBYE(year) \
	fprintf(stderr, "------------------------------------------------------------------\n"); \
	fprintf(stderr, "(c) 1993-%d KLT(Korean Language Technology), Seung-Shik Kang\n", year); \
	fprintf(stderr, "    Email: sskang@kookmin.ac.kr, http://nlp.kookmin.ac.kr/\n"); \
	fprintf(stderr, "------------------------------------------------------------------\n")
/*--------------------------------- end of ham-kma.h ---------------------------------*/
