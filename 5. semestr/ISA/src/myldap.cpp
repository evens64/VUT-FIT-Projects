// Soubor:		myldap.cpp
// Projekt:		LDAP server
// Autor:		Jakub Frýz (xfryzj01@stud.fit.vutbr.cz)

#include "myldap.h"

/*
 * http://beej.us/guide/bgnet/output/html/multipage/clientserver.html
 * z této stránky jsem se insiroval pro vytvoření serverové komunikace s klientem
 * + prezentace z předmětu ISA
 */

//	-----------------------------------------------------------------------------------------------
//	globální proměnné
//	-----------------------------------------------------------------------------------------------
char* file = NULL;				// cesta k souboru
int port = 389;					// cislo portu (nastavene vychozi)
std::ifstream db;				// stream

volatile sig_atomic_t stop;		// promenna pro signal handler

// proměnné pro komunikaci
int server_socket, client_socket;
struct sockaddr_in server_addr, client_addr;
socklen_t client_length;

// proměnná id procesu pro fork()
pid_t pid;

// formát výstupu
std::ios def_fmt(NULL);


int main(int argc, char **argv) {
//	-----------------------------------------------------------------------------------------------
//	uložení výchozího nastavení 'std::cout'
//	-----------------------------------------------------------------------------------------------
	def_fmt.copyfmt(std::cout);

//	-----------------------------------------------------------------------------------------------
//	nastavení handleru pro SIGINT signál
//	-----------------------------------------------------------------------------------------------
	struct sigaction a;
	a.sa_handler = sigint_handler;
	a.sa_flags = 0;
	sigemptyset(&a.sa_mask);
	sigaction(SIGINT, &a, NULL);

	stop = 0;

//	-----------------------------------------------------------------------------------------------
//	parsování argumentů pomocí 'getopt'
//	-----------------------------------------------------------------------------------------------
	int c;
	opterr = 0;
	while ((c = getopt(argc, argv, "hf:p:")) != -1) {
		switch (c) {
			case 'h':				// nápověda
				print_help();
				std::exit(OK);

			case 'f':				// soubor
				file = optarg;
				break;

			case 'p':				// port
				port = chararray2int(optarg);
				break;

			case '?':				// chyby
				if (optopt == 'f' || optopt == 'p')
					std::cerr << EMPH("ERROR: ") << TAB_JUMP << "Option '" << (char)optopt << "' requires an argument" << END_LINE;
				else if (std::isprint (optopt))
					std::cerr << EMPH("ERROR: ") << TAB_JUMP << "Unknown option '" << (char)optopt << "'" << END_LINE;
				else
					std::cerr << EMPH("ERROR: ") << TAB_JUMP << "Unknown option character '" << std::hex << optopt << "'" << END_LINE;
				std::exit(arg_err);

			default:
				abort();
		}
	}

//	-----------------------------------------------------------------------------------------------
//	kontrola, zda-li byl zadán soubor
//	-----------------------------------------------------------------------------------------------
	if (file == NULL) {
		std::cerr << "FILE not set!" << END_LINE;
		std::exit(arg_err);
	}

	std::cout << EMPH("INFO: ") << TAB_JUMP << "File '" << file << "' will be used as database" << END_LINE;
	std::cout << EMPH("INFO: ") << TAB_JUMP << "Port no. " << port << " will be used for listening" << END_LINE;

//	-----------------------------------------------------------------------------------------------
//	otevření souboru pro čtení
//	-----------------------------------------------------------------------------------------------
	db.open(file, std::ifstream::in);
	if (db.is_open()) {
		std::cout << EMPH("INFO: ") << TAB_JUMP << "File '" << file << "' opened for reading" << END_LINE;
	} else {
		std::cerr << EMPH("ERROR: ") << TAB_JUMP << "Unable to open file '" << file << "'" << END_LINE;
		std::exit(file_open_err);
	}

//	-----------------------------------------------------------------------------------------------
//	vytvoření socketu
//	-----------------------------------------------------------------------------------------------
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		std::cerr << EMPH("ERROR: ") << TAB_JUMP << "Socket establishing error" << END_LINE;
		std::exit(socket_err);
	} else {
		std::cout << EMPH("INFO: ") << TAB_JUMP << "Socket established" << END_LINE;
	}

//	-----------------------------------------------------------------------------------------------
//	přiřazení adresy hosta socketu
//	-----------------------------------------------------------------------------------------------
	memset((char *) &server_addr, 0, sizeof(server_addr));
	server_addr.sin_addr.s_addr = INADDR_ANY;		// automaticky se vyplni IP hosta
	server_addr.sin_family = AF_INET;				// rodina adres (AF_INET = internet domain sockets)
	server_addr.sin_port = htons(port);				// port pro naslouchání

	if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		std::cerr << EMPH("ERROR: ") << TAB_JUMP << "Binding error" << END_LINE;
		std::exit(bind_err);
	} else {
		std::cout << EMPH("INFO: ") << TAB_JUMP << "Binding completed" << END_LINE;
	}

//	-----------------------------------------------------------------------------------------------
//	poslouchání
//	-----------------------------------------------------------------------------------------------
	if (listen(server_socket, BACKLOG) != 0) {
		std::cerr << EMPH("ERROR: ") << TAB_JUMP << "Listening error" << END_LINE;
		std::exit(listen_err);
	} else {
		std::cout << EMPH("INFO: ") << TAB_JUMP << "Server is listening" << END_LINE;
	}

	client_length = sizeof(client_addr);
	while(!stop) {		// nekonečný loop, dokud SIGINT

		if (!socket_select()) {		// neblokující accept (pokud se nic neděje -> neblokuje)
			continue;
		}

		if (stop) break;			// kontrola, zda-li nepřišel signál SIGINT

		if ((client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_length)) < 0) {
			std::cerr << EMPH("ERROR: ") << TAB_JUMP << "Accept error" << END_LINE;
			std::exit(accept_err);
		} else {
			std::cout << EMPH("INFO: ") << TAB_JUMP << "Socket was succesfully accepted" << END_LINE;
		}

		std::cout << EMPH("INFO: ") << TAB_JUMP << "Server got connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << END_LINE;

		if ((pid = fork()) > 0) {			// parrent process
			std::cout << EMPH("INFO: ") << TAB_JUMP << FONT_UNLN << "PID:" << (long)getpid() << FONT_RESET << " - Closing client socket of parent process" << END_LINE;
			close(client_socket);
		} else if (pid == 0) {				// child process
			std::cout << EMPH("INFO: ") << TAB_JUMP << FONT_UNLN << "PID:" << (long)getpid() << FONT_RESET << " - Closing server socket of child process" << END_LINE;
			close(server_socket);

			// komunikace

			return_code ret = OK;
			bool while_end = false;				// ukončení do-while
			unsigned char buffer[BUFFER_SIZE];
			ldap_msg_t msg;
			int msg_size;

			do {			// dokud můžeš, komunikuj s klientem
				memset((unsigned char *) &buffer, 0, BUFFER_SIZE);

				msg_size = read(client_socket, buffer, BUFFER_SIZE);		// přijmi zprávu od klienta
				if (msg_size == -1) {
					while_end = true;
					ret = sock_read_err;
				}

				msg = decode_request(buffer, msg_size);						// dekóduj zprávu

				switch(msg.protocol_op) {									// podle typu zprávy -> udělej ...
					case bind_request_simple:		// BindRequest (simple)
						std::cout << EMPH("INFO: ") << TAB_JUMP << FONT_UNLN << "PID:" << (long)getpid() << FONT_RESET << " - Bind request from client" << END_LINE;

						msg.protocol_op = bind_response_simple;
						msg_size = encode_response(msg, buffer);
						if (msg_size == 0) {
							ret = encode_rsp_err;
							while_end = true;
							break;
						}

						msg_size = write(client_socket, buffer, msg_size);
						if (msg_size == -1) {
							ret = sock_write_err;
							while_end = true;
						}

						break;

					case bind_request_sasl:		// BindRequest (SASL) -> neotestováno
						std::cout << EMPH("INFO: ") << TAB_JUMP << FONT_UNLN << "PID:" << (long)getpid() << FONT_RESET << " - Bind request from client" << END_LINE;
						std::cerr << EMPH("ERROR: ") << TAB_JUMP << FONT_UNLN << "PID:" << (long)getpid() << FONT_RESET << " - SASL is not supported" << END_LINE;

						msg.protocol_op = bind_response_sasl;
						msg_size = encode_response(msg, buffer);
						if (msg_size == 0) {
							ret = encode_rsp_err;
							while_end = true;
							break;
						}

						msg_size = write(client_socket, buffer, msg_size);
						if (msg_size == -1) {
							ret = sock_write_err;
							while_end = true;
						}

						ret = bind_sasl_err;
						while_end = true;
						break;

					case search_request:		// SearchRequest
						std::cout << EMPH("INFO: ") << TAB_JUMP << FONT_UNLN << "PID:" << (long)getpid() << FONT_RESET << " - Search request from client" << END_LINE;

						// SearchResultEntry

						switch(dsew(buffer, msg_size)) {
							case succ:
								break;

							case succ_size_exc:
								break;

							case wrong_msg_err:
								break;

							case unknown_attr_err:
								break;

							case unsupp_err:
								break;

							case unexpect_err:
								break;

							default:
								break;
						}

						// SearchResultDone
						std::cout << EMPH("INFO: ") << TAB_JUMP << FONT_UNLN << "PID:" << (long)getpid() << FONT_RESET << " - Done searching" << END_LINE;

						msg.protocol_op = search_result_done;
						msg_size = encode_response(msg, buffer);
						if (msg_size == 0) {
							ret = encode_rsp_err;
							while_end = true;
							break;
						}

						msg_size = write(client_socket, buffer, msg_size);
						if (msg_size == -1) {
							ret = sock_write_err;
							while_end = true;
						}

						break;

					case unbind_request:		// UnbindRequest
						std::cout << EMPH("INFO: ") << TAB_JUMP << FONT_UNLN << "PID:" << (long)getpid() << FONT_RESET << " - Unbind request from client" << END_LINE;
						while_end = true;
						break;

					default:					// Ostatní nepodporované requesty od klienta
						std::cerr << EMPH("ERROR: ") << TAB_JUMP << FONT_UNLN << "PID:" << (long)getpid() << FONT_RESET << " - Unknown request from client" << END_LINE;
						ret = unknown_req_err;
						while_end = true;
						break;
				}

			} while (while_end == false);


			// komunikace - konec

			shutdown(client_socket, SHUT_RDWR);
			std::cout << EMPH("INFO: ") << TAB_JUMP << FONT_UNLN << "PID:" << (long)getpid() << FONT_RESET << " - Closing client socket of child process" << END_LINE;
			close(client_socket);
			std::cout << EMPH("INFO: ") << TAB_JUMP << FONT_UNLN << "PID:" << (long)getpid() << FONT_RESET << " - Ending child process" << END_LINE;
			std::exit(ret);
		} else {							// error
			std::cerr << EMPH("ERROR: ") << TAB_JUMP << "Fork failed" << END_LINE;
			std::exit(pid_err);
		}

	}

	wait(NULL);	// čekat dokud neskončí všechny 'child process'

//	-----------------------------------------------------------------------------------------------
//	ukončení socketu a uzavření souboru
//	-----------------------------------------------------------------------------------------------
	std::cout << EMPH("INFO: ") << TAB_JUMP << "Closing server socket" << END_LINE;
	shutdown(server_socket, SHUT_RDWR);
	close(server_socket);

	std::cout << EMPH("INFO: ") << TAB_JUMP << "Closing file '" << file << "'" << END_LINE;
	db.close();

	std::exit(OK);
}
