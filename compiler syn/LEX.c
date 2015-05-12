#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <regex.h>
#include <stdlib.h>
#include <ctype.h>

#include "token.h"

#define MAXLEN 4096

#define SymTable_length 27
#define Pattern_length 4

char buf[MAXLEN*2];

enum boolean{FALSE,TRUE};

struct SymTable SymTable[] = {
  {"int",INT} , {"float",FLOAT} , {"string",STRING} ,
  {"if",IF} , {"else",ELSE} , {"while",WHILE} ,{"print",PRINT},
  {"+",ADD} , {"-",SUB} , {"*",MUL} , {"/",DIV} , {"=",EQUAL},  {"(",L_BRAK} , {")",R_BRAK} ,
  {">",GREAT} , {"<",LESS} , {">=",G_EQUAL} , {"<=",L_EQUAL} , {"==",E_EQUAL} , {"!=",N_EQUAL} , {"&&",AND} , {"||",OR},
  {"//",EXPLA} , {"{",BL_BRAK} , {"}",BR_BRAK} , {";",SIMI} , {",",COMMA}
};

struct {
  char *pattern;
  int symbol;
}  Pattern[Pattern_length] = {
  {"0|[1-9][0-9]*",INT_V} , {"(0|[1-9][0-9]*)\\.[0-9]+",FLOAT_V} , {"\".*\"",STRING_V} , {"[A-Za-z]+[0-9]?",ID}
};

char *show[] = {"INT","FLOAT","STRING",\
		"INT_V","FLOAT_V","STRING_V",\
		"IF","ELSE","WHILE","PRINT",\
		"+","-","*","/","=","(",")",\
		">","<",">=","<=","==","!=","&&","||",\
		"//","{","}",";",",",\
		"ID","END"};

char sign[] = {'+','-','*','/','=','(',')','{','}',',',';','>','<','&','|','/','[',']','!'};

//正则表达式
char errbuf[1024];
int err;    //err返回错误信息
regex_t reg[Pattern_length];    //有五个正则表达式需要匹配
regmatch_t pmatch[1];

void Free(){
  int i = 0;
  for(i = 0;i<Pattern_length;i++)
    regfree(&reg[i]);
}

void save_token(char *temp, struct Token **tpp, int idtf){
  (*tpp)->string = (char *)malloc(sizeof(char)*strlen(temp));
  (*tpp)->show = (char *)malloc(sizeof(char) * 10);
  strcpy((*tpp)->string, temp);
  strcpy((*tpp)->show,show[idtf-1]); 
  (*tpp)->idtf = idtf;

  struct Token *t;
  t = (struct Token *)malloc(sizeof(struct Token));
  (*tpp)->next = t;
  *tpp = t;
  (*tpp)->next = NULL;
}

int isKey(char *temp){
  int i = 0;
  for(i = 0;i<SymTable_length;i++){
    if(!strcmp(temp,SymTable[i].key))  
      return SymTable[i].symbol;
  }
  return -1;
}

int isNumOrId(char *temp){
  int i = 0;
  for(i = 0;i<Pattern_length;i++) {
    err = regexec(&reg[i],temp,1,pmatch,0);
    if(!err && ((pmatch[0].rm_eo - pmatch[0].rm_so) == strlen(temp)) )
      return Pattern[i].symbol;
  }
  printf("FALSE\n");
  return FALSE;
}

int isSign(char c){
  int i = 0;
  for(i = 0;i<strlen(sign);i++){
    if(c == sign[i])
      return TRUE;  
  }
  return FALSE;
}



struct Token *lex(char *filename){
  int i = 0;
  char temp[50];

  //建立正则表达式
  for(i = 0;i<Pattern_length;i++){
    if(regcomp(&reg[i],Pattern[i].pattern,REG_EXTENDED) < 0){
      regerror(err,&reg[i],errbuf,sizeof(errbuf));
      printf("err:%s\n",errbuf);
    }
  }

  FILE *file;
  /* char *filename = "1"; */
  file = fopen(filename,"r");
  if(file == NULL){
    printf("文件打开失败！！！结束。。。\n");
    exit(1);
  }

  struct Token *token,*origin;
  token = (struct Token *)malloc(sizeof(struct Token));
  origin = token;

  int pre = 0, state = 0, return_num = -1;

  //i代表当前正要处理的字符，i用于自增，cur用于字符的处理
  i = 0;
  while(1){
    if (0 == i || MAXLEN == i) {
      int n;
      if ((n = fread(buf+i, sizeof(char), MAXLEN, file)) < MAXLEN)
	buf[i+n] = EOF;
      printf("Read %d bytes to buffer\n", n);
    }
    /* Meet EOF? */
    if (EOF == buf[i] && 0 == state)
      break;

    /* printf("state: %d, buf[%d]: %c\n", state, i, buf[i]); */

    switch (state) {
    case 0:
      if (isspace(buf[i]))
	state = 0;
      else if (buf[i] == '"') {
	pre = i;
	state = 1;
      }
      else if (isSign(buf[i])) {
	pre = i;
	state = 2;
      }
      else {
	pre = i;
	state = 3;
      }
      break;
      
    case 1:
      if (buf[i] == '"') {
	i += 1;
	state = 4;
      }
      break;
	
    case 2:
      if (!isSign(buf[i]))
	state = 4;
      break;
	
    case 3:
      if (isspace(buf[i]) || isSign(buf[i]))
	state = 4;
      break;

    case 4:
      strncpy(temp, buf+pre, i-pre + 1);
      temp[i-pre + 1] = '\0';
      /* printf("temp: %s\n",temp); */

      if ((return_num = isKey(temp)) > 0 ||
	  (return_num = isNumOrId(temp)) > 0)
	save_token(temp, &token, return_num);
      else
	printf("Unknown token: %s\n", temp);

      state = 0;
      break;
      
    default:
      break;
    }

    if (state != 4)
      i = (i+1) % MAXLEN;
    else
      i -= 1;
  }
  
  temp[0] = '\0';
  save_token(temp,&token,END);
  
  Free();
  return origin;
}
