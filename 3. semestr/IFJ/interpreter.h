// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Jakub Hud (xhudja00)

#ifndef INTERPRETER_H_INCLUDED
#define INTERPRETER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "enumlist.h"
#include "error.h"
#include "builtinfce.h"
#include "generator.h"
#include "typelist.h"

typedef struct SPara{
	void *argPtr;
	struct SPara *next;
} TPara;

void interprete(tBST*);
void fceLocVarCopy(struct identifier*);
void argCopy(struct identifier*, struct parameters_item*);
void setRetPosAlloc(struct identifier*, struct SInstr*);
struct SInstr* JumpInFce(struct identifier*);
struct SInstr* JumpOutFce(struct identifier*);
void fceLocVarClean(struct identifier *);
void ReturnClean(struct identifier*, void*);
void IdenAllocOrCleanAll(struct identifier*, tIdentifier* fncident, char);
void globalVarHandle(tBST*);

#endif //INTERPRETER_H_INCLUDED
