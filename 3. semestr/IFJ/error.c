// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Jakub Frýz (xfryzj01)

#include "error.h"

void print_error (Terror err) /* fce na výpis chybové hlášky */ {
	switch (err) {
		case err_lex:
			fprintf(stderr, "ERROR: Failure of lexical analysis!\n");
			exit(1);
		case err_syn:
			fprintf(stderr, "ERROR: Failure of syntactic analysis!\n");
			exit(2);
		case err_sem_und:
			fprintf(stderr, "ERROR: Failure of semantic analysis - undefined/redefinition class/fce/variable!\n");
			exit(3);
		case err_sem_kom:
			fprintf(stderr, "ERROR: Failure of semantic analysis! - error of type compatibility\n");
			exit(4);
		case err_sem_oth:
			fprintf(stderr, "ERROR: Failure of semantic analysis!\n");
			exit(6);
		case err_run_load:
			fprintf(stderr, "ERROR: Runtime error loading the numerical value from the input!\n");
			exit(7);
		case err_run_uni:
			fprintf(stderr, "ERROR: Runtime error - working with uninitialized variables!\n");
			exit(8);
		case err_run_div:
			fprintf(stderr, "ERROR: Runtime error - division by zero!\n");
			exit(9);
		case err_run_oth:
			fprintf(stderr, "ERROR: Runtime error!\n");
			exit(10);
		case err_oth:
			fprintf(stderr, "ERROR: Internal error of interpreter!\n");
			exit(99);
	}
}
