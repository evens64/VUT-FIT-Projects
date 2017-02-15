// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Jakub Frýz (xfryzj01),
//				Filip Dostálík (xdosta46),
//				Michal Hrabovský (xhrabo08)

#include "scanner.h"

extern tBST BST;
extern Tqueue Q;

enum status {initial_status, reading_number, reading_id_keyword, reading_equal, reading_lesser, reading_greater, reading_exclamation, reading_string, escape_sequence, reading_oktal1, reading_oktal2, reading_slash, reading_comment,endof_comment};

Tqueue scanner(char* in) {
	FILE* code = fopen(in, "r");
	if (code == NULL) {
		print_error(err_oth);
	}
                                //fronta na tokeny
                                //inicializace fronty

	int dotcount = 0;			//counter pro tečky
	int i = 0;					//counter pro buffer
	char c;						//načítaný znak
	char buf[255];				//buffer
	int s = initial_status;					//stavová proměnná
    int charvalue;
    int semstatus = 0;
    TokenTypes lasttype;
    Ttoken * lastid;
    QLastInsert(&Q, _EOF_);
	while (1) {			//stavový automat
		c=fgetc(code);			//načtení znaku

		if (c == EOF) {			//kontrola konce souboru
			QLastInsert(&Q, _EOF_);
			break;
		}

		switch (s) {
			case initial_status:		//počáteční stav
				if (isspace(c)) {				//mezery
					s = initial_status;
				} else if (isdigit(c)) {		//čísla
					buf[i] = c;
					i++;
					s = reading_number;
				} else if (isalpha(c) || c == '$' || c == '_') {		//proměnné, řetězec
					buf[i] = c;
					i++;
					s = reading_id_keyword;
				} else switch (c) {
					case '/':		//	/
						s = reading_slash;
						i++;
						break;

					case ';':		//	;
						QLastInsert(&Q, semicolon);
						if (semstatus == 4) semstatus = 1;
						break;

					case '(':		//	(
						QLastInsert(&Q, lparenth);
						if (semstatus == 4){
                            semstatus = 5;
                            SemAnalFncDef(&BST,lastid,lasttype);
						}
						break;

					case ')':		//	)
						QLastInsert(&Q, rparenth);
						break;

					case '{':		//	{
						QLastInsert(&Q, lbraces);
						if (semstatus>4) semstatus++;
						break;

					case '}':		//	}
						QLastInsert(&Q, rbraces);
						if (semstatus>6) semstatus--;
						else if (semstatus == 6) {
                            semstatus = 1;
                            BSTPreviousLevel(&BST);
                        }
						break;

					case '=':		//	=
						buf[i] = c;
						i++;
						s = reading_equal;
						break;

					case '<':		//	<
						buf[i] = c;
						i++;
						s = reading_lesser;
						break;

					case '>':		//	>
						buf[i] = c;
						i++;
						s = reading_greater;
						break;

					case '"':		//	"
						s = reading_string;
						break;

					case '*':		//	*
						QLastInsert(&Q, mul);
						break;

					case '+':		//	+
						QLastInsert(&Q, add);
						break;

					case '-':		//	-
						QLastInsert(&Q, sub);
						break;

                    case ',':
                        QLastInsert(&Q,column);
                        break;

                    case '!':
                        s = reading_exclamation;
                        break;
				}	// end of switch(c)
				break;

			case reading_number:		//čísla
				if (isdigit(c)) {
					buf[i] = c;
					i++;
					s = reading_number;
				} else {
					if ((c == '.') || (c == ',')) {		// desetinná čísla
						if (dotcount == 0) {		//kontrola
							dotcount++;
							buf[i]=c;
							i++;
							s = reading_number;
						} else {
							print_error(err_lex);
						}
					} else {
						//overeni na nasledujici znak jestli je operator
						//tak odeslu token ale jestli je tam nepov. tak
						//chyba lexu
						if((c=='*') || (c=='/')||(c==';')||(c=='+')||(c=='-')||(c==')')||(isspace(c))){
							//dotcount==0 ? SendTok(buf,1) : SendTok(buf,2);
							if (dotcount == 0) {
								QLastInsertInt(&Q, atoi(buf));
							} else {
								QLastInsertDouble(&Q, atof(buf));
							}
							s = initial_status;
							i = 0;
							dotcount=0;
							//odeslani tokenu
							memset(buf, 0, sizeof(buf));
							ungetc(c, code);
						} else {
							print_error(err_lex);
						}
					}
				}
				break;

			case reading_id_keyword: 	//id & keywords
				if (isalnum(c) || c == '$' || c == '_' || c == '.') {
					buf[i] = c;
					i++;
					s = reading_id_keyword;
				} else {
					buf[i]='\0';
					if (strcmp(buf, "class") == 0) {
						QLastInsert(&Q, _class_);
						if (semstatus == 1) BSTPreviousLevel(&BST);
						semstatus = 2;
					} else if (strcmp(buf, "EOF") == 0) {
						QLastInsert(&Q, _EOF_);
					} else if (strcmp(buf, "static") == 0) {
						QLastInsert(&Q, _static_);
						lasttype = _static_;
						if (semstatus == 1) semstatus = 3;
					} else if (strcmp(buf, "void") == 0) {
						QLastInsert(&Q, _void_);
						lasttype = _void_;
					} else if (strcmp(buf, "int") == 0) {
						QLastInsert(&Q, _int_);
						lasttype = _int_;
					} else if (strcmp(buf, "String") == 0) {
						QLastInsert(&Q, _String_);
						lasttype = _String_;
					//} else if (strcmp(buf, "boolean") == 0) {
					//	QLastInsert(&Q, _boolean_);
					} else if (strcmp(buf, "double") == 0) {
						QLastInsert(&Q, _double_);
						lasttype = _double_;
					} else if (strcmp(buf, "if") == 0) {
						QLastInsert(&Q, _if_);
					} else if (strcmp(buf, "else") == 0) {
						QLastInsert(&Q, _else_);
					} else if (strcmp(buf, "while") == 0) {
						QLastInsert(&Q, _while_);
					} else if (strcmp(buf, "return") == 0) {
						QLastInsert(&Q, _return_);
					} else if (strcmp(buf, "ifj16.readInt") == 0) {
						QLastInsert(&Q, _ifj16readInt_);
					} else if (strcmp(buf, "ifj16.readDouble") == 0) {
						QLastInsert(&Q, _ifj16readDouble_);
					} else if (strcmp(buf, "ifj16.readString") == 0) {
						QLastInsert(&Q, _ifj16readString_);
					} else if (strcmp(buf, "ifj16.print") == 0) {
						QLastInsert(&Q, _ifj16print_);
					} else if (strcmp(buf, "ifj16.length") == 0) {
						QLastInsert(&Q, _ifj16length_);
					} else if (strcmp(buf, "ifj16.substr") == 0) {
						QLastInsert(&Q, _ifj16substr_);
					} else if (strcmp(buf, "ifj16.compare") == 0) {
						QLastInsert(&Q, _ifj16compare_);
					} else if (strcmp(buf, "ifj16.find") == 0) {
						QLastInsert(&Q, _ifj16find_);
					} else if (strcmp(buf, "ifj16.sort") == 0) {
						QLastInsert(&Q, _ifj16sort_);
					} else {
						QLastInsertID(&Q, buf);
						if (semstatus == 2){
                            SemAnalClassEnter(&BST,QPeekLast(&Q));
                            semstatus = 1;
						}
						else if (semstatus == 3){
                            lastid = QPeekLast(&Q);
                            semstatus = 4;
						}
						else if (semstatus == 5){
                            SemAnalAddArg(&BST,QPeekLast(&Q),lasttype);
						}
					}

					memset(buf, 0, sizeof(buf));
					i = 0;
					s = initial_status;
					ungetc(c, code);
				}
				break;

			case reading_equal:		// == or =
				if (c == '=') {
					QLastInsert(&Q, eq);	// ==
					memset(buf, 0, sizeof(buf));
				} else {
					QLastInsert(&Q, ass);	// =
					memset(buf, 0, sizeof(buf));
					ungetc(c, code);
					if (semstatus == 4) semstatus = 1;
				}
				s = initial_status;
				i = 0;
				break;

			case reading_lesser:		// <= or <
				if (c == '='){
					QLastInsert(&Q, lseq);	// <=
					memset(buf, 0, sizeof(buf));
				} else {
					QLastInsert(&Q, ls);	// <
					memset(buf, 0, sizeof(buf));
					ungetc(c, code);	//vrácení znaku
				}
				s = initial_status;
				i = 0;
				break;

			case reading_greater:		// >= or >
				if (c == '='){
					QLastInsert(&Q, gteq);	// >=
					memset(buf, 0, sizeof(buf));
				} else {
					QLastInsert(&Q, gt);	// >
					memset(buf,0,sizeof(buf));
					ungetc(c, code);
				}
				i = 0;
				s = initial_status;
				break;

            case reading_exclamation:
                if (c=='='){
                    QLastInsert(&Q, noteq);
                }
                else print_error(err_lex);
                s = initial_status;
                break;

			case reading_string:		// řetězce
				if (c == '"') {
					QLastInsertString(&Q, buf);
					i = 0;
					s = initial_status;
					memset(buf, 0, sizeof(buf));
				} else if (c=='\\'){
                    s = escape_sequence;
				} else {
					buf[i] = c;
					i++;
					s = reading_string;
				}
				break;
            case escape_sequence:
            if(isalpha(c))
                {
                    switch (c)
                    {
                        case 'n':
                            buf[i] = '\n';
                            break;
                        case 't':
                            buf[i] = '\t';
                            break;
                        default:
                            print_error(err_lex);
                    }
                        i++;
                        s = reading_string;
                }
                else if (c == '"'){
                    buf [i] = '\"';
                    i++;
                    s=reading_string;
                    break;
                }
                else if (c>= '0' && c <='3')
                {
                    charvalue = (c-'0')*8*8;
                    s=reading_oktal1;
                }
                else print_error(err_lex);
                break;

            case reading_oktal1:
                if (c>= '0' && c <='7')
                {
                    charvalue = charvalue + (c-'0')*8;
                    s=reading_oktal2;
                }
                else print_error(err_lex);
                break;

            case reading_oktal2:
                if (c>= '0' && c <='7')
                {
                    charvalue = charvalue + (c-'0');
                    buf[i]=charvalue%255;
                    i++;
                    s=reading_string;
                }
                else print_error(err_lex);
                break;

			case reading_slash:		// komentáře nebo /
				if (((c == '*') || (c == '/')) && (i == 1)) {
					s = reading_comment;
					i++;
				} else {
					// jedna se o deleni
					QLastInsert(&Q, dv);
					i = 0;
					s = initial_status;
					ungetc(c, code);
				}
				if ((i == 2) && (c=='/')){
					//zacatek radkoveho komentu
					i = 0;
					s = reading_comment;
				}
				if ((i > 1) && (c == '*')) {
					i++;
					s = reading_comment;
				}
				break;

			case reading_comment:		// komentáře
				if (i == 0) {
					if (c == '\n') {	// průchod do konce řádku
						s = initial_status;
					}
				} else {
					//hledam * a /
					if (c != '*') {		// /*	*/
						s = reading_comment;
					} else {
						s = endof_comment;
					}
				}
				break;

			case endof_comment:		// komentáře
				if (c == '/') {		//konec komentu
					i = 0;
					s = initial_status;
				} else {
					s = reading_comment;
				}
				break;

		}	//end of switch(s)
	}	// end of while(1)
    QLastInsert(&Q, _EOF_);
	fclose(code);

	return Q;
}
