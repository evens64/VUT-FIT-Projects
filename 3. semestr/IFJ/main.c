// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Jakub Frýz (xfryzj01),
//				Michal Hrabovský (xhrabo08)

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "generator.h"
#include "parser.h"
#include "scanner.h"
#include "interpreter.h"

tBST BST;
Tqueue Q;

TInstrList IList;					// list instrukcí
TAdrStack IStack;					// zásobník adres na instrukce

t_AdrList tempints;
t_AdrList tempdbls;
t_AdrList tempstrs;
t_AdrList literals;
t_AdrList strings;

int main (int argc, char* argv[]) {
	if (argc != 2) {				// kontrola argumentů
		print_error(err_oth);
	}
	QInit(&Q);
	BSTInit(&BST);
	AdrListInit(&tempints);
	AdrListInit(&tempdbls);
	AdrListInit(&tempstrs);
	AdrListInit(&literals);
	AdrListInit(&strings);

	IListInit(&IList);				// inicializace listu instrukcí
	IStackInit(&IStack);			// inicializace stacku adres na instrukce

	scanner(argv[1]);
	body_f();

	interprete(&BST);				// interpret

	IStackDispose(&IStack);			// smazání zásobníku adres na instrukce
	IListDispose(&IList);			// smazání listu instrukcí

	AdrListDispose(&tempints);
	AdrListDispose(&tempdbls);
	AdrListDisposeStrings(&tempstrs);
	AdrListDispose(&literals);
	AdrListDisposeStrings(&strings);

	return 0;
}


