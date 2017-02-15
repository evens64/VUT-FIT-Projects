// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Michal Hrabovský (xhrabo08)

#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "ial.h"
#include "enumlist.h"
#include "generator.h"
#include "queue.h"

// to do

/*
typedef struct SApair{
    void *ptr;
    TokenTypes sym;
}   t_SApair;

typedef struct Pstack_item{
    t_SApair sapair;
    struct Pstack_item *next;
} t_Pstack_item;

typedef struct Pstack{
    struct Pstack_item *First;
} t_Pstack;
*/

void GetToken();
int accept(TokenTypes t);
int expect(TokenTypes t);
int accept_datatype();
int expect_datatype();
Ttoken *Precedence();

void SemAnalId2Expr(Ttoken *token);
void SemAnalCvtDbl2Int(Ttoken *tkn);
void SemAnalCvtInt2Dbl(Ttoken *tkn);
void SemAnalExprOpExpr(Ttoken *operand1,Ttoken *opcode, Ttoken *operand2);
void SemAnalClassEnter(tBST *BST,Ttoken *token);
void SemAnalFncDef(tBST *BST, Ttoken *token, TokenTypes returntype);
void SemAnalAddArg (tBST *BST, Ttoken *token, TokenTypes argtype);
void SemAnalFncCall();
tIdentifier *SemAnalCheckVarUsed (tBST *BST, Ttoken *token);
void SemAnalAssignVar(tIdentifier *id,Ttoken *t);
void SemAnalIf(Ttoken *t);
void SemAnalWhile(Ttoken *t);

void Rulepick(Tstack *stack, Ttoken **top);
void body_f();
void class_f();
void function_f();
void block_f();
void statement_f();
void if_f();
void while_f();
void return_f();
void function_call();
void expression_f();

#endif //PARSER_H_INCLUDED
