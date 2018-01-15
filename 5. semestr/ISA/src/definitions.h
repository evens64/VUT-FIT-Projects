// Soubor:		definitions.h
// Projekt:		LDAP server
// Autor:		Jakub Frýz (xfryzj01@stud.fit.vutbr.cz)

#ifndef DEFINITIONS_H
#define DEFINITIONS_H


#include <iomanip>
#include <iostream>

#define END_LINE std::endl
#define TAB_JUMP "\t"
#define SET_WIDTH(x) std::setw(x) << std::left

#define FONT_BOLD "\033[1m"
#define FONT_UNLN "\033[4m"
#define FONT_RESET "\033[0m"

#define SIZE_LIMIT 10
#define BUFFER_SIZE 2048
#define BACKLOG 1

#define EMPH(x) FONT_BOLD x FONT_RESET
#define UNLN(x) FONT_UNLN x FONT_RESET
#define STRING(x) #x

/* enumerated */
enum return_code {
	OK = 0,
	arg_err,			// chyba při zpracovávání argumentů
	conv_err,			// chyba při konverzi mezi typy
	file_open_err,		// chyba při otevření souboru
	signal_err,
	socket_err,
	bind_err,
	listen_err,
	accept_err,
	pid_err,
	sock_read_err,
	sock_write_err,
	unknown_req_err,
	bind_sasl_err,
	encode_rsp_err
};

enum ldap_msg_type {
	unknown_request = 0,
	bind_request_simple,
	bind_request_sasl,
	bind_response_simple,
	bind_response_sasl,
	search_request,
	search_result_entry,
	search_result_done,
	unbind_request
};

enum ldap_filter {
	unknown_f = 0,
	present_f,
	equal_f,
	substring_f,
	substring_ini_f,
	substring_any_f,
	substring_fin_f
};

enum dsew_ret {
	succ = 0,
	succ_size_exc,
	wrong_msg_err,
	unknown_attr_err,
	unsupp_err,
	unexpect_err
};

/* types */
typedef struct {
	unsigned int	message_id;
	ldap_msg_type	protocol_op;
} ldap_msg_t;


typedef struct {
	unsigned int	message_id;
	char			object_name[128];
	unsigned int	scope;								// 0 - base, 1 - singleLevel, 2 - wholeSubtree
	unsigned int	deref_aliases;						// 0-3
	unsigned int	size_limit;
	unsigned int	time_limit;
	bool			types_only;
	struct {
		char			desc[128];						// cn, mail, ...
		char			value[128];
		bool			no;								// true = not
		ldap_filter		type;
	}				filter;
} ldap_search_t;

typedef struct {
	char*			cn;			// cn, CommonName
	char*			uid;		// uid, UserID
	char*			mail;		// mail
} ldap_record_t;


#endif
