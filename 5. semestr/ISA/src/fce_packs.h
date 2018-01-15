// Soubor:		fce_pack.h
// Projekt:		LDAP server
// Autor:		Jakub Frýz (xfryzj01@stud.fit.vutbr.cz)

#ifndef fce_packs_H
#define fce_packs_H


#include <cerrno>
#include <climits>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#include "definitions.h"


/* externí proměnné */
extern volatile sig_atomic_t stop;
extern int client_socket;
extern int server_socket;
extern std::ios def_fmt;
extern std::ifstream db;


/* 01 - výpis nápovědy */
void print_help(void);

/* 02 - pomocné fce */
int chararray2int(char* text);
bool in_range(int x, int a, int b);

/* 03 */
void sigint_handler(int signum);
int socket_select(void);

ldap_msg_t decode_request(unsigned char* msg, int msg_size);
int encode_response(ldap_msg_t r, unsigned char* buffer);

dsew_ret dsew(unsigned char* msg, int msg_size);
dsew_ret search_db(ldap_search_t r);

void build_response(ldap_record_t s, ldap_search_t r);
int build_attributes(unsigned char* r, int i, char* cn, char* mail);
int build_attribute(unsigned char* r, int i, char* desc, char* value);


#endif
