// Soubor:		myldap.h
// Projekt:		LDAP server
// Autor:		Jakub Frýz (xfryzj01@stud.fit.vutbr.cz)

#ifndef MYLDAP_H
#define MYLDAP_H


#include <arpa/inet.h>
#include <cctype>			// isprint
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>			// memset
#include <fstream>			// ifstream
#include <iomanip>			// setw
#include <iostream>			// cout, cerr
#include <netdb.h>
#include <string>
#include <sys/select.h>		// select
#include <sys/socket.h>		// socket, bind, listen, accept
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>			// getopt, fork

#include "definitions.h"
#include "fce_packs.h"

/* main fce */
int main(int argc, char **argv);


#endif
