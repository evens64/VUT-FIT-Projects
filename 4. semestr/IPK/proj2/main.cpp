// IPK Projekt 2
// chat_client -i IP_address -u username

// include
#include <arpa/inet.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// define variables
#define BUFFER_SIZE 1024
#define END "\n"
#define ENDW "\r\n"
#define MSG_ERROR "\033[1;31mERROR:    \033[0m"
#define MSG_INFO  "\033[1;32mINFO:     \033[0m"
#define MSG_WARN  "\033[1;33mWARN:     \033[0m"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 21011

// define macros
#define UNUSED(...) (void)(__VA_ARGS__)

// functions
int convert(std::string s, char* c);
void sigint_handler(int signal);
int kbhit();

// thread functions
void *chat_recv(void *threadarg);

//global variables
int client;
std::string username;
// struct sockaddr_in client_addr;
struct sockaddr_in server_addr;
volatile sig_atomic_t stop;


int main(int argc, char* argv[]) {

// ------------------------------------------------------------------------------------------------
//	signál SIGINT
// ------------------------------------------------------------------------------------------------

	if (signal(SIGINT, sigint_handler) == SIG_ERR) {
		std::cerr << MSG_ERROR << "cannot catch SIGINT" << END;
		exit(EXIT_FAILURE);
	}

	stop = 0;

// ------------------------------------------------------------------------------------------------
//	argumenty
// ------------------------------------------------------------------------------------------------

	if (argc != 5) {
		std::cerr << MSG_WARN << "chat_client -i IP_address -u username" << END;
		exit(1);
	}

	std::string args[4] = {argv[1], argv[2], argv[3], argv[4]};		// převod argumentů do 'string'
	int ip_id = 0;
	bool check[2] = {false, false};									// ip, username

	for (int i = 0; i < 4; i += 2) {								// 2 cykly (0,2)
		if (args[i] == "-i") {										// kontrola na argument '-i'
			if (check[0] != true) {										// kontrola, zda-li už nebyl argument zadán
				check[0] = true;
				ip_id = i + 2;
			} else {												// argument už byl zadán -> chyba
				std::cerr << MSG_ERROR << "duplicate argument '" << args[i] << "'" << END;
				exit(EXIT_FAILURE);
			}
		} else if (args[i] == "-u") {								// kontrola na argument '-u'
			if (check[1] != true) {										// kontrola, zda-li už nebyl argument zadán
				check[1] = true;
				username = args[i+1];
			} else {												// argument už byl zadán -> chyba
				std::cerr << MSG_ERROR << "duplicate argument '" << args[i] << "'" << END;
				exit(EXIT_FAILURE);
			}
		} else {													// je zadán špatný argument -> chyba
			std::cerr << MSG_ERROR << "wrong argument '" << args[i] << "'" << END;
			exit(EXIT_FAILURE);
		}
	}

#ifdef info
	std::cout << MSG_INFO << "IP: " << argv[ip_id] << ", UN: " << username << END;
#endif

// ------------------------------------------------------------------------------------------------
//	spojení
// ------------------------------------------------------------------------------------------------

	client = socket(AF_INET, SOCK_STREAM, 0);						// tvorba socketu
	if (client < 0) {
		std::cerr << MSG_ERROR << "socket establishing error" << END;
		exit(EXIT_FAILURE);
	}

#ifdef info
	std::cout << MSG_INFO << "socket has been created" << END;
#endif

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	if (inet_pton(AF_INET, argv[ip_id], &server_addr.sin_addr) != 1) {
		std::cerr << MSG_ERROR << "non-valid ip address '" << argv[ip_id] << "'" << END;
		exit(EXIT_FAILURE);
	}

	if (connect(client, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) != 0) {		// připojení
		std::cerr << MSG_ERROR << "connection error" << END;
		exit(EXIT_FAILURE);
	}

#ifdef info
	else {
		std::cout << MSG_INFO << "connection to the server " << argv[ip_id] << ":" << SERVER_PORT << " as " << username << END;
	}
#endif

// ------------------------------------------------------------------------------------------------
//	proměnné ke komunikaci
// ------------------------------------------------------------------------------------------------

	std::string msg;
	std::string line;
	char buffer[BUFFER_SIZE];
	bzero(buffer, BUFFER_SIZE);


// ------------------------------------------------------------------------------------------------
//	zpráva přihlášení
// ------------------------------------------------------------------------------------------------

	msg = username + " logged in" + ENDW;
	if (convert(msg, buffer) == 0) {
		send(client, buffer, BUFFER_SIZE, 0);
	} else {
		shutdown(client, SHUT_RD);
		std::cerr << MSG_ERROR << "convert errror" << END;
		exit(EXIT_FAILURE);
	}


// ------------------------------------------------------------------------------------------------
//	vlákno (tvorba)
// ------------------------------------------------------------------------------------------------

	pthread_t receiver;
	void *result;

	int pthread_val = pthread_create(&receiver, NULL, &chat_recv, NULL);
	if (pthread_val != 0) {
		shutdown(client, SHUT_RD);
		std::cerr << MSG_ERROR << "thread not created" << END;
		exit(EXIT_FAILURE);
	}

// ------------------------------------------------------------------------------------------------
//	komunikace
// ------------------------------------------------------------------------------------------------

	while (!stop) {

		while (!kbhit()) {											// neblokující čtení ze vstupu
			continue;
		}

		if (stop) {													// kontrola
			break;
		}

		line = "";

		std::getline(std::cin, line);

		if (line == "")												// neodesílání prázdných zpráv
			continue;

		msg = username + ": " + line + ENDW;

		if (convert(msg, buffer) == 0) {
			send(client, buffer, BUFFER_SIZE, 0);
		} else {
			shutdown(client, SHUT_RD);
			std::cerr << MSG_ERROR << "convert errror" << END;
			exit(EXIT_FAILURE);
		}
	}

	std::cout << END;

	shutdown(client, SHUT_RD);										// ukončení příjímání zpráv

// ------------------------------------------------------------------------------------------------
//	vlákno (konec)
// ------------------------------------------------------------------------------------------------

	pthread_val = pthread_cancel(receiver);							// žádost o ukončení vlákna
	if (pthread_val != 0) {
		shutdown(client, SHUT_RD);
		std::cerr << MSG_ERROR << "cancellation request error" << END;
		exit(EXIT_FAILURE);
	}

#ifdef info
	else {
		std::cout << MSG_INFO << "cancellation request" << END;
	}
#endif

	pthread_val = pthread_join(receiver, &result);					// počkání na ukončení
	if (pthread_val != 0) {
		shutdown(client, SHUT_RD);
		std::cerr << MSG_ERROR << "joining thread error" << END;
		exit(EXIT_FAILURE);
	}

#ifdef info
	else {
		std::cout << MSG_INFO << "joining thread" << END;
	}
#endif

	if (result != PTHREAD_CANCELED) {								// ukončil se?
		shutdown(client, SHUT_RD);
		std::cerr << MSG_ERROR << "cancellation thread error" << END;
		exit(EXIT_FAILURE);
	}

#ifdef info
	else {
		std::cout << MSG_INFO << "thread canceled" << END;
	}
#endif

// ------------------------------------------------------------------------------------------------
//	zpráva odhlášení
// ------------------------------------------------------------------------------------------------

	msg = username + " logged out" + ENDW;
	if (convert(msg, buffer) == 0) {
		send(client, buffer, BUFFER_SIZE, 0);
	} else {
		std::cerr << MSG_ERROR << "convert errror" << END;
	}

#ifdef info
	std::cout << MSG_INFO << "connection terminated" << END;
#endif

	close(client);													// ukončení socketu

	return 0;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
//	moje funkce
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
//	SIGINT signal handler
// ------------------------------------------------------------------------------------------------

void sigint_handler(int signal) {
	if (signal == SIGINT) {
		stop = 1;
	}
}

// ------------------------------------------------------------------------------------------------
//	neblokující čtení ze vstupu
// ------------------------------------------------------------------------------------------------

int kbhit() {
	struct timeval tv;
	fd_set fds;

	// timeout 2.5 sekundy
	tv.tv_sec = 2;
	tv.tv_usec = 500000;

	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);

	select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);

	return FD_ISSET(STDIN_FILENO, &fds);
}

// ------------------------------------------------------------------------------------------------
//	převed stringu na char array
// ------------------------------------------------------------------------------------------------

int convert(std::string s, char* c) {
	bzero(c, BUFFER_SIZE);						// vynulovat buffer
	strncpy(c, s.c_str(), BUFFER_SIZE);			// překopírovat obsah stringu do char array
	c[BUFFER_SIZE - 1] = 0;						// preventivně nastavit poslední znak na 0
	return 0;
}

// ------------------------------------------------------------------------------------------------
//	vlákno pro příjem zpráv
// ------------------------------------------------------------------------------------------------

void *chat_recv(__attribute__ ((unused)) void *threadarg) {

	char buffer[BUFFER_SIZE];					// buffer na příchozí zprávu

	while(!stop) {								// dokud nepřišel SIGINT signál
		bzero(buffer, BUFFER_SIZE);				// vynuluj buffer
		recv(client, buffer, BUFFER_SIZE, 0);	// přijmi zprávu
		std::cout << buffer;					// vypiš na stdout
	}

	pthread_exit(EXIT_SUCCESS);					// ukonči

}
