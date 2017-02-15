// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Jakub Frýz (xfryzj01),
//				Filip Dostálík (xdosta46),
//				Michal Hrabovský (xhrabo08)

#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enumlist.h"
#include "error.h"
#include "queue.h"
#include "ial.h"
#include "parser.h"

Tqueue scanner(char* in);

#endif //SCANNER_H_INCLUDED
