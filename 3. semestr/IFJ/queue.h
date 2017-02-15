// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Jakub Frýz (xfryzj01),
//				Michal Hrabovský (xhrabo08)

#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "enumlist.h"
#include "error.h"
#include "ial.h"

typedef struct Stoken_val {		//struktura s hodnotama pro token
	int i;			//integer
	double d;		//double
	char* s;		//string
} Ttoken_val;

typedef struct Stoken {			//struktura jednotlivých tokenů
	void *value;		//struktura s hodnotama
	struct Stoken *next;	//ukazatel na další token
	TokenTypes type;		//typ tokenu
} Ttoken;

typedef struct Squeue {			//struktura fronty
	Ttoken *First;		//ukazatel na první prvek
	Ttoken *Last;		//ukazatel na poslední token
} Tqueue;

typedef struct SStack {			//struktura fronty
	Ttoken *First;		//ukazatel na první prvek
} Tstack;



typedef struct AdrList_Item{
    void * ptr;
    char occupied;
    struct AdrList_Item *next;
} t_AdrList_Item;

typedef struct AdrList{
    t_AdrList_Item *First;
} t_AdrList;



void QInit(Tqueue *Q);
void QLastInsert_template(Tqueue *Q, TokenTypes t, int i, double d, char* s);

//--

void QLastInsert(Tqueue *Q, TokenTypes t);
void QLastInsertInt(Tqueue *Q, int i);
void QLastInsertDouble(Tqueue *Q, double d);
void QLastInsertString(Tqueue *Q, char* s);
void QLastInsertID(Tqueue *Q, char* s);
void QLastInsertRaw(Tqueue* Q,Ttoken *token);
//--

void QDeleteFirst(Tqueue *Q);
Ttoken *QPopFirst(Tqueue *Q);
Ttoken *QPeek(Tqueue *Q);
Ttoken *QPeekLast(Tqueue *Q);
TokenTypes QCheckFirst(Tqueue *Q);
TokenTypes QCheckSecond(Tqueue *Q);

void QCopyFirstInt(Tqueue *Q, int *i);
void QCopyFirstDouble(Tqueue *Q, double *d);
void QCopyFirstString(Tqueue *Q, char* *s);


void QSoftCopyFirst(Tqueue *Q, Ttoken *t);

void SInit(Tstack *S);
void SPush(Tstack *S, TokenTypes sym, void * adr);
void SPushBehind(Tstack *S, TokenTypes sym, void * adr);
void SPushRaw(Tstack *S, Ttoken *token);
void SPushBehindRaw(Tstack *S, Ttoken *token);
Ttoken *SPop(Tstack *S);
void SPopDelete(Tstack *S);
Ttoken *SPeek(Tstack *S);
void Sdispose(Tstack *S);
Ttoken *STopTerminal(Tstack *S);

void AdrListInit(t_AdrList* lst);
void *AdrListNewInt(t_AdrList* lst);
void *AdrListNewDbl(t_AdrList* lst);
void *AdrListNewString(t_AdrList* lst);
void *AdrListInsertRaw(t_AdrList* lst, void* ptr);
void *AdrListGetAvailable(t_AdrList* lst);
void AdrListMakeAvailable(void* ptr);
void AdrListMakeAvailableArgs(struct parameters_item *Ptr);

void AdrListDispose(t_AdrList* l);
void AdrListDisposeStrings(t_AdrList* l);
#endif // QUEUE_H_INCLUDED
