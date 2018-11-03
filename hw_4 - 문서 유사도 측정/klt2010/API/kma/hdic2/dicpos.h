/*
	Symbol definition for dic. information.

	1. nouns --- 11 types(13 --- range: 0~12)

		N: Noun
		P: Pronoun
		U: aUxiliary noun	--> 'X'를 'U'로 수정 -- 2001.07.27
		M: nuMeral

	2. verbs --- 77+2 types(79+2 --- range: 13~91+2)

		T: Transitive verb
		I: Intransitive verb
		W: aux. Verb		--> 'V'를 'W'로 수정 -- 2001.07.27

		S: paSsive verb
		C: Causitive verb

		J: adJective
		K: aux. adjective

		g: 'geora' irregular
		n: 'neora' irregular
		d: d-irregular
		b: b-irregular
		s: s-irregular
		h: h-irregular
		r: r-irregular
		l: l-irregular

	3. others --- 6 types(6 --- range: 92+2~97+2)

		B: adverB		--> 'A'를 'B'로 수정 -- 2001.07.27
		D: Determiner	--> 'E'를 'D'로 수정 -- 2001.07.27
		L: interjection(excLamation)

	4. appended POS

		c: Cmpound noun
*/
char *dicpos[NO_POS] = {
	"",
	"N",
	"NU",
	"U",
	"NP",
	"M",
	"P",
	"NM",
	"NUM",
	"NPU",
	"",
	"PU",
	"NPUM",
	"J",	/* verb begins here */
	"I",
	"T",
	"TI",
	"Jb",
	"S",
	"Jh",
	"C",
	"Tl",
	"",
	"IJ",
	"TJ",
	"",
	"Jl",
	"IS",
	"Ig",
	"TC",
	"TIJ",
	"Il",
	"In",
	"CS",
	"Td",
	"Tg",
	"Tn",
	"Ts",
	"",
	"ICS",
	"TW",
	"TS",
	"Tb",
	"Id",
	"IC",
	"TCS",
	"TICS",
	"Jr",
	"Is",
	"TlIl",
	"TlJl",
	"TIS",
	"Ib",
	"W",
	"K",
	"TsIs",
	"TnIn",
	"IWK",
	"IW",
	"TIW",
	"TIC",
	"TWK",
	"WK",
	"TgIg",
	"IgW",
	"TbIb",
	"TId",
	"TICJW",
	"TIl",
	"TdId",
	"TIs",
	"TdIId",
	"JWK",
	"TlIlJl",
	"TTdI",
	"IJ",
	"IJbJ",
	"TJ",
	"TTsIs",
	"JK",
	"TbJ",
	"InW",
	"TlIlIrJl",
	"IIg",
	"IgJWK",
	"TgI",
	"TICW",
	"IWK",
	"TTd",
	"TlI",
	"Tu",
	"TS",
	"IsJs",	/* HANDIC added for 'DbUGb' */
	"TJWK",	/* USERDIC added for 'QlUhbGb' */
	"B",	/* adv, det, excl begins here */
	"D",
	"L",
	"BL",
	"BD",
	"DL",
	"c"		/* compound noun in 'ham-usr.dic' */
};
/* NPUM, BDL, TIJWK -- 복합품사의 순서화 --> 2001.07.27 */
