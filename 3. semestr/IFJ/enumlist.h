// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Jakub Frýz (xfryzj01),
//				Michal Hrabovský (xhrabo08)

#ifndef ENUMLIST_H_INCLUDED
#define ENUMLIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

typedef enum {
	err=-1,eq, noteq, gt, ls, lseq, gteq, add, sub, dv, mul, //mul je hranice operandů pro precedenční analýzu
	// equal, not equal, greater, lesser, lesser equal, greater equal,
	// addition, substraction, division, multiplication
    lparenth, rparenth, id, //id je hranice terminálů pro precedenční analýzu
	//(, ), identifier,
	con_int, con_double, con_string, //don't change order
	endsymbol, lbraces, rbraces,
	//enum pro precedenč analýzu,{, }
	ass, //assigment
	// {, }, (, ), identifier
	//konstanty (int, double, string)
	semicolon, column,
	_class_, _EOF_,
	_static_, _void_, _int_, _String_, _boolean_, _double_, //keywords
	_if_, _else_, _while_, _return_,
	_ifj16readInt_, _ifj16readDouble_, _ifj16readString_, _ifj16print_, _ifj16length_, _ifj16substr_, _ifj16compare_, _ifj16find_, _ifj16sort_,
	expr_int, expr_dbl, expr_string, //precedenční analýza, pořadí nesmí být změněno
	cvtint2dbl,cvtdbl2int //opcodes
} TokenTypes;

typedef enum {
	if_start, if_end, else_start, else_end,		// if {...} else {...}
	while_start, while_condition, while_end, return_val,			// while {...}, return x
	ass_int, ass_dbl, ass_str,					// =
	eq_int, noteq_int, gt_int, gteq_int, ls_int, lseq_int, add_int, sub_int, mul_int, div_int,
	eq_dbl, noteq_dbl, gt_dbl, gteq_dbl, ls_dbl, lseq_dbl, add_dbl, sub_dbl, mul_dbl, div_dbl,
	// ==, !=, >, >=, <, <=, +, -, *, / (pro int a double)
	add_str,	// konkatenace (sčítání stringů)
	fce_call, fce_start, fce_end,	// volání funkce
	cvt_int2dbl, cvt_dbl2int,
	ifj16readIntCall, ifj16readDoubleCall, ifj16readStringCall, ifj16printCall, ifj16lengthCall, ifj16substrCall, ifj16compareCall, ifj16findCall, ifj16sortCall,
} InstrTypes;



#endif //ENUMLIST_H_INCLUDED
