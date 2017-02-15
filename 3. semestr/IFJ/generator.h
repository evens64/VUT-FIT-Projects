// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Jakub Frýz (xfryzj01)

#ifndef GENERATOR_H_INCLUDED
#define GENERATOR_H_INCLUDED

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "enumlist.h"
#include "ial.h"

//--definice-funkcí----------------------------------------------------------------------

typedef struct SInstr {
	InstrTypes t;								// typ instrukce
	void *op1;									// první operand
	void *op2;									// druhý operand
	void *res;									// výsledek
	struct SInstr *next;						// ukazatel na další prvek
	struct SInstr *nope;						// ukazetel na další prvek (if-else, while)
} TInstr;										// struktura instrukce

typedef struct SInstrList {
	struct SInstr *First;						// ukazatel na první prvek
	struct SInstr *Last;						// ukazatel na poslední prvek
	struct SInstr *Act;							// ukazatel na aktivní prvek
} TInstrList;									// struktura listu

typedef struct SAdr {
	TInstr *address;							//ukazatel na prvek listu
	struct SAdr *next;							//ukazatel na další prvek
} TAdr;

typedef struct SAdrStack {
	struct SAdr *Top;
} TAdrStack;									// struktura zásobníku s ukazateli

//--prototypy-funkcí---------------------------------------------------------------------

// fce na generování 3-adresného kódu
void generate(InstrTypes t, void *op1, void *op2, void *res);

// pomocné fce
TInstr *AllocInstr();							// alokace struktury instrukce
TAdr *AllocAdr();								// alokace struktury adresy na instrukci

// práce s listem
void IListInit(TInstrList *L);					// inicializace listu
void IListInsert(TInstrList *L, TInstr *I);		// vložení do listu
void IListInsertElse(TInstrList *L, TInstr *I);	// vložení do listu (if-else)
void IListDispose(TInstrList *L);				// smazání listu

void IActivate(TInstrList *L);					// aktivace listu
void IActiveNext(TInstrList *L);				// aktivace dalšího prvku
void IActiveElse(TInstrList *L);				// aktivace dalšího prvku (if-else, while)
void IGoTo(TInstrList *L, TInstr *I);			// skok na položku v Listu

bool IListActive(TInstrList *L);

// práce se zásobníkem
void IStackInit(TAdrStack *S);					// inicializace zásobníku
void IStackPush(TAdrStack *S, TInstr *I);		// vložení do zásobníku
TInstr *IStackPop(TAdrStack *S);				// vyhození ze zásobníku
void IStackTopRemove(TAdrStack *S);				// smazání vrchního prvku
void IStackDispose(TAdrStack *S);				// smazání zásobníku

#endif //GENERATOR_H_INCLUDED
