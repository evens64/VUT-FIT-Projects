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

// define
#define BUFFER_SIZE 1024
#define END "\n"
#define ENDW "\r\n"
#define MSG_ERROR "\033[1;31mERROR:    \033[0m"
#define MSG_INFO  "\033[1;32mINFO:     \033[0m"
#define MSG_WARN  "\033[1;33mWARN:     \033[0m"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 21011

// functions
int convert(std::string s, char* c);


int main(int argc, char* argv[]) {

// ------------------------------------------------------------------------------------------------
//	argumenty
// ------------------------------------------------------------------------------------------------

	if (argc != 5) {
		std::cerr << MSG_WARN << "chat_client -i IP_address -u username" << END;
		exit(1);
	}

	std::string args[4] = {argv[1], argv[2], argv[3], argv[4]};		// převod argumentů do 'string'
	std::string username = "";
	int ip_id = 0;
	bool check[2] = {false, false};									// ip, username

	for (int i = 0; i < 4; i += 2) {								// 2 cykly (0,2)
		if (args[i] == "-i") {										// kontrola na argument '-i'
			if (check[0] != true) {										// kontrola, zda-li už nebyl argument zadán
				check[0] = true;
				ip_id = i + 2;
			} else {												// argument už byl zadán -> chyba
				std::cerr << MSG_ERROR << "duplicate argument '" << args[i] << "'" << END;
			}
		} else if (args[i] == "-u") {								// kontrola na argument '-u'
			if (check[1] != true) {										// kontrola, zda-li už nebyl argument zadán
				check[1] = true;
				username = args[i+1];
			} else {												// argument už byl zadán -> chyba
				std::cerr << MSG_ERROR << "duplicate argument '" << args[i] << "'" << END;
			}
		} else {													// je zadán špatný argument -> chyba
			std::cerr << MSG_ERROR << "wrong argument '" << args[i] << "'" << END;
			return -1;
		}
	}

	std::cout << MSG_INFO << "IP: " << argv[ip_id] << ", UN: " << username << END;

// ------------------------------------------------------------------------------------------------
//	spojení
// ------------------------------------------------------------------------------------------------

	int client = socket(AF_INET, SOCK_STREAM, 0);
	if (client < 0) {
		std::cerr << MSG_ERROR << "socket establishing error" << END;
		exit(1);
	}

	std::cout << MSG_INFO << "socket has been created" << END;


	// funkce inet_pton() nevezme string jako argument, proto se musí převést na char array
	// char ip_c[20];
	// bzero(ip_c, sizeof(ip_c));
	// strncpy(ip_c, ip.c_str(), sizeof(ip_c));

	struct sockaddr_in server_addr;
	//struct sockaddr_in client_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	//server_addr.sin_addr.s_addr = inet_addr(argv[ip_id]);
	if (inet_pton(AF_INET, argv[ip_id], &server_addr.sin_addr) != 1) {
		std::cerr << MSG_ERROR << "non-valid ip address '" << argv[ip_id] << "'" << END;
		exit(1);
	}

	if (connect(client, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) == 0) {
		std::cout << MSG_INFO << "connection to the server " << argv[ip_id] << ":" << SERVER_PORT << " as " << username << END;
	} else {
		std::cerr << MSG_ERROR << "connection error" << END;
		exit(1);
	}



	std::string msg;
	std::string line;
	char buffer[BUFFER_SIZE];
	bzero(buffer, BUFFER_SIZE);

	msg = username + " logged in" + ENDW;
	if (convert(msg, buffer) == 0) {
		send(client, buffer, BUFFER_SIZE, 0);
		// std::cout << MSG_INFO << buffer << END;
	} else {
		std::cerr << MSG_ERROR << "convert errror" << END;
	}

	while (true) {
		line = "";
		
		std::getline(std::cin, line);

		msg = username + ": " + line + ENDW;

		if (convert(msg, buffer) == 0) {
			send(client, buffer, BUFFER_SIZE, 0);
			// std::cout << MSG_INFO << buffer << END;
		} else {
			std::cerr << MSG_ERROR << "convert errror" << END;
		}
	}

	msg = username + " logged out" + ENDW;
	if (convert(msg, buffer) == 0) {
		send(client, buffer, BUFFER_SIZE, 0);
		// std::cout << MSG_INFO << buffer << END;
	} else {
		std::cerr << MSG_ERROR << "convert errror" << END;
	}

	std::cout << MSG_INFO << "connection terminated" << END;

	close(client);

	return 0;
}

int convert(std::string s, char* c) {
	bzero(c, BUFFER_SIZE);
	strncpy(c, s.c_str(), BUFFER_SIZE);
	c[BUFFER_SIZE - 1] = 0;
	return 0;
}
