// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Michal Hrabovský (xhrabo08),
//				Roman Janík (xjanik20)

#include "builtinfce.h"

// pomocné fce

String sInit()		// inicializace řetězce
{
	String s;
	if ((s = (String) malloc(ALLOC_LEN)) == NULL)
		print_error(err_oth);
	return s;
}

int sRealloc(String s)		// fce pro realokaci retezce
{
	if ((s = (String) realloc(s, ALLOC_LEN + length(s))) == NULL)
		print_error(err_oth);
	return 0;
}

String nonwhite(String s)		// fce pro zjisteni, zda na zacatku jsou bile znaky
{
	if (isspace(s[0]))
		print_error(err_oth);
	return s;
}

String concat(String s1, String s2)
{
	int len_s1 = length(s1);
	int len_s2 = length(s2);
	String res;

	res = (String)malloc(len_s1 + len_s2 + 1);
	if (res == NULL) {
		print_error(err_oth);
	} else {
		memcpy(res, s1, len_s1);
		memcpy(res + len_s1, s2, len_s2 + 1);
	}
	return res;
}

String itoa(int i, String s){
    char const digit[] = "0123456789";

    int sh = i;
    do 		// posun na konec pole
	{
        ++s;
        sh /= 10;
    } while(sh);

    *s = '\0';		// vlozeni zarazky '\0' na konce retezce
    do		// posun na zacatek s vkladani znaku cislic
	{
        *--s = digit[i%10];
        i /= 10;
    } while(i);
    return s;
}

int lint(int x)
{
	int f = 1;		// nutnost mista pro '\0'
	do		// vypocet delky int
	{
		x /= 10;
		f++;
	} while (x);
	return f;
}

String convert(Ttoken *tok)
{
	String s;
	switch (tok->type)
	{
		case con_string:
			{
				s = (String) malloc(length(tok->value)+1);
				if (s == NULL)
					print_error(err_oth);
				strcpy(s, tok->value);
				return s;
			}
		case con_int:		// prevod int -> string
			{
				s = (String) malloc(lint( *(int*) tok->value));		// pole pro retezec
				if (s == NULL)
					print_error(err_oth);
				itoa(*(int*) (tok->value), s);
				return s;
			}
		case con_double:		// prevod double -> string
			{
				s = (String) malloc(DOUBLE_LEN);		// pole pro retezec
				if (s == NULL)
					print_error(err_oth);
				sprintf(s, "%g", *(double*) tok->value);
				return s;
			}
		default:
			return NULL;
	}
}

// vestavěné fce pro načítání literálů a výpis termů

int readInt()
{
	char* endptr;
	String s = readString();
	long int n = strtol(nonwhite(s), &endptr, 0);	// prevod retezce na int
	if (*endptr != '\0' || n < 0)		//kontrola prevodu
	{
		print_error(err_oth);
	}
	free(s);
	return n;
}

double readDouble()
{
	char *endptr;
	String s = readString();
	double n = strtod(nonwhite(s), &endptr);			// prevod retezce na double
	if (*endptr != '\0' || n < 0)		//kontrola prevodu
	{
		print_error(err_oth);
	}
	free(s);
	return n;
}

String readString()
{
	String s = sInit();		// alokace retezce
	int c = getchar();
	int i = 0;
	int all_size = ALLOC_LEN;		// pocet alokovanych znaku

	while ((c != EOF) && (c != '\n'))		// cteni ze vstupu
	{
		if (i == all_size-1)		// v pripade zaplneni alokovaneho mista, pole se zvetsi
		{
			sRealloc(s);
			all_size = all_size + ALLOC_LEN;
		}
		s[i++] = c;			// zapis precteneho znaku do retezce
		c = getchar();
	}
	s[i] = '\0';		// vlozeni zarazky '\0' na konce retezce
	return s;
}

void print(Ttoken *tok)
{
    Ttoken *pom = tok;

	int stringpos = 0;
	while (pom != NULL)		// pro 1 token
	{
	    switch (pom->type)
		{
		    case _String_:
		        stringpos = stringpos + strlen(*((char**)(pom->value)));
            case _int_:
            case _double_:
                stringpos = stringpos + 12;
                break;
            default:
                print_error(err_sem_kom);
		}
		pom = pom->next;
	}
	stringpos++;
    char *tempstring  = malloc(sizeof(char)*stringpos);
	pom = tok;
	stringpos=0;
	while (pom != NULL)		// pro 1 token
	{
		switch (pom->type)
		{
			case _String_:
			    stringpos = stringpos + sprintf(tempstring + stringpos,"%s",*((char**)pom->value));
				break;
			case _int_:
			    stringpos = stringpos + sprintf(tempstring + stringpos,"%d",*((int*)pom->value));
			    break;
			case _double_:
                stringpos = stringpos + sprintf(tempstring + stringpos,"%g",*((double*)pom->value));
                break;
			default:
			    print_error(err_sem_kom);
		}
		pom = pom->next;
	}
	printf("%s",tempstring);
	free(tempstring);
}

//vestavěné fce pro práci s řetězci

int length(String s)
{
	int l = 0;
	while(*s++) l++;		// prochazi retezec, pokud neni '\0' je 1 a zaroven posouva na dalsi znak
	return l;
}

String substr(String s, int i, int n)
{
	String subs;		// podretezec
	int del = length(s);
	int k = i;

	if (i >= del || i <= 0 || n <= 0)
	{
		print_error(err_oth);
	}

	n = n+i;
	if (n > del) {		// pripad, kdy i + pocet ctenych znaku prekroci delku retezce
		n = del;
	}

	subs = (String) malloc(n-i+1);
	if (subs == NULL) {
		print_error(err_oth); // dyn. alokace pro podretezec
	}

	for (; i < n; i++) {		// cyklus pro kopirovani
		subs[i-k] = s[i];
	}
	subs[n-k] = '\0';		// vlozeni zarazky '\0' na konce podretezce

	return subs;
}

int compare(String s1, String s2)
{
    while(*s1 && *s2)                                   // prochazi retezce, pokud nejsou '\0' je 1 a zaroven posouva na dalsi znak
    {
		if(*s1 < *s2) return -1;                        // ordninalni hodnota znaku s1[i] je mensi nez hodnota znaku s2[i]
		if(*s1 > *s2) return 1;                         // ordninalni hodnota znaku s1[i] je vetsi nez hodnota znaku s2[i]
		s1++;                                           // posuny na dalsi znak
		s2++;
	}
	if(*s1) return 1;                                   // pripady, kdy je 1 retezec delsi nez druhy
	if(*s2) return -1;
	return 0;                                           // retezce jsou stejne
}


/*
int main()      //testovani
{
    String s1;
    String s2;

    //testovani fce sort
    printf("testovani fce sort:\n--------------------\n");
    String text = (String) malloc(sizeof(char) * 6);
    if (text == NULL)
		print_error(err_oth);

	strcpy(text, "ROMAN");
	printf("%s\n", text);
	sort(text);
	printf("%s\n", text);
	free(text);


    //testovani fce length
    printf("testovani fce length:\n--------------------\n");
    s1 = "delka";
    printf("length: %d\n", length(s1));
    printf("strlen: %d\n", (int)strlen(s1));


    //testovani fce compare
    printf("\ntestovani fce compare:\n--------------------\n");
    s1 = "delka";
    s2 = "delka";
    printf("stejne retezce: %d\n", compare(s1, s2));
    printf("stejne retezce: %d\n", strcmp(s1, s2));

    s1 = "delkaP";
    s2 = "delka";
    printf("prvni vetsi: %d\n", compare(s1, s2));
    printf("prvni vetsi: %d\n", strcmp(s1, s2));

    s1 = "delkA";
    s2 = "delka";
    printf("druhy mensi: %d\n", compare(s1, s2));
    printf("druhy mensi: %d\n", strcmp(s1, s2));

    s1 = "";
    s2 = "";
    printf("prazdny retezec: %d\n", compare(s1, s2));
    printf("prazdny retezec: %d\n", strcmp(s1, s2));


    //testovani fce substr
    printf("\ntestovani fce substr:\n--------------------\n");
    s1 = "lokomotiva";
    s2 = substr(s1, 4, 5);
    printf("bezne pouziti: %s\n", s2);
    free(s2);
    s2 = substr(s1, 4, 50);
    printf("n + i je delsi nez delka retezce: %s\n", s2);
    free(s2);

    //printf("i je vetsi/rovno nez delka retezce: %s - zde misto (null) skonci program s kodem \"10\"\n", substr(s1, 11, 5));


    //testovani readString
    printf("\ntestovani fce readString:\n--------------------\n");
//    printf("\nretezec: %s\n", readString());

    //testovani readInt
    printf("\ntestovani fce readInt:\n--------------------\n");
//    printf("\ncislo: %d\n", readInt());

    //testovani readDouble
    printf("\ntestovani fce readDouble:\n--------------------\n");
//    printf("\ncislo: %g\n", readDouble());

    //testovani fce concat
    printf("\ntestovani fce concat:\n--------------------\n");
    s1 = "konec ";
    s2 = "filma!";
    s1 = concat(s1, s2);
    printf("%s\n", s1);
    free(s1);


    //testovani fce find
    printf("\ntestovani fce find:\n--------------------\n");
    s1 = "labyrint";
    s2 = "rin";
    printf("index v retezci: %d\n", find(s1,s2));

    s1 = "labyrint";
    s2 = "neni!";
    printf("index v retezci: %d\n", find(s1,s2));

    s1 = "labyrint";
    s2 = "";
    printf("index v retezci: %d\n", find(s1,s2));

    s1 = "";
    s2 = "";
    printf("index v retezci: %d\n", find(s1,s2));

	//testovani fce print
	printf("\ntestovani fce print:\n--------------------\n");
    Ttoken **arrt = (Ttoken**) malloc(sizeof(Ttoken*) * 4);		//pole ukazatelu na tokeny
	if (arrt == NULL)
		print_error(err_oth);


    String val1 = (String) malloc(sizeof(char) * 16);
    if (val1 == NULL)
			print_error(err_oth);
    strcpy(val1, "Ja jsem string!");
    Ttoken *tok1;
    tok1 = (Ttoken*) malloc(sizeof(Ttoken));
	if (tok1 == NULL)
		print_error(err_oth);
    tok1->value = val1;
    tok1->type = con_string;


    int *val2 = (int*) malloc(sizeof(int));
    if (val2 == NULL)
			print_error(err_oth);
    *(int*) val2 = 12345;
    Ttoken *tok2;
    tok2 = (Ttoken*) malloc(sizeof(Ttoken));
	if (tok2 == NULL)
		print_error(err_oth);
    tok2->value = val2;
    tok2->type = con_int;

    double *val3 = (double*) malloc(sizeof(double));
    if (val3 == NULL)
			print_error(err_oth);
    *(double*) val3 = 1234555111e+265;
    Ttoken *tok3;
    tok3 = (Ttoken*) malloc(sizeof(Ttoken));
	if (tok3 == NULL)
		print_error(err_oth);
    tok3->value = val3;
    tok3->type = con_double;
	//-------------------------------------------------

    String val4 = (String) malloc(sizeof(char) * 9);
    if (val4 == NULL)
			print_error(err_oth);
    strcpy(val4,"televize");
    Ttoken *tok4;
    tok4 = (Ttoken*) malloc(sizeof(Ttoken));
	if (tok4 == NULL)
		print_error(err_oth);
    tok4->value = val4;
    tok4->type = con_string;


	arrt[0] = tok1;
	print(1, arrt);
	putchar('\n');
	arrt[0] = tok2;
	print(1, arrt);
	putchar('\n');
	arrt[0] = tok3;
	print(1, arrt);
	putchar('\n');
	putchar('\n');

	arrt[0] = tok1;
	arrt[1] = tok2;
	arrt[2] = tok3;
	arrt[3] = tok4;

	print(4, arrt);		// test konkatenace


	//testovani fce convert
	printf("\n\ntestovani fce convert:\n--------------------\n");
	s1 = convert(tok1);
	printf("%s\n", s1);
	free(s1);
	s1 = convert(tok2);
	printf("%s\n", s1);
	free(s1);
	s1 = convert(tok3);
	printf("%s\n", s1);
	free(s1);

	free(tok1->value);
	free(tok2->value);
	free(tok3->value);
	free(tok4->value);
	free(tok1);
	free(tok2);
	free(tok3);
	free(tok4);
	free(arrt);

    return 0;

}
*/

