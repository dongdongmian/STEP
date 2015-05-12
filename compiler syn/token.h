enum Token_key{INT = 1,FLOAT,STRING,//3
	       INT_V,FLOAT_V,STRING_V,//3
	       IF,ELSE,WHILE,PRINT,//4
	       ADD,SUB,MUL,DIV,EQUAL,L_BRAK,R_BRAK,//7
	       GREAT,LESS,G_EQUAL,L_EQUAL,E_EQUAL,N_EQUAL,AND,OR,//8
	       EXPLA,BL_BRAK,BR_BRAK,SIMI,COMMA,//5 END is ;
	       ID,END} token_key;//1

struct SymTable{
  char *key;
  int symbol;
}; 

struct Token{
  int idtf;
  char *string;
  char *show;
  struct Token *next;
};

struct Token *lex(char *filename);

void grammer(struct Token * token);
