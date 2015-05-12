#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <regex.h>
#include <stdlib.h>
#include <ctype.h>

#include "token.h"

extern struct SymTable SymTable;
int main() {
  char *fn="1";
  struct Token *token, *tp;

   token = lex(fn);

   for (tp = token; tp->next != NULL; tp = tp->next)
     printf("idtf: %d   \t string: %s\t show : %s\n", \
   	    tp->idtf, tp->string, tp->show);

  grammer(token);
}
