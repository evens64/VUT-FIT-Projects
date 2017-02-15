// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Jakub Frýz (xfryzj01),
//				Michal Hrabovský (xhrabo08)

#include "queue.h"

void QInit(Tqueue *Q) {		//inicializace fronty
	Q->First = NULL;
	Q->Last = NULL;
}
/*
void QLastInsert_template(Tqueue *Q, TokenTypes t, int i, double d, char* s) {
	assert(Q->First == NULL && Q->Last != NULL);
	assert(Q->First != NULL && Q->Last == NULL);

	Ttoken *Helper = malloc(sizeof(struct Stoken));
	if (Helper == NULL) {				//test alokace
		print_error(err_oth);
	} else {
		Helper->next = NULL;			//"ukazatel" na další prvek

		Helper->type = t;				//vložení typu
		Helper->values.i = i;			//vložení 'int' hodnoty
		Helper->values.d = d;			//vložení 'double' hodnoty
		Helper->values.s = s;			//vložení 'string' hodnoty

		if (Q->First == NULL && Q->Last == NULL) {			//pokud je fronta prázdná
			Q->First = Helper;
			Q->Last = Helper;
		} else {	//pokud fronta není prázdná
			Q->Last->next = Helper;
			Q->Last = Helper;
		}
	}
}
*/
//--

void QLastInsert(Tqueue *Q, TokenTypes t) {		//vložení tokenu (např. int, {, ...)
	assert(t != con_int || t != con_double || t != con_string);
	assert(!(Q->First == NULL && Q->Last != NULL));
	assert(!(Q->First != NULL && Q->Last == NULL));

	Ttoken *Helper = malloc(sizeof(struct Stoken));
	if (Helper == NULL) print_error(err_oth);			//test alokace

	Helper->next = NULL;								//"ukazatel" na další prvek
	Helper->type = t;									//vložení typu
	Helper->value = NULL;

	if (Q->First == NULL && Q->Last == NULL) {			//pokud je fronta prázdná
		Q->First = Helper;
		Q->Last = Helper;
	} else {											//pokud fronta není prázdná
		Q->Last->next = Helper;
		Q->Last = Helper;
	}
}	//vyvolání fce


void QLastInsertInt(Tqueue *Q, int i) {					//vložení tokenu s hodnotou 'int'
	assert(!(Q->First == NULL && Q->Last != NULL));
	assert(!(Q->First != NULL && Q->Last == NULL));

	Ttoken *Helper = malloc(sizeof(struct Stoken));
	if (Helper == NULL) print_error(err_oth);			//test alokace

	Helper->next = NULL;								//"ukazatel" na další prvek
	Helper->type = con_int;									//vložení typu
	Helper->value = malloc(sizeof(int));
	if (Helper->value == NULL) print_error(err_oth);
	*(int*)(Helper->value) = i;

	if (Q->First == NULL && Q->Last == NULL) {			//pokud je fronta prázdná
		Q->First = Helper;
		Q->Last = Helper;
	} else {											//pokud fronta není prázdná
		Q->Last->next = Helper;
		Q->Last = Helper;
	}
}


void QLastInsertDouble(Tqueue *Q, double d) {	//vložení tokenu s hodnotou 'double'
	assert(!(Q->First == NULL && Q->Last != NULL));
	assert(!(Q->First != NULL && Q->Last == NULL));

	Ttoken *Helper = malloc(sizeof(struct Stoken));
	if (Helper == NULL) print_error(err_oth);			//test alokace

	Helper->next = NULL;								//"ukazatel" na další prvek
	Helper->type = con_double;									//vložení typu
	Helper->value = malloc(sizeof(double));
	if (Helper->value == NULL) print_error(err_oth);
	*(double*)(Helper->value) = d;

	if (Q->First == NULL && Q->Last == NULL) {			//pokud je fronta prázdná
		Q->First = Helper;
		Q->Last = Helper;
	} else {											//pokud fronta není prázdná
		Q->Last->next = Helper;
		Q->Last = Helper;
	}
}		//vyvolání fce


void QLastInsertString(Tqueue *Q, char* s) {	//vložení tokenu s hodnotou 'string'
	assert(!(Q->First == NULL && Q->Last != NULL));
	assert(!(Q->First != NULL && Q->Last == NULL));

	Ttoken *Helper = malloc(sizeof(struct Stoken));
	if (Helper == NULL) print_error(err_oth);			//test alokace

	Helper->next = NULL;								//"ukazatel" na další prvek
	Helper->type = con_string;

	int i = 0;										  //hardcopy stringu
	while(s[i] != 0){
		i++;
	}
	Helper->value = malloc(sizeof(void*));
	if (Helper->value == NULL) print_error(err_oth);
	*(char**)Helper->value = malloc(sizeof(char) * (i + 1));
	if (*(char**)Helper->value == NULL) print_error(err_oth);
	for(; i >= 0; i--){
		(*(char**)(Helper->value))[i] = s[i];
	}

	if (Q->First == NULL && Q->Last == NULL) {			//pokud je fronta prázdná
		Q->First = Helper;
		Q->Last = Helper;
	} else {											//pokud fronta není prázdná
		Q->Last->next = Helper;
		Q->Last = Helper;
	}
		//vyvolání fce
}

void QLastInsertID(Tqueue *Q, char* s) {	//vložení tokenu s hodnotou 'string'
	assert(!(Q->First == NULL && Q->Last != NULL));
	assert(!(Q->First != NULL && Q->Last == NULL));

	Ttoken *Helper = malloc(sizeof(struct Stoken));
	if (Helper == NULL) print_error(err_oth);			//test alokace

	Helper->next = NULL;								//"ukazatel" na další prvek
	Helper->type = id;

	int i = 0;										  //hardcopy stringu
	while(s[i] != 0){
		i++;
	}
	Helper->value = malloc(sizeof(char) * (i + 1));
	if (Helper->value == NULL) print_error(err_oth);
	for(; i >= 0; i--){
		((char*)(Helper->value))[i] = s[i];
	}

	if (Q->First == NULL && Q->Last == NULL) {			//pokud je fronta prázdná
		Q->First = Helper;
		Q->Last = Helper;
	} else {											//pokud fronta není prázdná
		Q->Last->next = Helper;
		Q->Last = Helper;
	}
		//vyvolání fce
}

void QLastInsertRaw(Tqueue* Q,Ttoken *token){
	assert(!(Q->First == NULL && Q->Last != NULL));
	assert(!(Q->First != NULL && Q->Last == NULL));
    token->next = NULL;
    if (Q->First == NULL && Q->Last == NULL) {			//pokud je fronta prázdná
        Q->First = token;
        Q->Last = token;
    } else {	//pokud fronta není prázdná
        Q->Last->next = token;
        Q->Last = token;
    }
}

//--

void QDeleteFirst(Tqueue *Q) {
	if (Q->First != NULL) {		//pokud fronta není prázdná
		Ttoken *Helper = Q->First;		//pomocný ukazatel
		if (Q->First == Q->Last) {		//pokud je ve frontě jen jeden prvek
			Q->First = NULL;
			Q->Last = NULL;
		} else {						//pokud je ve frontě víc jak jeden prvek
			Q->First = Helper->next;
		}
		free(Helper);		//uvolnění prvku
	}
}

Ttoken *QPopFirst(Tqueue *Q) {
	if (Q->First != NULL) {		//pokud fronta není prázdná
		Ttoken *Helper = Q->First;		//pomocný ukazatel
		if (Q->First == Q->Last) {		//pokud je ve frontě jen jeden prvek
			Q->First = NULL;
			Q->Last = NULL;
		} else {						//pokud je ve frontě víc jak jeden prvek
			Q->First = Helper->next;
		}
		return Helper;
	}
	return NULL;
}

Ttoken *QPeek(Tqueue *Q) {
	return Q->First;
}

Ttoken *QPeekLast(Tqueue *Q){
    return Q->Last;
}

TokenTypes QCheckFirst(Tqueue *Q) {
	if (Q->First != NULL) {		//pokud fronta není prázdná
		return Q->First->type;
	} else {
		print_error(err_oth);
		return _EOF_;			//zamezení varování při překladu (mrtvý kód)
	}
}
TokenTypes QCheckSecond(Tqueue *Q) {
	if (Q->First != NULL && Q->First->next != NULL) {		//pokud fronta není prázdná
		return Q->First->next->type;
	} else {
		print_error(err_oth);
		return _EOF_;			//zamezení varování při překladu (mrtvý kód)
	}
}

void QCopyFirstInt(Tqueue *Q, int *i) {
	if (Q->First != NULL) {		//pokud fronta není prázdná
		*i = *((int*)(Q->First->value));
	} else {
		print_error(err_oth);
	}
}

void QCopyFirstDouble(Tqueue *Q, double *d) {
	if (Q->First != NULL) {		//pokud fronta není prázdná
		*d = *(double*)(Q->First->value);
	} else {
		print_error(err_oth);
	}
}

void QCopyFirstString(Tqueue *Q, char** s) {
	if (Q->First != NULL) {		//pokud fronta není prázdná
		*s = (char*)(Q->First->value);
	} else {
		print_error(err_oth);
	}
}

void QSoftCopyFirst(Tqueue *Q, Ttoken *t){
    t->type = Q->First->type;
    t->value = Q->First->value;
}
//--

inline void SInit(Tstack *S){
	S->First = NULL;
}

void SPush(Tstack *S, TokenTypes sym, void* adr){
   Ttoken *token;
   token = malloc(sizeof(Ttoken));
   if (token == NULL) print_error(err_oth);
   token->type = sym;				//vložení typu
   token->next = S->First;
   token->value = adr;
   S->First = token;
}

void SPushBehind(Tstack *S, TokenTypes sym, void* adr){
   Ttoken *token;
   token = malloc(sizeof(Ttoken));
   if (token == NULL) print_error(err_oth);
   token->type = sym;				//vložení typu
   token->next = S->First->next;
   token->value = adr;
   S->First->next = token;
}

void SPushRaw(Tstack *S, Ttoken *token){
	token->next=S->First;
	S->First = token;
}

void SPushBehindRaw(Tstack *S, Ttoken *token){
	token->next=S->First->next;
	S->First->next = token;
}

Ttoken *SPop(Tstack *S){
	if(S->First != NULL){
	Ttoken *token = S->First;
	S->First = S->First->next;
	return token;
	}
	else return NULL;
}
void SPopDelete(Tstack *S){
	Ttoken *token = S->First;
	S->First = S->First->next;
	if (token->type == id) free(token->value);
	free(token);
}
Ttoken *SPeek(Tstack *S){
	return S->First;
}
void Sdispose(Tstack *S){
	while (S->First != NULL){
	Ttoken *token = S->First;
	S->First = S->First->next;
	free(token);
	}
}

Ttoken *STopTerminal(Tstack *S){
	Ttoken *cur = S->First;
	while (cur->type > endsymbol) cur = cur->next;
	return cur;
}

void AdrListInit(t_AdrList* lst){
	lst->First=NULL;
}
void *AdrListNewInt(t_AdrList* lst){
	t_AdrList_Item *pom;
	pom = lst->First;
	lst->First = malloc(sizeof(t_AdrList_Item));
	if (lst->First == NULL) print_error(err_run_oth);
	lst->First->occupied = 1;
	lst->First->next = pom;
	lst->First->ptr = malloc(sizeof(int));
	if (lst->First->ptr == NULL) print_error(err_run_oth);
	return lst->First;
}
void *AdrListNewDbl(t_AdrList* lst){
	t_AdrList_Item *pom;
	pom = lst->First;
	lst->First = malloc(sizeof(t_AdrList_Item));
	if (lst->First == NULL) print_error(err_run_oth);
	lst->First->occupied = 1;
	lst->First->next = pom;
	lst->First->ptr = malloc(sizeof(double));
	if (lst->First->ptr == NULL) print_error(err_run_oth);
	return lst->First;
}
void *AdrListNewString(t_AdrList* lst){
	t_AdrList_Item *pom;
	pom = lst->First;
	lst->First = malloc(sizeof(t_AdrList_Item));
	if (lst->First == NULL) print_error(err_run_oth);
	lst->First->occupied = 1;
	lst->First->next = pom;
	lst->First->ptr = malloc(sizeof(char*));
	if (lst->First->ptr == NULL) print_error(err_run_oth);
	*(char**)lst->First->ptr = NULL;
	return lst->First;
}

void *AdrListInsertRaw(t_AdrList* lst, void* ptr){
	t_AdrList_Item *pom;
	pom = lst->First;
	lst->First = malloc(sizeof(t_AdrList_Item));
	if (lst->First == NULL) print_error(err_run_oth);
	lst->First->occupied = 1;
	lst->First->next = pom;
	lst->First->ptr = ptr;
	return lst->First;
}

void *AdrListGetAvailable(t_AdrList* lst){
	t_AdrList_Item *pom = lst->First;
	while(pom != NULL){
        if (pom->occupied == 0) {
            pom->occupied = 1;
            return pom;
        }
        else{
            pom = pom->next;
        }
	}
	return NULL;
}

void AdrListMakeAvailable(void* ptr){
	((t_AdrList_Item*) ptr)->occupied = 0;
}

void AdrListMakeAvailableArgs(tParams_item *Ptr){
    while (Ptr != NULL){
    ((t_AdrList_Item*)(Ptr->Param))->occupied = 0;
    Ptr = Ptr->Next;
    }
}

void AdrListDispose(t_AdrList* l){
    t_AdrList_Item* item = l->First;
    t_AdrList_Item* temp;
    while (item != NULL){
        temp = item;
        item = item->next;
        free(temp->ptr);
        free(temp);
    }
}

void AdrListDisposeStrings(t_AdrList* l){
    t_AdrList_Item* item = l->First;
    t_AdrList_Item* temp;
    while (item != NULL){
        temp = item;
        item = item->next;
        free((*((char**)temp->ptr)));
        free(temp->ptr);
        free(temp);;
    }
}
