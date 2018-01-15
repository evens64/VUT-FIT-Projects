// Soubor:		fce_pack_01.cpp
// Projekt:		LDAP server
// Autor:		Jakub Frýz (xfryzj01@stud.fit.vutbr.cz)

#include "fce_packs.h"


// vypise napovedu ve formatu manualu 'man'
void print_help(void) {
	std::cout << FONT_BOLD << "NAME" << FONT_RESET << END_LINE;
	std::cout << TAB_JUMP << "myldap - LDAPv2 server" << END_LINE;
	std::cout << END_LINE;

	std::cout << FONT_BOLD << "SYNOPSIS" << FONT_RESET << END_LINE;
	std::cout << TAB_JUMP << "myldap -f <file> {-p <port>} {-h}" << END_LINE;
	std::cout << END_LINE;

	std::cout << FONT_BOLD << "DESCRIPTION" << FONT_RESET << END_LINE;
	std::cout << TAB_JUMP << "LDAPv2 server" << END_LINE;
	std::cout << END_LINE;

	std::cout << FONT_BOLD << "OPTIONS" << FONT_RESET << END_LINE;
	std::cout << TAB_JUMP << "-f <file>" << TAB_JUMP << "cesta k souboru ve formátu CSV" << END_LINE;
	std::cout << END_LINE;
	std::cout << TAB_JUMP << "-h" << TAB_JUMP TAB_JUMP << "vypíše nápovědu" << END_LINE;
	std::cout << END_LINE;
	std::cout << TAB_JUMP << "-p <port>" << TAB_JUMP << "specifikuje port (výchozí je 389)" << END_LINE;
	std::cout << END_LINE;

	std::cout << FONT_BOLD << "EXAMPLES" << FONT_RESET << END_LINE;
	std::cout << TAB_JUMP << "myldap -f db.csv"  << END_LINE;
	std::cout << TAB_JUMP << "myldap -f db.csv -p 350"  << END_LINE;
	std::cout << TAB_JUMP << "myldap -h"  << END_LINE;
	std::cout << END_LINE;

	std::cout << FONT_BOLD << "SEE ALSO" << FONT_RESET << END_LINE;
	std::cout << TAB_JUMP << "dokumentaci ve složce 'doc'"  << END_LINE;
	std::cout << END_LINE;

	std::cout << FONT_BOLD << "BUGS" << FONT_RESET << END_LINE;
	std::cout << TAB_JUMP << "'and' s 'or' nejsou podporovány" << END_LINE;
	std::cout << TAB_JUMP << "funguje pouze simple bind (ldapseach -x)" << END_LINE;
	std::cout << TAB_JUMP << "při SearchResultEntry nejsou ošetřeny některé chyby" << END_LINE;
	std::cout << TAB_JUMP << "(aplikace odešle klientu SearchResultDone s návratovou hodnotou 0)" << END_LINE;
	std::cout << END_LINE;

	std::cout << FONT_BOLD << "AUTHOR" << FONT_RESET << END_LINE;
	std::cout << TAB_JUMP << "Jakub Frýz (xfryzj01@stud.fit.vutbr.cz)" << END_LINE;
	std::cout << END_LINE;

	std::cout << FONT_BOLD << "COPYRIGHT" << FONT_RESET << END_LINE;
	std::cout << END_LINE;
}
