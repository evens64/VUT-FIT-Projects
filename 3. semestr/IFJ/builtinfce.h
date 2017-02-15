// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Michal Hrabovský (xhrabo08),
//				Roman Janík (xjanik20)

#ifndef BUILTINFCE_H_INCLUDED
#define BUILTINFCE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "error.h"
//#include "heapsort.h"
#include "ial.h"
#include "typelist.h"
#include "queue.h"

#define ALLOC_LEN 32		//  chunk pro cteny string
#define DOUBLE_LEN 13		// max delka double pri tisku pomoci %g

// zadání:
//		str. 8-9		kap. 6
//		str. 10			kap. 7
//		str. 10			kap. 8

//typedef char* String;	//definice typu 'String'

//pomocné fce--------------------------------------------------------------------------------------
/**
* @defgroup ancillary Ancillary functions
* @addtogroup ancillary
* @{
*/

/**
* Allocates string.
* @return   a new string
*/
String sInit();

/**
* Reallocates string.
* @param    s    string to be reallocated
* @pre      s 	!= NULL
* @return   a 	reallocated string
*/
int sRealloc(String s);

/**
* Tests on white chars at the beginning of string.
* @param    s    string
* @pre      s 	!= NULL
* @return   s 	 string
*/
String nonwhite(String s);

/**
* Concatenates strings.
* @param    s1    string to be concatenated
* @param    s2    string to be concatenated
* @pre      s1 		!= NULL
* @pre      s2 		!= NULL
* @return   a 		concatenated string
*/
String concat(String s1, String s2);

/**
* Converts int to string.
* @param    i	int
* @param    s	string
* @pre      i 	i = int, i >= 0
* @pre      s 	!= NULL
* @return   s  	string
*/
String itoa(int i, String s);

/**
* Counts length of int's numerals.
* @param    s    string
* @pre      s 	 != NULL
* @return   f	 length of numerals
*/
int lint(int x);

/**
* Converts token's values to string.
* @param    tok		token
* @pre      tok		!= NULL
* @return   s  		string
*/
//String convert(Ttoken *tok); nevysvetlitelna chyba

/// @}
// vestavěné fce pro načítání literálů a výpis termů------------------------------------------------
/**
* @defgroup load&print Loading and printing functions
* @addtogroup load&print
* @{
*/

/**
* Loads an int.
* @return   n	int
*/
int readInt();
/**
* Loads a double.
* @return   n	double
*/
double readDouble();

/**
* Loads a string.
* @return   n	string
*/
String readString();

/**
* Prints string or concatenation of strings from array of tokens.
* @param    tok    	array of tokens
* @pre      pocet	>=1
* @pre      tok		!= NULL
*/
void print(Ttoken *tok); //nevysvetlitelna chyba

/// @}

//vestavěné fce pro práci s řetězci----------------------------------------------------------------
/**
* @defgroup string_fun String functions
* @addtogroup string_fun
* @{
*/

/**
* Counts length of string.
* @param    s		string
* @pre      s		!= NULL
* @return   l		length
*/
int length(String s);

/**
* Returns substring.
* @param    s		string
* @param    i		starting index
* @param    n		number of chars to be copied
* @pre      s		!= NULL
* @pre      i		>0, <length(s)
* @pre      n		>0
* @return   l		substring
*/
String substr(String s, int i, int n);

/**
* Compares two strings.
* @param    s1		string
* @param    s2		string
* @pre      s1		!= NULL
* @pre      s2		!= NULL
* @return   l		int
*/
int compare(String s1, String s2);
/// @}

#endif //BUILTINFCE_H_INCLUDED
