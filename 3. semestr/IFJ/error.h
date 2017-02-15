// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Jakub Frýz (xfryzj01)

#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

// zadání:
//		str. 2			kap. 2

typedef enum {				// enumerátor typů chyb, které mohou nastat
	err_lex = 1,			// chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému)
	err_syn = 2,			// chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu)
	err_sem_und = 3,		// sémantická chyba v programu – nedefinovaná třída/funkce/proměnná, pokus o redefinici třídy/funkce/proměnné, atd.
	err_sem_kom = 4,		// sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech, příp. špatný počet či typ parametrů u volání funkce
	err_sem_oth = 6,		// ostatní sémantické chyby
	err_run_load = 7,		// běhová chyba při načítání číselné hodnoty ze vstupu
	err_run_uni = 8,		// běhová chyba při práci s neinicializovanou proměnnou
	err_run_div = 9,		// běhová chyba dělení nulou
	err_run_oth = 10,		// ostatní běhové chyby
	err_oth = 99			// interní chyba interpretu tj. neovlivněná vstupním programem (např. chyba alokace paměti, chyba při otvírání souboru s řídicím programem, špatné parametry příkazové řádky atd.).
} Terror;

void print_error (Terror err);

#endif //ERROR_H_INCLUDED
