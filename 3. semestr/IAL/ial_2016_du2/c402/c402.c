
/* c402.c: ********************************************************************}
{* Téma: Nerekurzivní implementace operací nad BVS
**                                     Implementace: Petr Přikryl, prosinec 1994
**                                           Úpravy: Petr Přikryl, listopad 1997
**                                                     Petr Přikryl, květen 1998
**			  	                        Převod do jazyka C: Martin Tuček, srpen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                         Úpravy: Karel Masařík, říjen 2013
**                                         Úpravy: Radek Hranický, říjen 2014
**                                         Úpravy: Radek Hranický, listopad 2015
**                                         Úpravy: Radek Hranický, říjen 2016
**
** S využitím dynamického přidělování paměti, implementujte NEREKURZIVNĚ
** následující operace nad binárním vyhledávacím stromem (předpona BT znamená
** Binary Tree a je u identifikátorů uvedena kvůli možné kolizi s ostatními
** příklady):
**
**     BTInit .......... inicializace stromu
**     BTInsert ........ nerekurzivní vložení nového uzlu do stromu
**     BTPreorder ...... nerekurzivní průchod typu pre-order
**     BTInorder ....... nerekurzivní průchod typu in-order
**     BTPostorder ..... nerekurzivní průchod typu post-order
**     BTDisposeTree ... zruš všechny uzly stromu
**
** U všech funkcí, které využívají některý z průchodů stromem, implementujte
** pomocnou funkci pro nalezení nejlevějšího uzlu v podstromu.
**
** Přesné definice typů naleznete v souboru c402.h. Uzel stromu je typu tBTNode,
** ukazatel na něj je typu tBTNodePtr. Jeden uzel obsahuje položku int Cont,
** která současně slouží jako užitečný obsah i jako vyhledávací klíč
** a ukazatele na levý a pravý podstrom (LPtr a RPtr).
**
** Příklad slouží zejména k procvičení nerekurzivních zápisů algoritmů
** nad stromy. Než začnete tento příklad řešit, prostudujte si důkladně
** principy převodu rekurzivních algoritmů na nerekurzivní. Programování
** je především inženýrská disciplína, kde opětné objevování Ameriky nemá
** místo. Pokud se Vám zdá, že by něco šlo zapsat optimálněji, promyslete
** si všechny detaily Vašeho řešení. Povšimněte si typického umístění akcí
** pro různé typy průchodů. Zamyslete se nad modifikací řešených algoritmů
** například pro výpočet počtu uzlů stromu, počtu listů stromu, výšky stromu
** nebo pro vytvoření zrcadlového obrazu stromu (pouze popřehazování ukazatelů
** bez vytváření nových uzlů a rušení starých).
**
** Při průchodech stromem použijte ke zpracování uzlu funkci BTWorkOut().
** Pro zjednodušení práce máte předem připraveny zásobníky pro hodnoty typu
** bool a tBTNodePtr. Pomocnou funkci BTWorkOut ani funkce pro práci
** s pomocnými zásobníky neupravujte
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c402.h"
int solved;

void BTWorkOut (tBTNodePtr Ptr)		{
/*   ---------
** Pomocná funkce, kterou budete volat při průchodech stromem pro zpracování
** uzlu určeného ukazatelem Ptr. Tuto funkci neupravujte.
**/

	if (Ptr==NULL)
    printf("Chyba: Funkce BTWorkOut byla volána s NULL argumentem!\n");
  else
    printf("Výpis hodnoty daného uzlu> %d\n",Ptr->Cont);
}

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu tBTNodePtr. Tyto funkce neupravujte.
**/

void SInitP (tStackP *S)
/*   ------
** Inicializace zásobníku.
**/
{
	S->top = 0;
}

void SPushP (tStackP *S, tBTNodePtr ptr)
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
{
                 /* Při implementaci v poli může dojít k přetečení zásobníku. */
  if (S->top==MAXSTACK)
    printf("Chyba: Došlo k přetečení zásobníku s ukazateli!\n");
  else {
		S->top++;
		S->a[S->top]=ptr;
	}
}

tBTNodePtr STopPopP (tStackP *S)
/*         --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
{
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0)  {
		printf("Chyba: Došlo k podtečení zásobníku s ukazateli!\n");
		return(NULL);
	}
	else {
		return (S->a[S->top--]);
	}
}

bool SEmptyP (tStackP *S)
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
{
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu bool. Tyto funkce neupravujte.
*/

void SInitB (tStackB *S) {
/*   ------
** Inicializace zásobníku.
**/

	S->top = 0;
}

void SPushB (tStackB *S,bool val) {
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
                 /* Při implementaci v poli může dojít k přetečení zásobníku. */
	if (S->top==MAXSTACK)
		printf("Chyba: Došlo k přetečení zásobníku pro boolean!\n");
	else {
		S->top++;
		S->a[S->top]=val;
	}
}

bool STopPopB (tStackB *S) {
/*   --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0) {
		printf("Chyba: Došlo k podtečení zásobníku pro boolean!\n");
		return(NULL);
	}
	else {
		return(S->a[S->top--]);
	}
}

bool SEmptyB (tStackB *S) {
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** Následuje jádro domácí úlohy - funkce, které máte implementovat.
*/

void BTInit (tBTNodePtr *RootPtr)	{
/*   ------
** Provede inicializaci binárního vyhledávacího stromu.
**
** Inicializaci smí programátor volat pouze před prvním použitím binárního
** stromu, protože neuvolňuje uzly neprázdného stromu (a ani to dělat nemůže,
** protože před inicializací jsou hodnoty nedefinované, tedy libovolné).
** Ke zrušení binárního stromu slouží procedura BTDisposeTree.
**
** Všimněte si, že zde se poprvé v hlavičce objevuje typ ukazatel na ukazatel,
** proto je třeba při práci s RootPtr použít dereferenční operátor *.
**/

	*RootPtr = NULL;

	//solved = FALSE;		  /* V případě řešení smažte tento řádek! */
}

void BTInsert (tBTNodePtr *RootPtr, int Content) {
/*   --------
** Vloží do stromu nový uzel s hodnotou Content.
**
** Z pohledu vkládání chápejte vytvářený strom jako binární vyhledávací strom,
** kde uzly s hodnotou menší než má otec leží v levém podstromu a uzly větší
** leží vpravo. Pokud vkládaný uzel již existuje, neprovádí se nic (daná hodnota
** se ve stromu může vyskytnout nejvýše jednou). Pokud se vytváří nový uzel,
** vzniká vždy jako list stromu. Funkci implementujte nerekurzivně.
**/

	bool FOUND = FALSE;
	tBTNodePtr Temp = *RootPtr;
	tBTNodePtr Item;

	if (*RootPtr == NULL) {		//strom je prázdný
		FOUND = FALSE;				//nenalezen
		Item = NULL;
	} else {					//strom není prázdný
		do {
			Item = Temp;			//uložení pointeru
			if (Content < Temp->Cont) {						//pokud je Content menší
				Temp = Temp->LPtr;								//jde se doleva
			} else if (Content > Temp->Cont) {		//pokud je Content větší
				Temp = Temp->RPtr;								//jde se doprava
			} else {										//pokud je Content roven
				FOUND = TRUE;									//nalezen
			}
		} while (FOUND == FALSE && Temp != NULL);		//podmínka cyklu (dokud se nenajde nebo nedojde na konec)
	}

	if (FOUND != TRUE) {	//Content nenalezen
		tBTNodePtr Helper = malloc(sizeof(struct tBTNode));		//alokace nového
		if (Helper == NULL) {		//kontrola alokace
			return;
		}
		Helper->Cont = Content;
		Helper->LPtr = NULL;
		Helper->RPtr = NULL;
		if (Item == NULL) {			//strom je prázdný
			*RootPtr = Helper;
		} else {					//strom není prázdný
			if (Content < Item->Cont) {		//pokud je 'Content' menší
				Item->LPtr = Helper;			//uloží se doleva
			} else {						//pokud je 'Content' větší
				Item->RPtr = Helper;			//uloží se doprava
			}
		}
	}

	//solved = FALSE;		  /* V případě řešení smažte tento řádek! */
}

/*                                  PREORDER                                  */

void Leftmost_Preorder (tBTNodePtr ptr, tStackP *Stack)	{
/*   -----------------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Preorder navštívené uzly zpracujeme voláním funkce BTWorkOut()
** a ukazatele na ně is uložíme do zásobníku.
**/

	while (ptr != NULL) {
		SPushP(Stack, ptr);		//uložení pointeru
		BTWorkOut(ptr);			//zpracování
		ptr = ptr->LPtr;		//posun doleva
	}

	//solved = FALSE;		  /* V případě řešení smažte tento řádek! */
}

void BTPreorder (tBTNodePtr RootPtr)	{
/*   ----------
** Průchod stromem typu preorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Preorder a zásobníku ukazatelů. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut().
**/

		//A -> B -> C

	if (RootPtr != NULL) {		//kontrola existence
		tStackP Stack;
		SInitP(&Stack);		//inicializace stacku
		Leftmost_Preorder(RootPtr, &Stack);		//použití fce
		while (SEmptyP(&Stack) == FALSE) {		//dokud jsou na stacku nějaké pointery
			RootPtr = STopPopP(&Stack);					//popni pointer
			Leftmost_Preorder(RootPtr->RPtr, &Stack);	//použítí fce doprava
		}
	}

	//solved = FALSE;		  /* V případě řešení smažte tento řádek! */
}


/*                                  INORDER                                   */

void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack)		{
/*   ----------------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Inorder ukládáme ukazatele na všechny navštívené uzly do
** zásobníku.
**/

	while (ptr != NULL) {
		SPushP(Stack, ptr);		//uložení pointeru
		ptr = ptr->LPtr;		//posun doleva
	}

	//solved = FALSE;		  /* V případě řešení smažte tento řádek! */
}

void BTInorder (tBTNodePtr RootPtr)	{
/*   ---------
** Průchod stromem typu inorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Inorder a zásobníku ukazatelů. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut().
**/

		//B -> A -> C

	if (RootPtr != NULL) {	//kontrola existence
		tStackP Stack;
		SInitP(&Stack);		//inicializace stacku
		Leftmost_Inorder(RootPtr, &Stack);	//použití fce
		while (SEmptyP(&Stack) == FALSE) {		//dokud není stack prázdný
			RootPtr = STopPopP(&Stack);					//popni pointer
			BTWorkOut(RootPtr);							//zpracuj
			Leftmost_Inorder(RootPtr->RPtr, &Stack);	//použítí fce doprava
		}
	}

	//solved = FALSE;		  /* V případě řešení smažte tento řádek! */
}

/*                                 POSTORDER                                  */

void Leftmost_Postorder (tBTNodePtr ptr, tStackP *StackP, tStackB *StackB) {
/*           --------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Postorder ukládáme ukazatele na navštívené uzly do zásobníku
** a současně do zásobníku bool hodnot ukládáme informaci, zda byl uzel
** navštíven poprvé a že se tedy ještě nemá zpracovávat.
**/

	while (ptr != NULL) {
		SPushP(StackP, ptr);		//uložení pointeru
		SPushB(StackB, TRUE);		//uložení boolu (navštíven poprvé)
		ptr = ptr->LPtr;			//posun doleva
	}

	//solved = FALSE;		  /* V případě řešení smažte tento řádek! */
}

void BTPostorder (tBTNodePtr RootPtr)	{
/*           -----------
** Průchod stromem typu postorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Postorder, zásobníku ukazatelů a zásobníku hotdnot typu bool.
** Zpracování jednoho uzlu stromu realizujte jako volání funkce BTWorkOut().
**/

		// B -> C -> A

	if(RootPtr != NULL) {
		tStackP StackP;
		tStackB StackB;
		SInitP(&StackP);		//inicializace stacku pointerů
		SInitB(&StackB);		//inicializace stacku boolů
		Leftmost_Postorder(RootPtr, &StackP, &StackB);		//použití fce
		while (SEmptyP(&StackP) == FALSE) {					//dokud není stack prázdný
			RootPtr = STopPopP(&StackP);					//popni
			SPushP(&StackP, RootPtr);						//zpátky ulož na stack
			if (STopPopB(&StackB) == TRUE) {					//je navštíven poprvé
				SPushB(&StackB, FALSE);										//příště už nebude
				Leftmost_Postorder(RootPtr->RPtr, &StackP, &StackB);		//použítí fce doprava
			} else {											//není navštíven poprvé
				STopPopP(&StackP);											//popni pointer
				BTWorkOut(RootPtr);											//zpracuj
			}
		}
	}

	//solved = FALSE;		  /* V případě řešení smažte tento řádek! */
}


void BTDisposeTree (tBTNodePtr *RootPtr)	{
/*   -------------
** Zruší všechny uzly stromu a korektně uvolní jimi zabranou paměť.
**
** Funkci implementujte nerekurzivně s využitím zásobníku ukazatelů.
**/

	if (*RootPtr != NULL) {		//kontrola existence
		tBTNodePtr Helper;
		tStackP Stack;
		SInitP(&Stack);		//inicializace stacku
		do {
			if (*RootPtr == NULL) {		//pokud jsme se zbavili všech podstromů vlevo
				if (SEmptyP(&Stack) == FALSE) {			//pokud stack není prázdný
					*RootPtr = STopPopP(&Stack);	//pop pointer pravého podstromu
				}
			} else {
				if ((*RootPtr)->RPtr != NULL) {			//pokud má uzel pravý podstrom
					SPushP(&Stack, (*RootPtr)->RPtr);		//push na stack
				}
				Helper = *RootPtr;				//uložení aktuální uzlu
				*RootPtr = (*RootPtr)->LPtr;	//posun doleva
				free(Helper);					//uvolnění uloženého uzlu
			}
		} while ((*RootPtr) != NULL || (SEmptyP(&Stack) == FALSE));		//podmínka cyklu
	}

	//solved = FALSE;		  /* V případě řešení smažte tento řádek! */
}

/* konec c402.c */

