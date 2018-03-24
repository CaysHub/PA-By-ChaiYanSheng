#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

enum {
  TK_NOTYPE = 256, TK_EQ,TK_NUM,TK_ADD,TK_SUB,
  TK_MUL,TK_DIV,TK_L_BRACKET,TK_R_BRACKET,TK_REG,
	TK_HEXNUM,TK_AND,TK_OR,TK_NOT,TK_OBJECT,TK_NEQ,
	TK_POINTER,TK_MINUS

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},       // spaces
	{"0[xX][0-9a-fA-F]+",TK_HEXNUM},// HEXNUM
  {"[0-9]+",TK_NUM},       // number
	{"!=",TK_NEQ},           // !=
  {"\\+", TK_ADD},         // plus
  {"-",TK_SUB},            // subtract
  {"\\*",TK_MUL},          // multiply
  {"/",TK_DIV},            // divide
  {"\\(",TK_L_BRACKET},    // left bracket
  {"\\)",TK_R_BRACKET},    // rigth bracket
  {"\\$e[abcd]x",TK_REG},  // register
  {"\\$e[bs]p",TK_REG},    // register
  {"\\$e[sd]i",TK_REG},    // register
  {"\\$eip",TK_REG},       // register
  {"&&",TK_AND},           // &&
  {"\\|\\|",TK_OR},        // ||
  {"!",TK_NOT},            // !
  {"==", TK_EQ}            // equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;
  nr_token = 0;
  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

	      if(rules[i].token_type==TK_NOTYPE);
	      else{
            tokens[nr_token].type=rules[i].token_type;
	          if(tokens[nr_token].type==TK_NUM||tokens[nr_token].type==TK_HEXNUM||
				        tokens[nr_token].type==TK_REG){
	              int j;
	              for(j=1;j<substr_len;j++){
	                  tokens[nr_token].str[j]=substr_start[j];
	              }
	              tokens[nr_token].str[j]='\0';
	          }
	          nr_token++;
	      }
        /*switch (rules[i].token_type) {
          default: TODO();
        }*/
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}
int eval(int p,int q);
bool check_parentheses(int p,int q);
int find_dominant(int p,int q);
bool isoperator(int index);
bool isinbracket(int index,int p,int q);
int priority(int i);
int getnum(char ch){
  if(ch>='0'&&ch<='9')return ch-'0';
	else if(ch>='a'&&ch<='z')return ch-'a'+10;
	else if(ch>='A'&&ch<='Z')return ch-'A'+10;
	return 0;
}
uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  //TODO();
	int i;
	for(i=0;i<nr_token;i++){
	  if(tokens[i].type==TK_MUL&&(i==0||isoperator(i-1)||
				tokens[i-1].type==TK_L_BRACKET)){
		  tokens[i].type=TK_POINTER;
		}else if(tokens[i].type==TK_SUB&&(i==0||isoperator(i-1)||
					   tokens[i-1].type==TK_L_BRACKET)){
		  tokens[i].type=TK_MINUS;
		}
	}
	if(*success==true){
	  int num=eval(0,nr_token-1);
		if(num==-2147483648){
		  printf("Somthing wrong with the expressions!\n");
			return 0;
		}else{
		  return num;
		}
	}
  return 0;
}
bool isoperator(int index){
  if(tokens[index].type==TK_ADD||tokens[index].type==TK_SUB||
      tokens[index].type==TK_MUL||tokens[index].type==TK_DIV||
			tokens[index].type==TK_MINUS||tokens[index].type==TK_POINTER||
			tokens[index].type==TK_EQ||tokens[index].type==TK_NEQ||
			tokens[index].type==TK_AND||tokens[index].type==TK_OR||
			tokens[index].type==TK_NOT){
    return true;
  }else{
    return false;
  }
}
bool isinbracket(int index,int p,int q){
	int left,right_bracket_num=0;
  for(left=index;left>=p;left--){
		if(tokens[left].type==TK_R_BRACKET)right_bracket_num++;
		else if(tokens[left].type==TK_L_BRACKET){
		  if(right_bracket_num!=0)right_bracket_num--;
			else{
				int right,left_bracket_num=0;
				for(right=index;right<=q;right++){
				  if(tokens[right].type==TK_L_BRACKET)left_bracket_num++;
					else if(tokens[right].type==TK_R_BRACKET){
					  if(right_bracket_num!=0)right_bracket_num--;
						else{
						  return true;
						}
					}
				}
				return false;
			}
		}
	}
  return false;
}
int priority(int index){
  if(tokens[index].type==TK_MUL||tokens[index].type==TK_DIV)return 2;
	else if(tokens[index].type==TK_ADD||tokens[index].type==TK_SUB)return 1;
	else if(tokens[index].type==TK_MINUS||tokens[index].type==TK_POINTER)return 3;
	else return 3;
}
int find_dominant(int p,int q){
  int i,operator_num=0;
	int *operator=(int*)malloc((q-p+1)*sizeof(int));
	for(i=p;i<=q;i++){
	  if(isoperator(i)&&!isinbracket(i,p,q)){
		  operator[operator_num]=i;
			operator_num++;
		}
	}
	int min=0;
	for(i=0;i<operator_num;i++){
	  if(priority(operator[i])<priority(operator[min]))min=i;
	}
	if(priority(operator[min])==3){
	  for(i=0;i<operator_num;i++){
		  if(priority(operator[i])==priority(operator[min])){
			  int result=operator[i];
				free(operator);
				return result;
			}
		}
	}
	for(i=operator_num-1;i>=0;i--){
	  if(priority(operator[i])==priority(operator[min])){
		  int result=operator[i];
			free(operator);
			return result;
		}
	}
	int result=operator[min];
	free(operator);
	return result;
}
bool check_parentheses(int p,int q){
  if(tokens[p].type==TK_L_BRACKET&&tokens[q].type==TK_R_BRACKET){
			int *op=(int*)malloc(32*sizeof(int));
			int i,count=-1;
			for(i=0;i<32;i++)op[i]=-1;
			for(i=p+1;i<=q-1;i++){
			    if(tokens[i].type==TK_L_BRACKET){
					    count++;
						  op[count]=i;
					}else if(tokens[i].type==TK_R_BRACKET){
							if(count!=-1)count--;
							else{
							    free(op);
									return false;
							}
					}
			}
			free(op);
			if(count==-1)return true;
			return true;
	}
	return false;
}
int eval(int p,int q){
    if(p>q){
				printf("Error!\n");
				return -2147483648;
		}else if(p==q){
				if(tokens[p].type==TK_NUM){
				    int num=atoi(tokens[p].str);
						return num;
				}else if(tokens[p].type==TK_REG){
						int i;
						for(i=0;i<8;i++){
						    if(strcmp(tokens[p].str,regsl[i])==0){
								    return reg_l(i);
								}
						}
				}else if(tokens[p].type==TK_HEXNUM){
				  int num;
					sscanf(tokens[p].str,"0x%x",&num);
					return num;
				}else{
				    //do nothing;
				};
		}else if(check_parentheses(p,q)){
				return eval(p+1,q-1);
		}else{
				int dominant=find_dominant(p,q);
				if(tokens[dominant].type==TK_POINTER){
				  return vaddr_read(eval(dominant+1,q),4);
				}else if(tokens[dominant].type==TK_MINUS){
				  return -eval(dominant+1,q);
				}else if(tokens[dominant].type==TK_NOT){
				  return !eval(dominant+1,q);
				}else{
					int val1=eval(p,dominant-1);
					int val2=eval(dominant+1,q);
					switch(tokens[dominant].type){
				   	  case TK_ADD:return val1+val2;break;
							case TK_SUB:return val1-val2;break;
							case TK_MUL:return val1*val2;break;
							case TK_DIV:return val1/val2;break;
							case TK_AND:return val1&&val2;break;
							case TK_OR:return val1||val2;break;
							case TK_EQ:return val1==val2;break;
							case TK_NEQ:return val1!=val2;break;
							default:return -2147483648;
					}
				}
		}
		return 0;
}











