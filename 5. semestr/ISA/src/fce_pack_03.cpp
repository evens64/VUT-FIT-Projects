// Soubor:		fce_pack_03.cpp
// Projekt:		LDAP server
// Autor:		Jakub Frýz (xfryzj01@stud.fit.vutbr.cz)

#include "fce_packs.h"


//	-----------------------------------------------------------------------------------------------
//	SIGINT signál handler
//	-----------------------------------------------------------------------------------------------
void sigint_handler(int signum) {
	if (signum == SIGINT) {
		std::cout << END_LINE << EMPH("INFO: ") << TAB_JUMP << "Interrupt signal SIGINT received" << END_LINE;
		stop = 1;
	}
}

//	-----------------------------------------------------------------------------------------------
//	neblokující 'accept'
//	-----------------------------------------------------------------------------------------------
int socket_select(void) {
	struct timeval tv;
	fd_set fds;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO(&fds);
	FD_SET(server_socket, &fds);

	select(server_socket + 1, &fds, NULL, NULL, &tv);

	return FD_ISSET(server_socket, &fds);
}

//	-----------------------------------------------------------------------------------------------
//	dekódování LDAP zprávy (hlavička je neměnná)
//	-----------------------------------------------------------------------------------------------
ldap_msg_t decode_request(unsigned char* msg, int msg_size) {

	/*
	// výpis zprávy v posloupnosti hex čísel
	std::cout << EMPH("MSG (hex): ") << TAB_JUMP;
	for (int i = 0; i < msg_size; i++) {
		std::cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)msg[i] << " ";
	}
	std::cout << std::dec << END_LINE;
	std::cout.copyfmt(def_fmt);
	//*/

// -- decoding --

	ldap_msg_t r;
	memset(&r, 0, sizeof(r));

	if (msg[0] != 0x30 && msg[1] != msg_size - 2) {
		r.protocol_op = unknown_request;
		return r;
	}

	r.message_id = msg[4];

	switch(msg[5]) {
		case 0x60:		// BindRequest
			switch(msg[12]) {
				case 0x80:
					r.protocol_op = bind_request_simple;
					break;

				case 0xA3:
					r.protocol_op = bind_request_sasl;
					break;

				default:
					r.protocol_op = unknown_request;
					break;
			}
			break;

		case 0x42:		// UnbindRequest
			r.protocol_op = unbind_request;
			break;

		case 0x63:		// SearchRequest
			r.protocol_op = search_request;
			break;

		default:
			r.protocol_op = unknown_request;
			break;
	}

	return r;
}

// pouze BindResponse, SearchResultDone
int encode_response(ldap_msg_t r, unsigned char* buffer) {
	memset((unsigned char *) buffer, 0, BUFFER_SIZE);

	switch(r.protocol_op) {
		case bind_response_simple:

			buffer[0] =		(char)0x30;				// SEQUENCE
			buffer[1] =		(char)0x0c;				// length:	12
			buffer[2] =		(char)0x02;				// INTEGER				-> messageID
			buffer[3] =		(char)0x01;				// length:	01
			buffer[4] =		(char)r.message_id;		// value:	01			-> (example)
			buffer[5] =		(char)0x61;				// BindResponse			-> protocolOp
			buffer[6] =		(char)0x07;				// length:	07
			buffer[7] =		(char)0x0a;				// ENUMERATED			-> resultCode
			buffer[8] =		(char)0x01;				// length:	01
			buffer[9] =		(char)0x00;				// value:	00			-> success
			buffer[10] =	(char)0x04;				// OCTET STRING
			buffer[11] =	(char)0x00;				// length:	00
			buffer[12] =	(char)0x04;				// OCTET STRING
			buffer[13] =	(char)0x00;				// length:	00

			return 14 * sizeof(char);
			break;

		case bind_response_sasl:

			buffer[0] =		(char)0x30;				// SEQUENCE
			buffer[1] =		(char)0x0c;				// length:	12
			buffer[2] =		(char)0x02;				// INTEGER				-> messageID
			buffer[3] =		(char)0x01;				// length:	01
			buffer[4] =		(char)r.message_id;		// value:	01			-> (example)
			buffer[5] =		(char)0x61;				// BindResponse			-> protocolOp
			buffer[6] =		(char)0x07;				// length:	07
			buffer[7] =		(char)0x0a;				// ENUMERATED			-> resultCode
			buffer[8] =		(char)0x01;				// length:	01
			buffer[9] =		(char)0x00;				// value:	07			-> authMethodNotSupported
			buffer[10] =	(char)0x04;				// OCTET STRING
			buffer[11] =	(char)0x00;				// length:	00
			buffer[12] =	(char)0x04;				// OCTET STRING
			buffer[13] =	(char)0x00;				// length:	00

			return 14 * sizeof(char);

		case search_result_entry:					// ukázka pro testovaní komunikace


			return 75 * sizeof(char);
			break;

		case search_result_done:

			buffer[0] =		(char)0x30;				// SEQUENCE
			buffer[1] =		(char)0x0c;				// length:	12
			buffer[2] =		(char)0x02;				// INTEGER				-> messageID
			buffer[3] =		(char)0x01;				// length:	01
			buffer[4] =		(char)r.message_id;		// value:	02			-> (example)
			buffer[5] =		(char)0x65;				// SearchResultDone		-> protocolOp
			buffer[6] =		(char)0x07;				// length:	07
			buffer[7] =		(char)0x0a;				// ENUMERATED			-> resultCode
			buffer[8] =		(char)0x01;				// length:	01
			buffer[9] =		(char)0x00;				// value:	00
			buffer[10] =	(char)0x04;				// OCTET STRING
			buffer[11] =	(char)0x00;				// length:	00
			buffer[12] =	(char)0x04;				// OCTET STRING
			buffer[13] =	(char)0x00;				// length:	00

			return 14 * sizeof(char);
			break;

		default:
			return 0;
	}

	return 0;
}

// d = decode; s = search; e = encode; w = write
dsew_ret dsew(unsigned char* msg, int msg_size) {
	if (msg[5] != 0x63) return wrong_msg_err;

	ldap_search_t r;
	memset(&r, 0, sizeof(r));
	r.message_id = msg[4];
	r.filter.no = false;


	bool f = false;
	int p = 0;		// postup
	int l = 0;		// length
	int k = 0;		// pohyb v poli

	for (int i = 7; i < 7 + msg[6]; i++) {
		if (f == false) {

			switch (msg[i]) {
				case 0x01:						// boolean;
					if (p == 5) {
						p++;

						i++;
						i++;
						if (msg[i] == 0)
							r.types_only = false;
						else
							r.types_only = true;

					} else return unexpect_err;
					break;

				case 0x02:						// integer
					if (p == 3 || p == 4) {			// sizeLimit | timeLimit
						p++;

						i++;							// posun na položku 'length'
						i++;							// posun na položku 'value'
						if (p == 3) {				// sizeLimit
							r.size_limit = msg[i];
						} else {	/* p == 4 */	// timeLimit
							r.time_limit = msg[i];
						}

					} else return unexpect_err;
					break;

				case 0x04:						// octet string
					if (p == 0) {					// baseObject
						p++;

						i++;							// posun na položku 'length'
						l = msg[i];						// uložení délky
						k = 0;							// vynulování průchodové proměnné
						i++;							// posun na první znak
						while (l > 0) {
							r.object_name[k] = msg[i];	// ulož znak
							l--;						// dekrementuj počet průchodů
							i++;						// posun na další znak
							k++;						// posun na další místo
						}
						i--;							// návrat o jedno

					} else return unexpect_err;
					break;

				case 0x0a:						// enumerated
					if (p == 1 || p == 2) {			// scope | derefAliases
						p++;

						i++;							// posun na položku 'length'
						i++;							// posun na položku 'value'
						if (p == 1) {				// scope
							r.scope = msg[i];			// ulož
						} else { /* p == 2 */		// derefAliases
							r.deref_aliases = msg[i];	// ulož
						}

					} else return unexpect_err;
					break;

				default:
					if (p == 6) {
						f = true;
						p = 0;
						i--;
					} else return unexpect_err;
					break;

			}	// switch end

		} else {

			switch (msg[i]) {
				case 0x04:						// octet string

					i++;							// posun na položku 'length'
					l = msg[i];						// uložení délky
					k = 0;							// vynulování průchodové proměnné
					i++;							// posun na první znak
					while (l > 0) {

						switch(r.filter.type) {
							case equal_f:
								if (p == 0) {
									r.filter.desc[k] = tolower(msg[i]);
								} else {
									r.filter.value[k] = msg[i];
								}
								break;

							case substring_f:
								r.filter.desc[k] = tolower(msg[i]);
								break;

							default:
								return unexpect_err;
						}

						l--;						// dekrementuj počet průchodů
						i++;						// posun na další znak
						k++;						// posun na další místo
					}
					p++;
					i--;							// návrat o jedno

					break;


				case 0xa0:						// and
					return unsupp_err;			// UNSUPPORTED

				case 0xa1:						// or
					return unsupp_err;			// UNSUPPORTED

				case 0xa2:						// not
					r.filter.no = not r.filter.no;
					i++;
					break;

				case 0xa3:						// equalityMatch
					r.filter.type = equal_f;
					i++;

					break;

				case 0xa4:						// substring
					r.filter.type = substring_f;
					i++;

					break;

				case 0xa5:						// greaterOrEqual
					return unsupp_err;			// UNSUPPORTED

				case 0xa6:						// lessOrEqual
					return unsupp_err;			// UNSUPPORTED

				case 0xa8:						// aproxxMatch
					return unsupp_err;			// UNSUPPORTED

				case 0x87:						// present
					r.filter.type = present_f;
					i++;

					i = msg_size;

					break;

				case 0xa9:						// extensibleMatch
					return unsupp_err;			// UNSUPPORTED


				case 0x30:
					if (r.filter.type == substring_f) {
						i++;	// length
						i++;	// value

						switch(msg[i]) {
							case 0x80:
								r.filter.type = substring_ini_f;
								break;

							case 0x81:
								r.filter.type = substring_any_f;
								break;

							case 0x82:
								r.filter.type = substring_fin_f;
								break;

							default:
								return unexpect_err;
								break;

						}

						i++;
						l = msg[i];
						k = 0;
						i++;

						while (l > 0) {
							r.filter.value[k] = msg[i];	// ulož znak

							l--;						// dekrementuj počet průchodů
							i++;						// posun na další znak
							k++;						// posun na další místo
						}
						i--;

						break;

					}

					if ((i + msg[i+1] + 1) == (msg_size -1)) {	// na koci zprávy je SEQUENCE, kterou můžeme ignorovat
						i = msg_size;
						break;
					}

					return unexpect_err;

				default:
					return unexpect_err;

			}	// switch end

		}	// else end
	}	// for end

	if (		// kontrola, zda-li je atribut vůbec poporován
		strcmp(r.filter.desc, "cn") != 0 &&
		strcmp(r.filter.desc, "commonname") != 0 &&
		strcmp(r.filter.desc, "uid") != 0 &&
		strcmp(r.filter.desc, "userid") != 0 &&
		strcmp(r.filter.desc, "mail") != 0
	) return unknown_attr_err;

	return search_db(r);

}

dsew_ret search_db(ldap_search_t r) {
	std::string helper;
	std::size_t found, last;
	ldap_record_t s;

	db.clear();
	db.seekg(0);

	int i = (r.size_limit == 0) ? SIZE_LIMIT : (r.size_limit > SIZE_LIMIT ? SIZE_LIMIT : r.size_limit);

	for(std::string line; std::getline(db, line); ) {
		if (i < 1) break;

		char* cline = const_cast<char*>(line.c_str());

		memset(&s, 0, sizeof(s));
		s.cn = strtok(cline, ";");
		s.uid = strtok(NULL, ";");
		s.mail = strtok(NULL, ";");

		switch(r.filter.type) {

			case present_f:
				build_response(s, r);
				i--;
				break;

			case equal_f:
				if (strcmp(r.filter.desc, "cn") == 0 || strcmp(r.filter.desc, "commonname") == 0) {						// cn, CommonName
					if (r.filter.no == false) {
						if (strcmp(r.filter.value, s.cn) == 0) {
							build_response(s, r);
							i--;
						}
					} else {
						if (strcmp(r.filter.value, s.cn) != 0) {
							build_response(s, r);
							i--;
						}
					}
				} else if (strcmp(r.filter.desc, "uid") == 0 || strcmp(r.filter.desc, "userid") == 0) {					// uid, UserID
					if (r.filter.no == false) {
						if (strcmp(r.filter.value, s.uid) == 0) {
							build_response(s, r);
							i--;
						}
					} else {
						if (strcmp(r.filter.value, s.uid) != 0) {
							build_response(s, r);
							i--;
						}
					}
				} else if (strcmp(r.filter.desc, "mail") == 0) {														// mail
					if (r.filter.no == false) {
						if (strcmp(r.filter.value, s.mail) == 0) {
							build_response(s, r);
							i--;
						}
					} else {
						if (strcmp(r.filter.value, s.mail) != 0) {
							build_response(s, r);
							i--;
						}
					}
				} else return unknown_attr_err;
				break;

			case substring_ini_f:
				if (strcmp(r.filter.desc, "cn") == 0 || strcmp(r.filter.desc, "commonname") == 0) {						// cn, CommonName
					helper = s.cn;
					found = helper.find(r.filter.value);

					if (r.filter.no == false) {
						if (found != std::string::npos && found == 0) {
							build_response(s, r);
							i--;
						}
					} else {
						if (found == std::string::npos || found != 0) {
							build_response(s, r);
							i--;
						}
					}
				} else if (strcmp(r.filter.desc, "uid") == 0 || strcmp(r.filter.desc, "userid") == 0) {					// uid, UserID
					helper = s.uid;
					found = helper.find(r.filter.value);

					if (r.filter.no == false) {
						if (found != std::string::npos && found == 0) {
							build_response(s, r);
							i--;
						}
					} else {
						if (found == std::string::npos || found != 0) {
							build_response(s, r);
							i--;
						}
					}
				} else if (strcmp(r.filter.desc, "mail") == 0) {														// mail
					helper = s.mail;
					found = helper.find(r.filter.value);

					if (r.filter.no == false) {
						if (found != std::string::npos && found == 0) {
							build_response(s, r);
							i--;
						}
					} else {
						if (found == std::string::npos || found != 0) {
							build_response(s, r);
							i--;
						}
					}
				} else return unknown_attr_err;
				break;

			case substring_any_f:
				if (strcmp(r.filter.desc, "cn") == 0 || strcmp(r.filter.desc, "commonname") == 0) {						// cn, CommonName
					helper = s.cn;
					found = helper.find(r.filter.value);

					if (r.filter.no == false) {
						if (found != std::string::npos) {
							build_response(s, r);
							i--;
						}
					} else {
						if (found == std::string::npos) {
							build_response(s, r);
							i--;
						}
					}
				} else if (strcmp(r.filter.desc, "uid") == 0 || strcmp(r.filter.desc, "userid") == 0) {					// uid, UserID
					helper = s.uid;
					found = helper.find(r.filter.value);

					if (r.filter.no == false) {
						if (found != std::string::npos) {
							build_response(s, r);
							i--;
						}
					} else {
						if (found == std::string::npos) {
							build_response(s, r);
							i--;
						}
					}
				} else if (strcmp(r.filter.desc, "mail") == 0) {														// mail
					helper = s.mail;
					found = helper.find(r.filter.value);

					if (r.filter.no == false) {
						if (found != std::string::npos) {
							build_response(s, r);
							i--;
						}
					} else {
						if (found == std::string::npos) {
							build_response(s, r);
							i--;
						}
					}
				} else return unknown_attr_err;
				break;

			case substring_fin_f:
				if (strcmp(r.filter.desc, "cn") == 0 || strcmp(r.filter.desc, "commonname") == 0) {						// cn, CommonName
					helper = s.cn;
					found = helper.find(r.filter.value);
					last = found;
					while (found != std::string::npos) {
						found = helper.find(r.filter.value, found + 1);
						if (found != std::string::npos) {
							last = found;
						}
					}

					if (r.filter.no == false) {
						if (last != std::string::npos && (last + std::strlen(r.filter.value) == std::strlen(s.cn))) {
							build_response(s, r);
							i--;
						}
					} else {
						if (last == std::string::npos || (last + std::strlen(r.filter.value) != std::strlen(s.cn))) {
							build_response(s, r);
							i--;
						}
					}
				} else if (strcmp(r.filter.desc, "uid") == 0 || strcmp(r.filter.desc, "userid") == 0) {					// uid, UserID
					helper = s.uid;
					found = helper.find(r.filter.value);
					last = found;
					while (found != std::string::npos) {
						found = helper.find(r.filter.value, found + 1);
						if (found != std::string::npos) {
							last = found;
						}
					}

					if (r.filter.no == false) {
						if (last != std::string::npos && (last + std::strlen(r.filter.value) == std::strlen(s.uid))) {
							build_response(s, r);
							i--;
						}
					} else {
						if (last == std::string::npos || (last + std::strlen(r.filter.value) != std::strlen(s.uid))) {
							build_response(s, r);
							i--;
						}
					}
				} else if (strcmp(r.filter.desc, "mail") == 0) {														// mail
					helper = s.mail;
					found = helper.find(r.filter.value);
					last = found;
					while (found != std::string::npos) {
						found = helper.find(r.filter.value, found + 1);
						if (found != std::string::npos) {
							last = found;
						}
					}

					if (r.filter.no == false) {
						if (last != std::string::npos && (last + std::strlen(r.filter.value) == std::strlen(s.mail))) {
							build_response(s, r);
							i--;
						}
					} else {
						if (last == std::string::npos || (last + std::strlen(r.filter.value) != std::strlen(s.mail))) {
							build_response(s, r);
							i--;
						}
					}
				} else return unknown_attr_err;
				break;


			default:
				return unexpect_err;
		}

	}

	return (i == 0) ? succ_size_exc : succ;

}

// sestav celou odpověď SearchResultEntry
void build_response(ldap_record_t s, ldap_search_t r) {
	unsigned char response[BUFFER_SIZE];
	memset(&response, 0, sizeof(response));

	response[0] = (char)0x30;

	response[2] = (char)0x02;
	response[3] = (char)0x01;
	response[4] = (char)r.message_id;
	response[5] = (char)0x64;

	response[7] = (char)0x04;				// objectName
	response[8] = (char)0x06;
	response[9] = 'p';
	response[10] =	'e';
	response[11] =	'r';
	response[12] =	's';
	response[13] =	'o';
	response[14] =	'n';

	int l = build_attributes(response, 15, s.cn, s.mail);
	response[6] = (char)(l + 8);
	response[1] = (char)(l + 13);

	/*
	std::cout << EMPH("SEARCH RESULT: ") << TAB_JUMP;
	for (int k = 0; k < (l + 15); k++) {
		std::cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)response[k] << " ";
	}
	std::cout << std::dec << END_LINE;
	std::cout.copyfmt(def_fmt);
	*/

	l = write(client_socket, response, l+15);

}

// sestav atributy pro SearchResultEntry
int build_attributes(unsigned char* r, int i, char* cn, char* mail) {
	int l = 0;
	int l0 = 0;
	int l1 = 0;
	int i0 = 0;

	r[i] = (char)0x30;
	l++;

	i0 = ++i;
	l++;

	l0 = build_attribute(r, i+1, (char*)"cn", cn);

	l += l0;
	i += l0;

	l1 = build_attribute(r, i+1, (char*)"mail", mail);

	l += l1;
	i += l1;

	r[i0] = (char)(l0 + l1);

	return l;
}

// vytvoř jeden atribut pro SearchResultEntry
int build_attribute(unsigned char* r, int i, char* desc, char* value) {
	int l = 0;

	r[i] = (char)0x30;
	l++;
	r[++i] = (char)(2 + std::strlen(desc) + 4 + std::strlen(value));
	l++;

	r[++i] = (char)0x04;
	l++;
	r[++i] = (char)std::strlen(desc);
	l++;
	for (unsigned int j = 0; j < std::strlen(desc); j++) {
		r[++i] = desc[j];
		l++;
	}

	r[++i] = (char)0x31;
	l++;
	r[++i] = (char)(std::strlen(value) + 2);
	l++;
	r[++i] = (char)0x04;
	l++;
	r[++i] = (char)(std::strlen(value));
	l++;
	for (unsigned int j = 0; j < std::strlen(value); j++) {
		r[++i] = value[j];
		l++;
	}

	return l;
}
