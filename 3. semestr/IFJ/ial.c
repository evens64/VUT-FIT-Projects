// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Jakub Hud (xhudja00),
//				Roman Janík (xjanik20)

#include "ial.h"

//binarytree

void BSTInit(tBST *BST){
    //BST->Depth = 1;
    //BST->Top = malloc(sizeof(struct level));
    //if (BST->Top == NULL) print_error(err_oth);
    //BST->Active = BST->Top;
    //BST->Top->Args= NULL;
    //BST->Top->MembersList = NULL;
    //BST->Top->Parent = NULL;
    //BST->Top->TreeRoot = NULL;
    BST->Depth=1;
    BST->Top = NULL;
    BST->Active = NULL;

}

void BSTSetActiveTop(tBST *BST){
    BST->Depth=1;
    BST->Active = BST->Top;
}



char* StringCopy(char *s){

	int i = 0;
	while(s[i] != 0){
		i++;
	}

	char *new_s = malloc(sizeof(char) * (i + 1));

	if(new_s == NULL){
		exit(1);
	}

	for(; i >= 0; i--){
		new_s[i] = s[i];
	}

	return new_s;
}

struct level* BSTPreviousLevel(tBST *BST){

	assert(BST->Depth != 1);

	BST->Active = (BST->Active)->Parent;

	BST->Depth--;
	return BST->Active;
}

struct level* BSTCreateFindLevel(tBST *BST, tMember *Member){

	assert(BST->Depth < 3);

	if(Member->NextLevel == NULL){
		tLevel *NewLevel = malloc(sizeof(struct level));

		if(NewLevel == NULL){
			print_error(99);
		}

		NewLevel->Parent = BST->Active;
		NewLevel->MembersList = NULL;
		NewLevel->TreeRoot = NULL;
		NewLevel->Args = NULL;
		Member->NextLevel = NewLevel;
	}

	BST->Depth++;
	BST->Active = Member->NextLevel;
	return BST->Active;
}

struct member* BSTInsertMember(tBST *BST, char *Name){

	//BST->Active je NULL jen v pripade ze jde o prvni pouziti
	if(BST->Active == NULL){
		assert(BST->Depth == 1);

		tLevel *FirstLevel = malloc(sizeof(struct level));

		if(FirstLevel == NULL){
			print_error(99);
		}

		FirstLevel->Parent = NULL;
		FirstLevel->TreeRoot = NULL;
		FirstLevel->MembersList = NULL;

		BST->Active = FirstLevel;
		BST->Top = FirstLevel;
	}

	if(BSTSearchMember(BST, Name) == NULL){
		tMember *new = malloc(sizeof(struct member));
		tMember *pom = (BST->Active)->MembersList;
		(BST->Active)->MembersList = new;

		new->Next = pom;
		new->Name = StringCopy(Name);
		new->NextLevel = NULL;

		return BST->Active->MembersList;
	}
	else{
		print_error(3);
	}

	return NULL;
}

void BSTAddArgument(tBST *BST,tIdentifier *arg){
     tArg *pom = BST->Active->Args;
     BST->Active->Args = malloc(sizeof (tArg));
     if(BST->Active->Args == NULL) print_error(err_oth);
     BST->Active->Args->Next = pom;
     BST->Active->Args->Arg = arg;
}

void ParamsInit(tParams *params){
    params->First = NULL;
    params->Last = NULL;
}

void ParamsInsert(tParams *params,void *value){
    tParams_item *param = malloc(sizeof (tParams_item));
    if(param == NULL) print_error(err_oth);

    param->Param = value;
    param->Next = NULL;
    if (params->Last == NULL){
        params->Last = param;
        params->First = param;
    }
    else params->Last->Next = param;
}

void ParamsDispose(tParams *params){
    tParams_item *pom;
    while(params->First != NULL)
        pom = params->First;
        params->First = params->First->Next;
        free (pom);
    params->Last=NULL;
}


struct member* BSTSearchMember(tBST *BST, char *Name){
    if(BST->Active == NULL) return NULL;
	tMember *pom = (BST->Active)->MembersList;

	int i = 0;
	while(pom != NULL){

		while(Name[i] != 0 || (pom->Name)[i] != 0){
			if(Name[i] == (pom->Name)[i]){
				i++;
			}
			else{
				break;
			}
		}

		if(Name[i] == 0 && (pom->Name)[i] == 0){
			break;
		}
		else{
			pom = pom->Next;
		}
	}

	return pom;
}

void BSTDisposeLevel(tBST *BST, struct level **Level){
	assert(*Level != NULL);

	TreeDispose(&((*Level)->TreeRoot));

	tMember *PomNextMember;
	while((*Level)->MembersList != NULL){
		if(((*Level)->MembersList)->NextLevel != NULL){
			BSTDisposeLevel(BST, &(((*Level) -> MembersList) -> NextLevel) );
		}

		PomNextMember = ((*Level)->MembersList)->Next;

		//uvolneni Levelu vcetne jeho jmena
		free(((*Level)->MembersList)->Name);
		tArg * tempArg;
		while ((*Level)->Args != NULL){
            tempArg = ((*Level)->Args)->Next;
            free((*Level)->Args);
            (*Level)->Args = tempArg;
		}
		free((*Level)->MembersList);

		(*Level)->MembersList = PomNextMember;
	}

	free(*Level);
	*Level = NULL;
}

void BSTDisposeAll(tBST *BST){

	struct level *PomLevel = BST->Top;

	assert(PomLevel != NULL);
	assert(PomLevel->TreeRoot == NULL);

	tMember *PomNextMember;
	while(PomLevel->MembersList != NULL){

		if((PomLevel->MembersList)->NextLevel != NULL){
			BSTDisposeLevel(BST, &((PomLevel->MembersList)->NextLevel));
		}

		PomNextMember = (PomLevel->MembersList)->Next;

		//uvolneni Levelu vcetne jeho jmena
		free((PomLevel->MembersList)->Name);
		free(PomLevel->MembersList);

		PomLevel->MembersList = PomNextMember;
	}
    tArg *PomNextArg;
    while(PomLevel->Args != NULL){
		PomNextArg = (PomLevel->Args)->Next;
		//uvolneni Levelu vcetne jeho jmena
		free(PomLevel->Args);
		PomLevel->Args = PomNextArg;
	}

	free(PomLevel);

	BST->Top = NULL;
	BST->Active = NULL;
	BST->Depth = 1;
}

/**Funkce pro praci s binarnim stromem**/

TokenTypes* CopyParametrs(TokenTypes parametrs[],int nParam){

	if(parametrs != NULL){
		TokenTypes *new_parametrs = malloc(sizeof(TokenTypes) * nParam);

		if(new_parametrs == NULL){
			print_error(99);
		}

		for(int i = 0; i < nParam; i++){
			new_parametrs[i] = parametrs[i];
		}

		return new_parametrs;
	}

	return parametrs;
}

int Changedottozero(char *string){
    int i = 0;
    int dotfound = -1;
    while (string[i] != '\0'){
        if (string[i] == '.'){
            if (dotfound == -1){
                dotfound = i+1;
                string[i] = '\0';
            } else print_error(err_sem_oth);
        }
        i++;
    }
    return dotfound;
}

struct identifier* BSTSearchIdentifier(tBST BST, char *name){ //returns NULL if it doesn't exist

    int dotpos = Changedottozero(name);
    struct identifier *RootPtr;

    if(dotpos == -1){
        RootPtr = BST.Active->TreeRoot;
    }
    else{
        BST.Active = BST.Top;
        BST.Depth = 0;
        tMember * member = BSTSearchMember(&BST, name);
        if (member == NULL) print_error(err_sem_und);
        RootPtr = BSTCreateFindLevel(&BST,member)->TreeRoot;
        name = name + dotpos;
    }

    int com;
	while(RootPtr != NULL){

		while(RootPtr != NULL){

			com = strcmp(RootPtr->name, name);

			if(com == 0){
				return RootPtr;
			}

			if(com < 0){
				RootPtr = RootPtr->RPtr;
			}

			if(com > 0){
				RootPtr = RootPtr->LPtr;
			}
		}
        BST.Active = (BST.Active)->Parent;
		RootPtr = (BST.Active)->TreeRoot;
	}

	return RootPtr;
}

tIdentifier *BSTInsertIdentifier(tBST *BST, char *name, TokenTypes type, void *value /*, int nParam, TokenTypes parametrs[]*/, bool decl, bool used){
//Vytvori uzel s danymi parametry. Pokud uz uzel se stejnym jmenem
//existuje nastane chyba 3
	assert(BST->Active != NULL);
	tIdentifier **RootPtr = &((BST->Active)->TreeRoot);

	if(*RootPtr == NULL){
		*RootPtr = malloc(sizeof(struct identifier));

		if(*RootPtr == NULL){
		//neuspesny malloc
			print_error(99);
		}

		(*RootPtr)->name = StringCopy(name);
		(*RootPtr)->type = type;
		(*RootPtr)->value = value;
		/*(*RootPtr)->nParam = nParam;
		(*RootPtr)->parametrs = CopyParametrs(parametrs, nParam);*/
		(*RootPtr)->decl = decl;
		(*RootPtr)->used = used;
		(*RootPtr)->RPtr = NULL;
		(*RootPtr)->LPtr = NULL;
		(*RootPtr)->prevValue = NULL;
		(*RootPtr)->retValues = NULL;
		return *RootPtr;
	}
	else{
	//hledani uzlu s jmenem Name

		int com ;
		while(*RootPtr != NULL){

			com = strcmp((*RootPtr)->name, name);

			if(com == 0){
				print_error(3);
			}

			if(com < 0){
				RootPtr = &((*RootPtr)->RPtr);
			}

			if(com > 0){
				RootPtr = &((*RootPtr)->LPtr);
			}
		}

		*RootPtr = malloc(sizeof(struct identifier));

		if(*RootPtr == NULL){
		//neuspesny malloc
			print_error(99);
		}

		(*RootPtr)->name = StringCopy(name);
		(*RootPtr)->type = type;
		(*RootPtr)->value = value;
		(*RootPtr)->decl = decl;
		(*RootPtr)->used = used;
		(*RootPtr)->RPtr = NULL;
		(*RootPtr)->LPtr = NULL;
		return *RootPtr;
	}
}

void TreeDispose(tIdentifier **RootPtr){
//odstraneni celeho stromu, strom je ve stavu jako po inicializaci

	//postupne prochazeni stale hloubeji
	if(*RootPtr != NULL){
		if((*RootPtr)->LPtr != NULL){
			TreeDispose(&((*RootPtr)->LPtr));
		}

		if((*RootPtr)->RPtr != NULL){
			TreeDispose(&((*RootPtr)->RPtr));
		}

		//nakonec uvolneni kazdeho uzlu vcetne jmena a parametru
		free((*RootPtr)->name);
		/*free((*RootPtr)->parametrs);*/
		if((*RootPtr)->type == _String_) free(*(char**)(*RootPtr)->value);
        free((*RootPtr)->value);
		free(*RootPtr);
		*RootPtr = NULL;
	}

}

//heapsort

String sort(String s)
{
	int i;
	char tmp;		// promenna pro prvek
	int n = strlen(s);

	heapify(s, n);		// vytvoreni haldy
	for (i = n-1; i>0; i--)		// cyklus prochazi pole od posledniho do 2. prvku
	{
		tmp = s[0];			// vymena prvku
		s[0] = s[i];
		s[i] = tmp;
		heapbuild(s,i);			// tridici funkce
	}

    return s;
}

void heapify(String s, int n)			// funkce pro vytvoreni haldy
{
	int k, i, j;						// promenne pro indexaci
	char tmp;							// promenna pro prvek
	for (k = 1; k < n; k++)				// cyklus prochazi pole od 2. do predposledniho prvku
	{
		tmp = s[k];
		i = k;
		j = (i - 1) / 2;

		while (i > 0 && tmp > s[j])		// cyklus pro vymenu prvku o 1 uroven v halde
		{
			s[i] = s[j];
			i = j;
			j = (i - 1) / 2;
		}
		s[i] = tmp;
	}
}

void heapbuild(String s, int n)			// funkce pro trideni haldy
{
	int i, j = 0;						// promenne pro indexaci
	char tmp;							// promenna pro prvek
	tmp = s[j];
	i = 2 * j + 1;

	while (i < n)							// cyklus prochazi pole od 2. do max. predposledniho prvku
	{
		if (i + 1 < n) {
			if (s[i] < s[i+1]) {
				i++;
			}
		}

		if (tmp < s[i])					// cyklus pro vymenu prvku o 1 uroven v halde
		{
			s[j] = s[i];
			j = i;
			i = 2 * j + 1;
		}
		else break;
	}
	s[j] = tmp;
}

//KMP
int find(String s, String search)
{

	String search_pom = search;
	int M = 0;
	while(*search_pom++) M++;

	search_pom = s;
	int N = 0;
	while(*search_pom++) N++;

	int j  = 0;			// index pro podretezec
	int i = 0;			// index pro retezec

	if (M == 0 && N == 0) {		// pripad prazdnych retezcu
		return 0;
	}
	int *fail = kmpgraph(search);                       // alokace pole

	while (i < N)		// cyklus od 2. znaku podretezce do posledniho
	{
		if (search[j] == s[i]) {	// shoda
			j++;
			i++;
		}
		if (j == M) {		// konec podretezce
			free(fail); // to avoid memory leak
			return i-j;
		} else if (search[j] != s[i]) {		// pripad neshody po shode
			if (j != 0) {
				j = fail[j-1];		// navrat v podretezci
			} else {
				i++;
			}
		}
	}
	free(fail); // to avoid memory leak
	return -1;
}

int *kmpgraph(String search) {
	int i;
	int *fail;

	int N = 0;
	String search_pom = search;
	while(*search_pom++) N++;

	int len = 0;		// delka predchoziho nejdelsiho preffixu suffixu

	fail = (int*)malloc(sizeof(int) * N);		// alokace pole pro navrat v podretezci
	if (fail == NULL) {
		print_error(err_oth);
	}

	fail[0] = 0;		// 1.prvek je vzdy 0
	i = 1;

	while(i < N)		// cyklus od 2. znaku podretezce do posledniho
	{
		if(search[i] == search[len])		// shoda
		{
			len++;
			fail[i] = len;
			i++;
		}
		else								// neshoda
		{
			if( len != 0 )
			{
				len = fail[len-1];		// navrat v retezci
			}
			else						// if (len == 0)
			{
				fail[i] = 0;
				i++;
			}
		}
	}
	return fail;
}
