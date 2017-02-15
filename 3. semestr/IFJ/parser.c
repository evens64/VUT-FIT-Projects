// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Michal Hrabovský (xhrabo08)

#include "parser.h"

extern tBST BST;
extern Tqueue Q;

int tokenno = 0;
TokenTypes sym;
TokenTypes nxtsym;

tIdentifier *curfncid = NULL;

extern t_AdrList tempints;
extern t_AdrList tempdbls;
extern t_AdrList tempstrs;
extern t_AdrList literals;
extern t_AdrList strings;

void InitSymNxtSym(){
    sym = QCheckFirst(&Q);
    Ttoken *token = QPeek(&Q);
    switch (sym){
        case con_int:
        case con_double:
            token->value = AdrListInsertRaw(&literals, token->value);
            break;
        case con_string:
            token->value = AdrListInsertRaw(&strings, token->value);
            break;
        default:;
    }
    token = QPeek(&Q)->next;
    nxtsym = QCheckSecond(&Q);
    switch (nxtsym){
        case con_int:
        case con_double:
            token->value = AdrListInsertRaw(&literals, token->value);
            break;
        case con_string:
            token->value = AdrListInsertRaw(&strings, token->value);
            break;
        default:;
    }

}

void GetToken(){
    QDeleteFirst(&Q);
    Ttoken *token;
    sym = nxtsym;
    nxtsym = QCheckSecond(&Q);
    token = QPeek(&Q)->next;
    switch (nxtsym){
        case con_int:
        case con_double:
            token->value = AdrListInsertRaw(&literals, token->value);
            break;
        case con_string:
            token->value = AdrListInsertRaw(&strings, token->value);
            break;
        default:;
    }

}

Ttoken *PopToken(){
    Ttoken *res = QPopFirst(&Q);
    sym = nxtsym;
    nxtsym = QCheckSecond(&Q);

    Ttoken *token;
    token = QPeek(&Q)->next;
    switch (nxtsym){
        case con_int:
        case con_double:
            token->value = AdrListInsertRaw(&literals, token->value);
            break;
        case con_string:
            token->value = AdrListInsertRaw(&strings, token->value);
            break;
        default:;
    }

    return res;
}

int accept(TokenTypes t){
    if (nxtsym == t){
        GetToken();
        return 1;
    }
    return 0;
}

int expect(TokenTypes t){
    if (accept(t)) return 1;
    else{
    print_error(err_syn); //TODO fails
    return 1;
    }

}
int const_one = 1;

//                                == != >  <  <= >= +  -  /  *  (  ) i  $ consts
const TokenTypes Ptable[][17] = {{gt,gt,gt,gt,gt,gt,gt,gt,gt,gt,ls,gt,ls,ls,ls,ls,gt}, //==
                                {gt,gt,gt,gt,gt,gt,gt,gt,gt,gt,ls,gt,ls,ls,ls,ls,gt}, //=!
                                {ls,ls,gt,gt,gt,gt,gt,gt,gt,gt,ls,gt,ls,ls,ls,ls,gt}, //>
                                {ls,ls,gt,gt,gt,gt,gt,gt,gt,gt,ls,gt,ls,ls,ls,ls,gt}, //<
                                {ls,ls,gt,gt,gt,gt,gt,gt,gt,gt,ls,gt,ls,ls,ls,ls,gt}, //<=
                                {ls,ls,gt,gt,gt,gt,gt,gt,gt,gt,ls,gt,ls,ls,ls,ls,gt}, //>=
                                {ls,ls,gt,gt,gt,gt,gt,gt,ls,ls,ls,gt,ls,ls,ls,ls,gt}, //+
                                {ls,ls,gt,gt,gt,gt,gt,gt,ls,ls,ls,gt,ls,ls,ls,ls,gt}, //-
                                {ls,ls,gt,gt,gt,gt,ls,ls,gt,gt,ls,gt,ls,ls,ls,ls,gt}, // /
                                {ls,ls,gt,gt,gt,gt,ls,ls,gt,gt,ls,gt,ls,ls,ls,ls,gt}, //*
                                {ls,ls,ls,ls,ls,ls,ls,ls,ls,ls,ls,eq,ls,ls,ls,ls,gt}, // (
                                {gt,gt,gt,gt,gt,gt,gt,gt,gt,gt,-1,gt,-1,-1,-1,-1,gt}, // )
                                {gt,gt,gt,gt,gt,gt,gt,gt,gt,gt,-1,gt,-1,-1,-1,-1,gt}, // i
                                {gt,gt,gt,gt,gt,gt,gt,gt,gt,gt,-1,gt,-1,-1,-1,-1,gt}, //consts
                                {gt,gt,gt,gt,gt,gt,gt,gt,gt,gt,-1,gt,-1,-1,-1,-1,gt},
                                {gt,gt,gt,gt,gt,gt,gt,gt,gt,gt,-1,gt,-1,-1,-1,-1,gt},
                                {ls,ls,ls,ls,ls,ls,ls,ls,ls,ls,ls,-1,ls,ls,ls,ls,-1}};  //$

const InstrTypes opcodes [][10] =   {{eq_int,noteq_int,gt_int,ls_int,lseq_int,gteq_int,add_int,sub_int,div_int,mul_int},
                                    {eq_dbl,noteq_dbl,gt_dbl,ls_dbl,lseq_dbl,gteq_dbl,add_dbl,sub_dbl,div_dbl,mul_dbl},
                                    {-1,-1,-1,-1,-1,-1,add_str,-1,-1,-1}};

const InstrTypes builtins [] = {ifj16readIntCall, ifj16readDoubleCall, ifj16readStringCall, ifj16printCall, ifj16lengthCall, ifj16substrCall, ifj16compareCall, ifj16findCall, ifj16sortCall};

const TokenTypes builtinparameters [][5] = {{_int_,0,0,0,0},{_double_,0,0,0,0},{_String_,0,0,0,0},{_void_,0,0,0,0},
                                            {_int_,expr_string,0,0,0},{_String_,expr_string,expr_string,0,0},{_int_,expr_string,expr_string,0,0},
                                            {_int_,expr_string,expr_string,0,0},{_String_,expr_string,0,0,0}};
/*
t_SApair Pstack_pop (t_Pstack *stack){
    t_Pstack_item *popped;
    popped = stack->First;
//popping member
    stack->First = stack->First->next;

    t_SApair data = popped->sapair;
    free(popped);

    return data;
}

inline t_SApair Pstack_peek (t_Pstack stack){
    return stack.First->next->sapair;
}

void Pstack_push (t_Pstack *stack, TokenTypes token, void *ptr){
    t_Pstack_item *newitem;
    newitem=malloc(sizeof(t_Pstack_item));
    if (newitem == NULL) exit(99);
    newitem->next=stack->First;
    newitem->sapair.sym = token;
    newitem->sapair.ptr = ptr;
    stack->First = newitem;
}
*/


Ttoken *Precedence(){ //stops before invalid tkn, retuned token must be manually freed
    Tstack stack;
    SInit(&stack);
    TokenTypes input;
    SPush(&stack,endsymbol,NULL);
    Ttoken *top = STopTerminal(&stack);
    Ttoken *result = NULL;

    if(sym > con_string) input = endsymbol;
    else input = sym;

    do{
        switch(Ptable[top->type][input]){
            case gt:
                Rulepick(&stack,&top);

            break;
            case ls:
                if (SPeek(&stack)->type>endsymbol) SPushBehind(&stack,semicolon,NULL);
                else  SPush(&stack,semicolon,NULL);

                if((top->type != lparenth && nxtsym == rparenth)||nxtsym > con_string){
                    result = malloc(sizeof(Ttoken));
                    QSoftCopyFirst(&Q,result);
                    SPushRaw(&stack, result);
                    input = endsymbol;
                }else{
                    input = nxtsym;
                    SPushRaw(&stack, PopToken());
                }
                top = stack.First;

            break;
            case eq:
                SPushRaw(&stack, PopToken(&Q));

                if((top->type != lparenth && nxtsym == rparenth)||nxtsym > con_string){
                    result = malloc(sizeof(Ttoken));
                    QSoftCopyFirst(&Q,result);
                    SPushRaw(&stack, result);
                    input = endsymbol;
                }else{
                    input = nxtsym;
                    SPushRaw(&stack, PopToken());
                }
                top = stack.First;

            break;
            case -1:
                print_error(err_syn);
            break;
            default:
            break;
        }
    } while (top->type != endsymbol || input != endsymbol);
    result = SPop(&stack);
    Sdispose(&stack);
    return result;
}

void SemAnalId2Expr(Ttoken *token){
    tIdentifier *ident;
    switch (token->type) {
        case id:
            ident = SemAnalCheckVarUsed(&BST,token);
            if (ident->type == _int_) {
                token->value = AdrListGetAvailable(&tempints);
                if (token->value == NULL) token->value = AdrListNewInt(&tempints);
                token->type = expr_int;
                generate(ass_int, ident->value, NULL, ((t_AdrList_Item*)token->value)->ptr);
            }
            if (ident->type == _double_){
                token->value = AdrListGetAvailable(&tempdbls);
                if (token->value == NULL) token->value = AdrListNewDbl(&tempdbls);
                token->type = expr_dbl;
                generate(ass_dbl, ident->value, NULL, ((t_AdrList_Item*)token->value)->ptr);
            }
            if (ident->type == _String_){
                token->value = AdrListGetAvailable(&tempstrs);
                if (token->value == NULL) token->value = AdrListNewString(&tempstrs);
                token->type = expr_string;
                generate(ass_str, ident->value, NULL, ((t_AdrList_Item*)token->value)->ptr);
            }
        	break;
        case con_int:
            token->type = expr_int;
            //ptr = AdrListGetAvailable(&tempints);
            //if (ptr == NULL) ptr = AdrListNewInt(&tempints);
            //generate(ass_int, token->value, NULL, ptr);
        	break;
        case con_double:
            token->type = expr_dbl;
            //ptr = AdrListGetAvailable(&tempints);
            //if (ptr == NULL) ptr = AdrListNewDbl(&tempdbls);
            //generate(ass_dbl, token->value, NULL, ptr);
        	break;
        case con_string:
            token->type = expr_string;
            //ptr = AdrListGetAvailable(&tempstrs);
            //if (ptr == NULL) ptr = AdrListNewString(&tempstrs);
            //generate(ass_str, token->value, NULL, ptr);
        	break;
        default:
            print_error(err_sem_kom);
            return;
    }
}

void SemAnalCvtInt2Dbl(Ttoken *tkn){
        t_AdrList_Item *pom;
        pom = AdrListGetAvailable(&tempdbls);
        if (pom == NULL) pom = AdrListNewDbl(&tempdbls);
        generate(cvt_int2dbl,((t_AdrList_Item*)tkn->value)->ptr,NULL,pom->ptr);
        AdrListMakeAvailable(tkn->value);
        tkn->type=expr_dbl;
        tkn->value=pom;
}

void SemAnalCvtDbl2Int(Ttoken *tkn){
        t_AdrList_Item *pom;
        pom = AdrListGetAvailable(&tempints);
        if (pom == NULL) pom = AdrListNewInt(&tempints);
        generate(cvt_dbl2int,((t_AdrList_Item*)tkn->value)->ptr,NULL,pom->ptr);
        AdrListMakeAvailable(tkn->value);
        tkn->type=expr_dbl;
        tkn->value = pom;
}

void SemAnalCvtTerm2Value(Ttoken *tkn){
                if (tkn->type == id){
                    tIdentifier *tempid = SemAnalCheckVarUsed(&BST,tkn);
                    tkn->value = tempid->value;
                    tkn->type = tempid->type;
                }
                else{
                    switch (tkn->type){
                        case con_int:
                            tkn->type = _int_;
                            tkn->value = ((t_AdrList_Item*)tkn->value)->ptr;
                        break;
                        case con_double:
                            tkn->type = _double_;
                            tkn->value = ((t_AdrList_Item*)tkn->value)->ptr;
                        break;
                        case con_string:
                            tkn->type = _String_;
                            tkn->value = ((t_AdrList_Item*)tkn->value)->ptr;
                        break;
                        default:
                            print_error(err_syn);
                    }
                }
}

void SemAnalExprOpExpr(Ttoken *operand1,Ttoken *opcode, Ttoken *operand2){
    int optype =-1;
    t_AdrList_Item *rslt;
    if(operand1->type == expr_int){
        if(operand2->type == expr_string) print_error(err_sem_kom);
        if(operand2->type == expr_dbl){
            SemAnalCvtInt2Dbl(operand1);
            AdrListMakeAvailable(operand1->value);
            AdrListMakeAvailable(operand2->value);
            rslt = AdrListGetAvailable(&tempdbls);
            if (rslt == NULL) rslt = AdrListNewDbl(&tempdbls);
            optype = 1;
        }
        else{
            AdrListMakeAvailable(operand1->value);
            AdrListMakeAvailable(operand2->value);
            rslt = AdrListGetAvailable(&tempints);
            if (rslt == NULL) rslt = AdrListNewInt(&tempints);
            optype = 0;
        }
    }
    if(operand1->type == expr_dbl){
        if(operand2->type == expr_string) print_error(err_sem_kom);
        if(operand2->type == expr_int) {
            SemAnalCvtInt2Dbl(operand2);
        }
        AdrListMakeAvailable(operand1->value);
        AdrListMakeAvailable(operand2->value);
        rslt = AdrListGetAvailable(&tempdbls);
        if (rslt == NULL) rslt = AdrListNewDbl(&tempdbls);
        optype = 1;
    }
    if(operand1->type == expr_string){
        if(operand2->type != expr_string) print_error(err_sem_kom);
        if(opcode->type != add) print_error(err_sem_kom);
        AdrListMakeAvailable(operand1->value);
        AdrListMakeAvailable(operand2->value);
        rslt = AdrListGetAvailable(&tempstrs);
        if (rslt == NULL) rslt = AdrListNewString(&tempstrs);
        optype = 2;
    }
    generate(opcodes[optype][opcode->type], ((t_AdrList_Item*)(operand1->value))->ptr, ((t_AdrList_Item*)(operand2->value))->ptr, rslt->ptr);
    operand1->value = rslt;
}

void SemAnalClassEnter(tBST *BST,Ttoken *token){
    if (strcmp((char*)token->value,"ifj16")==0) print_error(err_sem_und);
    BSTInsertMember(BST,(char*)(token->value));
    tMember *member = BSTSearchMember(BST,(char*)(token->value));
    BSTCreateFindLevel(BST,member);
}

void SemAnalExistingClassEnter (tBST *BST,Ttoken *token){
    tMember *member = BSTSearchMember(BST,(char*)(token->value));
    BSTCreateFindLevel(BST,member);
}

void SemAnalFncDef(tBST *BST, Ttoken *token, TokenTypes returntype){ //volána s Ttoken obsahujícím id funkce
    tIdentifier *ident = BSTSearchIdentifier(*BST, token->value);
    if (ident == NULL){
        BSTInsertMember(BST,(char*)(token->value));
        tMember *member = BSTSearchMember(BST,(char*)(token->value));
        BSTCreateFindLevel(BST,member);
        BSTPreviousLevel(BST);
        BSTInsertIdentifier(BST,token->value,returntype,member->NextLevel,1,0);
        BSTCreateFindLevel(BST,member);
    } else {
        print_error(err_sem_und);
    }
}

tIdentifier * SemAnalExistingFncDef(tBST *BST, Ttoken *token){
    tIdentifier *ident = BSTSearchIdentifier(*BST, token->value);
    if (ident == NULL){
        print_error(err_sem_und);
    } else {
        tMember *member = BSTSearchMember(BST,(char*)(token->value));
        BSTCreateFindLevel(BST,member);
    }
    return ident;
}

tIdentifier *SemAnalCheckFncCalled(tBST *BST, Ttoken *token){ //volána s Ttoken obsahujícím id funkce
    tIdentifier *ident = BSTSearchIdentifier(*BST, token->value);
    if (ident == NULL){
        ident = BSTInsertIdentifier(BST,token->value,_void_,NULL,0,1);
    }
    if (ident->decl == 0) print_error(err_sem_und);
    ident->used = 1;
    return ident;
}

void SemAnalAddArg (tBST *BST, Ttoken *token, TokenTypes argtype){
    tIdentifier *id;
    id = BSTSearchIdentifier(*BST,token->value);
    if(id == NULL){
        void * value;
        if (argtype == _int_) value = malloc(sizeof(int));
        else if (argtype == _double_) value = malloc(sizeof(double));
        else if (argtype == _String_) {
            value = malloc(sizeof(char**));
            *(char**)value = NULL;
        }
        else print_error(err_sem_kom);
        id = BSTInsertIdentifier(BST,token->value,argtype,value,1,0);
        BSTAddArgument(BST,id);
    } else {
        print_error(err_sem_und);
    }
}

void SemAnalFncCall(tBST *BST,Ttoken *fnctkn,Tstack *params,tIdentifier *ident){
    tParams paramstruct;
    ParamsInit(&paramstruct);
    tArg *temp2;
    Ttoken *tkn;
    tIdentifier *functionid = SemAnalCheckFncCalled(BST,fnctkn);
    void *hlp = functionid->value;
    temp2 = ((tLevel*)hlp)->Args;
    while (temp2 != NULL){
        tkn = SPeek(params);
        if (tkn == NULL) print_error(err_sem_kom);
        switch(tkn->type){
            case expr_int:
                if (temp2->Arg->type != _int_) print_error(err_sem_kom);
            break;
            case expr_dbl:
                if (temp2->Arg->type != _double_) print_error(err_sem_kom);
            break;
            case expr_string:
                if (temp2->Arg->type != _String_) print_error(err_sem_kom);
            break;
            default:
            print_error(err_sem_kom);
        }
        ParamsInsert(&paramstruct,((t_AdrList_Item*)tkn->value)->ptr);
        SPopDelete(params);
        temp2 = temp2->Next;
    }
    if(SPeek(params)!= NULL) print_error(err_sem_kom);
    if(ident != NULL){
		if (ident->type != functionid->type) print_error(err_sem_kom);
		generate(fce_call, functionid, paramstruct.First, ident->value);
		AdrListMakeAvailableArgs(paramstruct.First);
	} else {
		generate(fce_call, functionid, paramstruct.First, NULL);
		AdrListMakeAvailableArgs(paramstruct.First);
	}
	//Generate(fnc_call,functionid,paramstruct.First,NULL);
	Sdispose(params);
}

void SemAnalBuiltinFncCall(Ttoken *fnctkn,Tstack *params,tIdentifier *ident){
    tParams paramstruct;
    ParamsInit(&paramstruct);
    Ttoken *tkn = SPeek(params);
    int i = 1;
    while (tkn != NULL){
        if (builtinparameters[fnctkn->type - _ifj16readInt_][i] != tkn->type) print_error(err_sem_kom);
        ParamsInsert(&paramstruct,((t_AdrList_Item*)tkn->value)->ptr);
        AdrListMakeAvailable((t_AdrList_Item*)tkn->value);
        SPopDelete(params);
        tkn = SPeek(params);
        i++;
    }
    if(builtinparameters[fnctkn->type - _ifj16readInt_][i] != 0) print_error(err_sem_kom);
    if (ident != NULL){
        if (ident->type != builtinparameters[fnctkn->type - _ifj16readInt_][0]) print_error(err_sem_kom);
		generate(builtins[fnctkn->type-_ifj16readInt_], NULL, paramstruct.First, ident->value);
		AdrListMakeAvailableArgs(paramstruct.First);
    } else {
		generate(builtins[fnctkn->type-_ifj16readInt_], NULL, paramstruct.First, NULL);
		AdrListMakeAvailableArgs(paramstruct.First);
    }
    Sdispose(params);
}

tIdentifier *SemAnalVarDec(tBST *BST, Ttoken *token, TokenTypes type){
    tIdentifier *ident = BSTSearchIdentifier(*BST, token->value);
    if (ident == NULL){
        void * value;
        if (type == _int_) value = malloc(sizeof(int));
        else if (type == _double_) value = malloc(sizeof(double));
        else if (type == _String_) {
            value = malloc(sizeof(char**));
            *(char**)value = NULL;
        }
        else print_error(err_sem_kom);
        ident = BSTInsertIdentifier(BST,token->value,type,value,1,0);
        free(token->value);
        token->value = NULL;
    } else{
        print_error(err_sem_und);
    }
    return ident;
}

tIdentifier *SemAnalCheckVarUsed (tBST *BST, Ttoken *token){
    tIdentifier *id;
    id = BSTSearchIdentifier(*BST,token->value);
    if(id == NULL){
        id = BSTInsertIdentifier(BST,token->value,-1,NULL,0,1);
    }
    if (id->decl == 0) print_error(err_sem_und);
    id->used = 1;
    free(token->value);
    token->value = NULL;
    return id;
}

void SemAnalAssignVar(tIdentifier *id,Ttoken *t){
    if(id->type == _String_){
        if(t->type != expr_string) print_error(err_sem_kom);
        generate(ass_str, ((t_AdrList_Item*)t->value)->ptr, NULL, id->value);
        AdrListMakeAvailable(t->value);
    }
    if(id->type == _int_){
        if(t->type == expr_string) print_error(err_sem_kom);
        void *intermediate;
        if(t->type == expr_dbl) {
            intermediate = AdrListGetAvailable(&tempints);
            if (intermediate == NULL) intermediate = AdrListNewInt(&tempints);
            AdrListMakeAvailable(t->value);
            generate(cvt_dbl2int, ((t_AdrList_Item*)t->value)->ptr, NULL, ((t_AdrList_Item*)intermediate)->ptr);
        }
        else intermediate = t->value;
        generate(ass_int, ((t_AdrList_Item*)intermediate)->ptr, NULL, id->value);
        AdrListMakeAvailable(intermediate);
    }
    if(id->type == _double_){
        if(t->type == expr_string) print_error(err_sem_kom);
        void *intermediate;
        if(t->type == expr_int) {
            intermediate = AdrListGetAvailable(&tempdbls);
            if (intermediate == NULL) intermediate = AdrListNewDbl(&tempdbls);
            AdrListMakeAvailable(t->value);
            generate(cvt_int2dbl, ((t_AdrList_Item*)t->value)->ptr, NULL, ((t_AdrList_Item*)intermediate)->ptr);
        }
        else intermediate = t->value;
        generate(ass_dbl, ((t_AdrList_Item*)intermediate)->ptr, NULL, id->value);
        AdrListMakeAvailable(intermediate);
    }
}
void SemAnalIf(Ttoken *t){
    generate(if_start, ((t_AdrList_Item*)t->value)->ptr, NULL, NULL);
    AdrListMakeAvailable(t->value);
}

void SemAnalWhile(Ttoken *t){
    generate(while_condition, ((t_AdrList_Item*)t->value)->ptr, NULL, NULL);
    AdrListMakeAvailable(t->value);
}

void SemAnalReturn(Ttoken *t){
    if (t != NULL){
         switch(t->type){
            case expr_int:
                if (curfncid->type != _int_) print_error(err_sem_kom);
            break;
            case expr_dbl:
                if (curfncid->type != _double_) print_error(err_sem_kom);
            break;
            case expr_string:
                if (curfncid->type != _String_) print_error(err_sem_kom);
            break;
            default:
            print_error(err_sem_kom);
        }
        generate(return_val, curfncid, ((t_AdrList_Item*)t->value)->ptr, NULL);
        AdrListMakeAvailable(t->value);
    }
    else generate(return_val, curfncid, NULL, NULL);
}

void Rulepick(Tstack *stack, Ttoken **top){
    Ttoken *operand1;
    Ttoken *operand2;
    Ttoken *opcode;
    switch ((*top)->type){
        case id:
        case con_int:
        case con_double:
        case con_string:
            operand1 = SPop(stack);
            opcode = SPop(stack);
            if (opcode->type == semicolon){
                SemAnalId2Expr(operand1);
                *top = STopTerminal(stack);
                SPushRaw(stack,operand1);
                free(opcode);
                return;
            }
            print_error(err_syn);

        break;
        case lparenth:
            SPopDelete(stack);
            operand1 = SPop(stack);
            if (operand1->type <= expr_string && operand1->type >= expr_int){
                if(SPeek(stack)->type == rparenth){
                    SPopDelete(stack);
                    if (SPeek(stack)->type == semicolon){
                        SPopDelete(stack);
                        *top = STopTerminal(stack);
                        SPushRaw(stack,operand1);
                        return;
                    }
                }
            }
            print_error(err_lex);
        break;
        default:
            if((*top)->type<=mul){
                operand2 = SPop(stack);
                if (operand2->type <= expr_string && operand2->type >= expr_int){
                    opcode = SPop(stack);
                    if (opcode == *top){
                        operand1 = SPop(stack);
                        if (SPeek(stack)->type == semicolon){
                            SPopDelete(stack);
                            SemAnalExprOpExpr(operand1,opcode,operand2);
                            *top = STopTerminal(stack);
                            SPushRaw(stack,operand1);
                            free(operand2);
                            free(opcode);
                            return;
                        }
                    }
                }
            }
            print_error(err_syn);
            break;
            print_error(err_syn);
    }
}

TokenTypes accept_datatype(){
    if (accept(_int_)) return _int_;
    if (accept(_String_)) return _String_;
    if (accept(_double_)) return _double_;
    if (accept(_void_)) return _void_;
    return 0;
}

TokenTypes expect_datatype(){
    if (accept(_int_)) return _int_;
    if (accept(_String_)) return _String_;
    if (accept(_double_)) return _double_;
    if (accept(_void_)) return _void_;
    print_error(err_syn); //TODO fails
    return _void_;
}

//-----------------------------

void body_f(){ //covers main body, classes, and static definitions inside classes
    InitSymNxtSym();
    BSTSetActiveTop(&BST);
    while (accept(_class_)){ //encountered class token, expects id, parenthesis,and static defintiion
        class_f();
    }
    expect(_EOF_);
}

void class_f(){
    expect(id);
    //SemAnalClassEnter(&BST,QPeek(&Q)); moved to lex
    SemAnalExistingClassEnter(&BST,QPeek(&Q));
    expect(lbraces);
    TokenTypes datatype = 0;
    while(accept(_static_)){//encountered definition inside class
        datatype = accept_datatype();
        if(datatype == 5){//is void function definition
            expect(id);
            curfncid = SemAnalExistingFncDef(&BST,QPeek(&Q)); //curfnid je global
            expect(lparenth);
            function_f();
        }
        else if(datatype > 0){
            expect(id);
            if (nxtsym == ass){//is definition of static variable with assigment
                tIdentifier *id = SemAnalVarDec(&BST,QPeek(&Q),datatype);
                GetToken();
                GetToken();
                Ttoken *t = Precedence();
                SemAnalAssignVar(id,t);
                free(t);
                expect(semicolon);
            }
            else if (nxtsym == lparenth){//is non-void function definition
                curfncid = SemAnalExistingFncDef(&BST,QPeek(&Q)); //curfnid je global
                GetToken();
                function_f();}
            else { //is definition of static variable
                SemAnalVarDec(&BST,QPeek(&Q),datatype);
                expect(semicolon);
            }
        }
        else {} //TODO error, no datatype after static
    }
    expect(rbraces);
    BSTPreviousLevel(&BST);
}


void function_f(){
    //parameters
    TokenTypes datatype = 0;
    do{
        datatype = accept_datatype();
        if(datatype){
            expect(id);
            //SemAnalAddArg(&BST,QPeek(&Q),datatype); moved to lex
        }
    }while(accept(column));

    expect(rparenth);
    expect(lbraces);
	generate(fce_start, curfncid, NULL, NULL);
	while (!accept(rbraces)) statement_f();
	generate(fce_end, curfncid, NULL, NULL);
    BSTPreviousLevel(&BST);
}

void function_call_f(tIdentifier *ident){ //called with lparenth on sym
    Ttoken *fnctkn = PopToken(&Q);
    if (fnctkn->type == id){
        Tstack params;
        SInit(&params);

        if(!accept(rparenth)){
        GetToken();
            SPushRaw(&params,Precedence());
            while (!accept(rparenth)){
                expect(column);
                GetToken();
                SPushRaw(&params,Precedence());
            }
        }
        SemAnalFncCall(&BST,fnctkn,&params,ident);
    }
    else if(fnctkn->type != _ifj16print_){
        Tstack params;
        SInit(&params);
        if(!accept(rparenth)){
            GetToken();
            SPushRaw(&params,Precedence());
            while (!accept(rparenth)){
                expect(column);
                GetToken();
                SPushRaw(&params,Precedence());
            }
        }
        SemAnalBuiltinFncCall(fnctkn,&params,ident);
    } else {
        Tqueue params;
        QInit(&params);
        if(!accept(rparenth)){
            GetToken();
            SemAnalCvtTerm2Value(QPeek(&Q));
            QLastInsertRaw(&params,PopToken());
            while (sym != rparenth){
                if (sym != add) print_error(err_syn);
                GetToken();
                SemAnalCvtTerm2Value(QPeek(&Q));
                QLastInsertRaw(&params,PopToken());
            }
        generate(ifj16printCall,NULL,params.First,NULL);
        }
    }
    free(fnctkn);
}

void block_f(){
    while(!accept(rbraces)){
        statement_f();
    }
}

void statement_f(){
    TokenTypes datatype = 0;
    if (accept(id)){
        if(nxtsym == lparenth){
            function_call_f(NULL);
            expect(semicolon);
        } else {
            tIdentifier * ident;
            ident = SemAnalCheckVarUsed(&BST,QPeek(&Q));
            if (accept(ass)){
                GetToken();
                if (nxtsym == lparenth && (sym == id || (sym >= _ifj16readInt_ && sym <= _ifj16sort_))){
                    function_call_f(ident);
                } else {
                    Ttoken *t;
                    t = Precedence();
                    SemAnalAssignVar(ident,t);
                    free (t);
                }
            }
            else print_error(err_syn);
            expect(semicolon);
        }
    }
    else if (nxtsym >= _ifj16readInt_ && nxtsym <= _ifj16sort_){
        GetToken();
        if (nxtsym == lparenth) function_call_f(NULL);
        else print_error(err_syn);
        expect(semicolon);
    }
    else if((datatype = accept_datatype())){
        expect(id);
        if (nxtsym == ass){//is definition of static variable with assigment
            tIdentifier *ident = SemAnalVarDec(&BST,QPeek(&Q),datatype);
            GetToken();
            GetToken();
            if (nxtsym == lparenth){
                function_call_f(ident);
            }
            else{
                Ttoken *t = Precedence();
                SemAnalAssignVar(ident,t);
                free (t);
            }
        }
        else{
            SemAnalVarDec(&BST,QPeek(&Q),datatype);
        }
        expect(semicolon);
    }
    else if(accept(_if_)) if_f();
    else if(accept(_while_)) while_f();
    else if(accept(_return_)) {
            return_f();
            expect(semicolon);
    }
    else if (accept(lbraces)){
        block_f();
    }
    else {print_error(err_syn);}
}

void if_f(){
    expect(lparenth);
    GetToken();
    Ttoken *expr = Precedence();
    expect(rparenth);
    expect(lbraces);

    SemAnalIf(expr);

    free (expr);
    block_f();
    generate(if_end, NULL, NULL, NULL);
    expect(_else_);
    generate(else_start, NULL, NULL, NULL);
    expect (lbraces);
    block_f();
    generate(else_end, NULL, NULL, NULL);
}
void while_f(){
    expect(lparenth);
    GetToken();
    generate(while_start, NULL, NULL, NULL);
    Ttoken *expr = Precedence();
    expect(rparenth);

    SemAnalWhile(expr);

    free (expr);
    expect(lbraces);
    block_f();
    generate(while_end, NULL, NULL, NULL);
}

void return_f(){
    if (curfncid->type != _void_){
    Ttoken *expr;
    GetToken();
    expr = Precedence();
    SemAnalReturn(expr);
    free(expr);
    }
    else SemAnalReturn(NULL);
}




