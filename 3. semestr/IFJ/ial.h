// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Jakub Hud (xhudja00),
//				Roman Janík (xjanik20)

#ifndef IAL_H_INCLUDED
#define IAL_H_INCLUDED

//binarytree
#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "enumlist.h"
#include "generator.h"

typedef struct{
    struct level *Active;
	int Depth;
	struct level *Top;
} tBST;

typedef struct varVlCopy{
	void *value;
	struct varVlCopy *prev;
} tVarVlCopy;

typedef struct identifier{
    void *value;	// struktura hodnot, u fce ukazuje na level fce
	char *name;		// identifikátor proměnné
	TokenTypes type;		// typ proměnné
	/*
	int nParam;		// počet parametrů funkce
	TokenTypes *parametrs;	// seznam typů parametrů funkce
	*/
	bool decl;		// 0 = nedeclarován, !0 = declarován
	bool used;		// 0 = nepoužit, !0 = použit
	//struct level *LevelOfIden;// u identifieru fce ukazatel na level odpovidajici fce
	struct identifier *LPtr; //ukazatel na levy podstrom
	struct identifier *RPtr; //ukazatel na pravy podstrom
	struct varVlCopy *prevValue; //ukazatel na predchozi instanci stejneho identifieru
	struct retValues *retValues;//jen u identifieru fce
} tIdentifier;

typedef struct retValues{
	void *retVal;
	TokenTypes type;// typ navratove hodnoty
	struct SInstr *retPos;
	struct retValues *next;
}tRetValues;

typedef struct argument{
    struct identifier *Arg;
    struct argument *Next;
}tArg;

typedef struct parameters_item{
    void *Param;
    struct parameters_item *Next;
}tParams_item;

typedef struct parameters{
    tParams_item *First;
    tParams_item *Last;
}tParams;

typedef struct level{
	struct level *Parent;
	struct member *MembersList;
	struct argument *Args;
	struct SInstr *fceStart;//pozice v 3-adresnem kodu kde fce zacina
	struct identifier *TreeRoot;
}tLevel;

typedef struct member{
	struct member *Next;
	char *Name;
	struct level *NextLevel;
} tMember;

/**Prototypy funkci**/

void BSTInit(tBST*);
void BSTSetActiveTop(tBST *BST);
char* StringCopy(char*);
struct level* BSTPreviousLevel(tBST*);
struct level* BSTCreateFindLevel(tBST*, tMember*);
struct member* BSTInsertMember(tBST*, char*);
void BSTAddArgument(tBST *BST,tIdentifier *arg);
void ParamsInit(tParams *params);
void ParamsInsert(tParams *params,void *value);
void ParamsDispose(tParams *params);
int Changedottozero(char *string);
struct member* BSTSearchMember(tBST*, char*);
void BSTDisposeLevel(tBST*, struct level**);
void BSTDisposeAll(tBST*);

TokenTypes* CopyParametrs(TokenTypes [],int);
struct identifier* BSTSearchIdentifier(tBST, char*);
tIdentifier *BSTInsertIdentifier (tBST*, char*, TokenTypes, void *value,/* int, TokenTypes[],*/ bool, bool);
void TreeDispose(tIdentifier**);

//heapsort
#include "typelist.h"


/**
* Sorts string.
* @param    s		string
* @pre      s		!= NULL
* @return   s		sorted string
*/
String sort(String s);

/**
* Creates heap of string.
* @param    s		string
* @param    n		length of string
* @pre      s		!= NULL
* @pre      s		>0
*/
void heapify(String s, int n);

/**
* Sorts heap of string.
* @param    s		string
* @param    n		index
* @pre      s		!= NULL
* @pre      s		>0
*/
void heapbuild(String s, int n);

//KMP
/**
* Creates fail array for sort function.
* @param    search   string
* @pre      s 		!= NULL
* @return   fail	array of int
*/
int *kmpgraph(String search);

/**
* Finds substring in the string.
* @param    s		string
* @param    search	substring
* @pre      s		!= NULL
* @pre      search		!= NULL
* @return   index	index where substring starts in the string
*/
int find(String s, String search);

#endif //IAL_H_INCLUDED
