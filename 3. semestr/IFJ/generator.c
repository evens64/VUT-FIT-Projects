// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Jakub Frýz (xfryzj01)

#include "generator.h"

extern TInstrList IList;
extern TAdrStack IStack;

char* Instrukce[] = {		// pomocný array pro debug výpisy
	"if_start", "if_end", "else_start", "else_end",
	"while_start", "while_end", "return_val",
	"ass_int", "ass_dbl", "ass_str",
	"eq_int", "noteq_int", "gt_int", "gteq_int", "ls_int", "lseq_int", "add_int", "sub_int", "mul_int", "div_int",
	"eq_dbl", "noteq_dbl", "gt_dbl", "gteq_dbl", "ls_dbl", "lseq_dbl", "add_dbl", "sub_dbl", "mul_dbl", "div_dbl",
	"add_str",
	"fce_call", "fce_start", "fce_end",
	"cvt_int2dbl", "cvt_dbl2int",
	"ifj16readIntCall", "ifj16readDoubleCall", "ifj16readStringCall", "ifj16printCall", "ifj16lengthCall", "ifj16substrCall", "ifj16compareCall", "ifj16findCall", "ifj16sortCall"
};


// fce na generování 3-adresného kódu
void generate(InstrTypes t, void *op1, void *op2, void *res) {

#ifdef debug
	printf("FCE_START: generate |type = %s|\n", Instrukce[t]);
#endif

	TInstr *Helper, *Temp;
	switch (t) {
//----------------------------------------------------------------------------------
		case if_start:		// začátek if větve
			Helper = AllocInstr();
			Helper->t = t;						// 'if_start'
			Helper->op1 = op1;
			Helper->op2 = op2;
			Helper->res = res;

			IListInsert(&IList, Helper);		// vložení 'if_start' do listu

			IStackPush(&IStack, IList.Last);	// push 'if_start'

			break;
//----------------------------------------------------------------------------------
		case if_end:		// konec if větve
			Helper = AllocInstr();				//while end před if start způsobuje přepsání while_end->nope
			Helper->t = t;						// 'if_start'
			Helper->op1 = op1;
			Helper->op2 = op2;
			Helper->res = res;
			IListInsert(&IList, Helper);

			break;
//----------------------------------------------------------------------------------
		case else_start:	// začátek else větve
			Helper = AllocInstr();
			Helper->t = t;							//'else_start'

			Temp = IList.Last;

			IListInsertElse(&IList, Helper);		// vložení 'else_start' do listu (ale do nope větve)

			IStackPop(&IStack)->nope = IList.Last;	// 'if_start' -> 'else_start'

			IStackPush(&IStack, Temp);				// push 'else_start'

			break;
//----------------------------------------------------------------------------------
		case else_end:		// konec else větve
			Helper = AllocInstr();
			Helper->t = else_end;				// 'else_end'

			IListInsert(&IList, Helper);		// vložení 'else_end' do listu

			Temp = IStackPop(&IStack);			// popnutí adresy na "'else_start'"
			Temp->next = IList.Last;			// "'else_start'" -> 'else_end'

			break;
//----------------------------------------------------------------------------------
		case while_start:	// začátek while cyklu
			Helper = AllocInstr();
			Helper->t = t;

			IListInsert(&IList, Helper);		// vložení 'while_start' do listu

			IStackPush(&IStack, IList.Last);	// uložení adresy na 'while_start' na stack

			break;
//----------------------------------------------------------------------------------
		case while_condition:		// podmínka pro cyklus while
			Helper = AllocInstr();
			Helper->t = t;
			Helper->op1 = op1;
			Helper->op2 = op2;
			Helper->res = res;

			IListInsert(&IList, Helper);		// vložení 'while_condition' do listu

			IStackPush(&IStack, IList.Last);	// uložení adresy na 'while_condition' na stack

			break;
//----------------------------------------------------------------------------------
		case while_end:		// konec while cyklu
			Helper = AllocInstr();
			Helper->t = t;

			IListInsert(&IList, Helper);		// vložení 'while_end' do listu

			Temp = IStackPop(&IStack);			// uložení adresy na 'while_condition'
			Temp->nope = IList.Last;			// 'while_condition' nyní ukazuje na poslední ('while_end')

			Temp = IStackPop(&IStack);			// uložení adresy na 'while_start'
			IList.Last->nope = Temp;			// poslední ('while_end') nyní ukazuje na 'while_start'

			break;
//----------------------------------------------------------------------------------
		case fce_start:		// začátek funkce
		default:
			if (IList.Last != NULL) {		// pokud není list prázdný
				if (IList.Last->t == else_start) {		// pokud poslední prvek je 'else_start'
					IList.Last->t = t;					// přepsat
					IList.Last->op1 = op1;
					IList.Last->op2 = op2;
					IList.Last->res = res;
				} else {
					Helper = AllocInstr();				// alokace nového prvku
					Helper->t = t;
					Helper->op1 = op1;
					Helper->op2 = op2;
					Helper->res = res;
					IListInsert(&IList, Helper);		// vložení instrukce do listu
				}
			} else {
				Helper = AllocInstr();				// alokace nového prvku
				Helper->t = t;
				Helper->op1 = op1;
				Helper->op2 = op2;
				Helper->res = res;
				IListInsert(&IList, Helper);		// vložení instrukce do listu
			}

			if(t == fce_start) ((tLevel*)((tIdentifier*)op1)->value)->fceStart = Helper;

			break;
//----------------------------------------------------------------------------------
	}
}

//--pomocné-fce--------------------------------------------------------------------------

TInstr *AllocInstr() {
	TInstr *Item = malloc(sizeof(struct SInstr));
	if (Item == NULL) {
		print_error(err_oth);	// test alokace
	}
	Item->op1 = NULL;
	Item->op2 = NULL;
	Item->res = NULL;
	Item->next = NULL;
	return Item;
}

TAdr *AllocAdr() {
	TAdr *Item = malloc(sizeof(struct SAdr));
	if (Item == NULL) {
		print_error(err_oth);	// test alokace
	}
	Item->address = NULL;
	Item->next = NULL;
	return Item;
}

//--práce-s-listem-----------------------------------------------------------------------

void IListInit(TInstrList *L) {					// inicializace listu
	L->First = NULL;
	L->Last = NULL;
	L->Act = NULL;
}

void IListInsert(TInstrList *L, TInstr *I) {
	if (L->First == NULL && L->Last == NULL) {	// list je prázdný
		L->First = I;
		L->Last = I;
	} else {									// list není prázdný
		L->Last->next = I;
		L->Last = I;
	}
}

void IListInsertElse(TInstrList *L, TInstr *I) {
	if (L->First != NULL && L->Last != NULL) {	// list není prázdný
		L->Last->nope = I;
		L->Last = I;
	} else {									// list je prázdný (blbost)
		print_error(err_oth);
	}
}

void IListDispose(TInstrList *L) {
	L->Act = NULL;								// reset aktivního prvku
	L->Last = NULL;
	while (L->First != NULL) {					// dokud jsou v seznamu prvky
		TInstr *Helper = L->First;				// uložení prvního prvku do pomocné proměnné
#ifdef debug
		printf("\tLIST: delete |%s|\n", Instrukce[Helper->t]);
#endif
		if (Helper->t == if_end) {
			L->First = L->First->nope;			// posun v seznamu
		} else {
			L->First = L->First->next;			// posun v seznamu
		}
		free(Helper);							// uvolnění prvku
	}
}

void IActivate(TInstrList *L) {
	L->Act = L->First;							// aktivace listu
}

void IActiveNext(TInstrList *L) {
	if (L->Act != NULL) {
		L->Act = L->Act->next;					// posun v seznamu přes 'next'
	}
}

void IActiveElse(TInstrList *L) {
	if (L->Act != NULL && (L->Act->t == if_start || L->Act->t == while_start || L->Act->t == while_end)) {
		L->Act = L->Act->nope;					// posun v seznamu přes 'nope' (pouze při prvku 'if_start')
	}
}

void IGoTo(TInstrList *L, TInstr *I) {
	L->Act = I;									// posun na zadanou položku
}

bool IListActive(TInstrList *L) {
	return (L->Act != NULL) ? true : false;	// kontrola aktivity listu
}

//--práce-se-zásobníkem------------------------------------------------------------------

void IStackInit(TAdrStack *S) {					// inicializace stacku
	S->Top = NULL;
}

void IStackPush(TAdrStack *S, TInstr *I) {
	TAdr *Helper = AllocAdr();					// alokace nového prvku stacku
	Helper->address = I;						// uložení ukazatele na instrukce
	Helper->next = S->Top;						// vlože na vrchol stacku
	S->Top = Helper;
}

TInstr *IStackPop(TAdrStack *S) {
	TInstr *Helper = S->Top->address;			// uložení adresy na instrukci na vrcholu
	IStackTopRemove(S);							// smazání vrchního prvku
	return Helper;								// návrat uložené adresy
}

void IStackTopRemove(TAdrStack *S) {
	if (S->Top != NULL) {						// pokud stack není prázdný
		TAdr *Helper = S->Top;					// ulož vrchol
		S->Top = Helper->next;					// posuň vrchol
		free(Helper);							// smaž
	}
}

void IStackDispose(TAdrStack *S) {
	if (S->Top != NULL) {						// pokud stack není prázdný
		do {
			IStackTopRemove(S);					// maž ...
		} while (S->Top != NULL);				// ... dokud stack není prázdný
	}
}
