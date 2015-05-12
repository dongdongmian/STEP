#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "token.h"

#define BUF 1024

/* extern struct SymTable SymTable[]; */

typedef enum {ERROR, ACCEPT, SHIFT, REDUCE} action_t;
struct production {
  int lpart;
  int *rpart;
};
struct action {
  action_t type;
  union {
    int state;
    struct production production;
  } u;
};

static char *symbols[] = {
  "" ,"int", "float", "string","int_v","float_v","string_v",
  "if", "else" , "while" , "print",
  "+" , "-" , "*" , "/" , "=" , "(", ")" , 
  ">" , "less" , ">=" , "l_equal" , "==" , "!=" , "&&" , "or" ,
  "note" , "{" , "}" , "simi" , ",","id",////////////
  "$",
  "S'","Statement","Type","List_id","Type_v","Assignment",
  "ArithExpre","LogicExpre","Logic_judge","A","T","F",
  "IF","While","Print","String","B","Note","C"
};

/* /* 将符号从字符串转成数字 */
int sym2num(const char *str)
{
  int i;

  for (i = 0; i < sizeof(symbols)/sizeof(symbols[0]); i++)
    if (strcmp(str, symbols[i]) == 0)
      return i;

  return -1;
}

/* 将符号从数字转成字符串 */
const char *num2sym(int i)
{
  if (0 <= i < sizeof(symbols)/sizeof(symbols[0]))
    return symbols[i];
  else
    return NULL;
}

/* 根据字符串解析action, 字符串样例为: error/accept/shift:3/reduce:A:->:a:B */
void str2action(char *str, struct action *pa)
{

  char *type_s;
  // printf("%s\n", str);

  /* strtok的基本用法: http://stackoverflow.com/questions/3889992/how
     -does-strtok-split-the-string-into-tokens-in-c 第二个回答 */
  type_s = strtok(str, ":");
  if (strcmp("error", type_s) == 0)
    pa->type = ERROR;
  else if (strcmp("accept", type_s) == 0)
    pa->type = ACCEPT;
  else if (strcmp("shift", type_s) == 0) {
    pa->type = SHIFT;
    pa->u.state = atoi(strtok(NULL, ":"));
  }
  else if (strcmp("reduce", type_s) == 0) {
    pa->type = REDUCE;
    pa->u.production.lpart = sym2num(strtok(NULL, ":"));

    strtok(NULL, ":");		/* 跳过箭头'->' */
    char *rpart_s, *symbol;
    rpart_s = symbol = strtok(NULL, ":");
    /* 计算产生式右部符号个数 */
    int n = 0;
    for (; symbol != NULL; symbol = strtok(NULL, ":"))
      n++;

    pa->u.production.rpart = (int *)calloc(n+1, sizeof(int));
    int i;
    for (i = 0, symbol = rpart_s; i < n; i++, symbol += strlen(symbol)+1) {
      pa->u.production.rpart[i] = sym2num(symbol);
    }
    pa->u.production.rpart[i] = -1; /* -1标记右部符号串结束 */
  }
  else {
    fprintf(stderr, "Unknown action type: %s\n", type_s);
    exit(-1);
  }
}

void print_action(struct action *pa)
{
  if (pa->type == ERROR)
    printf("error\n");
  else if (pa->type == ACCEPT)
    printf("accept\n");
  else if (pa->type == SHIFT)
    printf("shift %d\n", pa->u.state);
  else if (pa->type == REDUCE) {
    printf("reduce %s ->", num2sym(pa->u.production.lpart));
    int *ptr;
    for (ptr = pa->u.production.rpart; *ptr != -1; ptr++)
      printf(" %s", num2sym(*ptr));
    printf("\n");
  }
  else
    printf("Unknown action type\n");
}

int N_state, N_term, N_nterm;
int *Term_list, *Nterm_list;
struct action **Action_table;
int **Goto_table;

void build_table(const char *fn)
{
  int i;
  FILE *fp = fopen(fn, "r");
  fscanf(fp, "n_state:%d n_term:%d n_nterm:%d",
	 &N_state, &N_term, &N_nterm);

  /* 建立ACTION表 */
  Term_list = (int *)calloc(N_term, sizeof(int));

  //处理终结符，并由此建立表格
  for (i = 0; i < N_term; i++) {
    char term_s[10];
    fscanf(fp, "%s", term_s);   //由非空白符组成的字符串，赋值给term_s
    Term_list[i] = sym2num(term_s);
  }
  Action_table = (struct action **)calloc(N_state, sizeof(struct action *));
  for (i = 0; i < N_state; i++)
    Action_table[i] = (struct action *)calloc(N_term, sizeof(struct action));
  for (i = 0; i < N_state; i++) {
    int j;
    char buf[128];
    for (j = 0; j < N_term; j++) {
      fscanf(fp, "%s", buf);
      str2action(buf, &Action_table[i][j]);
    }
  }

  /* 建立GOTO表 */
  Nterm_list = (int *)calloc(N_nterm, sizeof(int));
  for (i = 0; i < N_nterm; i++) {
    char nterm_s[10];
    fscanf(fp, "%s", nterm_s);
    Nterm_list[i] = sym2num(nterm_s);
  }
  Goto_table = (int **)calloc(N_state, sizeof(int));
  for (i = 0; i < N_state; i++)
    Goto_table[i] = (int *)calloc(N_nterm, sizeof(int));
  for (i = 0; i < N_state; i++) {
    int j;
    char buf[10];
    for (j = 0; j < N_nterm; j++) {
      fscanf(fp, "%s", buf);
      if (strcmp("error", buf) == 0)
	Goto_table[i][j] = -1;
      else
	Goto_table[i][j] = atoi(buf);
    }
  }
}


//---------------------------------------------
//---------------------------------------------
enum {WRONG,RIGHT};

//返回action表格中的指针，结构 cs current state    term：终结符
struct action *next_action(int cs, int term)
{
  int i;

  for (i = 0; i < N_term; i++)
    if (Term_list[i] == term)
      return &Action_table[cs][i];

  return NULL;
}

//返回goto表格中压入栈的状态
int next_state(int cs, int nterm)
{
  int i;

  for (i = 0; i < N_nterm; i++)
    if (Nterm_list[i] == nterm)
      return Goto_table[cs][i];

  return -1;
}

//返回当前要规约的右部的个数
int rpart_num(int *rpart){
    int i = 0;
    for (i = 0;rpart[i] != -1; i++)
        ;
    return i;
}

int is_grammer(struct Token * token){

    int input[BUF],state_stack[BUF],symbol_stack[BUF];
    int state_point,sym_point,input_point;
    int state_curv,sys_curv,input_curv;
    int i = 0;

    state_stack[0] = -1;    // -1 作为栈底符号
    state_stack[1] = 0;
    symbol_stack[0] = -1;

    state_point = 1;
    sym_point = 0;
    input_point = 0;

    while(token->next != NULL){
        state_curv = state_stack[state_point];      //栈中当前指向的状态
        input_curv = token->idtf;   //当前符号入栈

        struct action *pa = next_action(state_curv,input_curv);

        if(pa == NULL){
            printf("next_action something is wrong!\n");
            return WRONG;
        }
	
	printf("state stack: "); 
        for(i = 0;i<=state_point;i++){ 
	  printf("%d ", state_stack[i]); 
        } 
	printf("\n"); 

        printf("Symbol stack: "); 
	for(i = 0;i<=sym_point;i++){ 
	  printf("%s ", num2sym(symbol_stack[i])); 
        } 
        printf("\n");

	printf("Action: ");
	print_action(pa);
	printf("\n");
	
        if(pa->type == ERROR)
            return WRONG;
        else if(pa->type == SHIFT){
            state_point ++;
            sym_point ++;
            symbol_stack[sym_point] = input_curv;   //当前符号入栈
            state_stack[state_point] = pa->u.state;    //状态压栈

            token = token->next;     //当前符号向后判断一个 
        }
        else if(pa->type == REDUCE){
            int n = rpart_num(pa->u.production.rpart);
            /* printf("n = %d \n",n); */
            for(i = n;i>0;i--){
                /* printf("state : %d \t rpart : %d \t sym_point : %d \n",symbol_stack[sym_point-n+i],pa->u.production.rpart[i-1],sym_point); */
                if(symbol_stack[sym_point-n+i] != pa->u.production.rpart[i-1]){
                    printf("reduce state something is wrong!!!");
                    return WRONG;
                }
            }
            /* printf("judge is over!\n \n "); */

            sym_point = sym_point - n + 1;  //symbol栈弹栈,准备将goto表中的state状态加入
            state_point = state_point - n + 1;  //state弹栈
            state_stack[state_point] = next_state(state_stack[state_point-1],pa->u.production.lpart);    //goto表中的状态压入当前的栈
            symbol_stack[sym_point] = pa->u.production.lpart;
        }

        else if(pa->type == ACCEPT){
	  /* printf("amazing , it's accept!!!\n"); */
	  return RIGHT;
        }
    }
    return WRONG;
}



void grammer(struct Token * token)
{
  char *fn = "1.txt";
  build_table(fn);
  printf("\nTable was built\n\n");

  if(is_grammer(token)){
    printf("Syntax Ok!\n");
  }
  else
    printf("居然没有被识别。。。肯定是哪里写错了吧，少年！\n");
}
