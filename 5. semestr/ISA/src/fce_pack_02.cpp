// Soubor:		fce_pack_02.cpp
// Projekt:		LDAP server
// Autor:		Jakub Frýz (xfryzj01@stud.fit.vutbr.cz)

#include "fce_packs.h"

// prevede 'char array' (pole znaku) na 'int'
int chararray2int(char* str) {
	char* str_end;
	errno = 0;
	long int val = strtol(str, &str_end, 10);
	if (*str_end) {
		std::cerr << EMPH("ERROR: ") << "'" << str << "' not a number" << END_LINE;
		std::exit(conv_err);
	}
	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0)) {	// overflow/underflow
		std::cerr << EMPH("ERROR: ") << "overflow/underflow error" << END_LINE;
		std::exit(conv_err);
	}
	if (val > INT_MAX || val < INT_MIN) {
		std::cerr << EMPH("ERROR: ") << "'" << val << "' too big for 'int'" << END_LINE;
		std::exit(conv_err);
	}
	return (int)val;
}

// zkontroluje, zda-li je 'x' v rozsahu <a;b>
bool in_range(int x, int a, int b) {
	return (x >= a && x <= b) ? true : false;
}
