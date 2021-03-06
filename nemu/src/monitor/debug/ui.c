#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
uint32_t expr(char* e,bool* success);
void insert_wp(char *args);
void print_all_wp();
void delete_wp(int no);
/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);
static int cmd_p(char *args);
static int cmd_w(char *args);
static int cmd_d(char *args);
//单步执行
static int cmd_si(char *args) {
  if(args==NULL){
    cpu_exec(1);
    return 0;
  }
  int n=atoi(args);
  if(n==0)cpu_exec(1);
	else if(n<-1){
	  printf("Unknown number '%d'\n",n);
	}else cpu_exec(n);
  return 0;
}

//info打印寄存器状态
static void print_registers(){
  printf("eax:   0x%08x   %d\n", cpu.eax,cpu.eax);
  printf("edx:   0x%08x   %d\n",cpu.edx,cpu.edx);
  printf("ecx:   0x%08x   %d\n",cpu.ecx,cpu.ecx);
	printf("ebx:   0x%08x   %d\n",cpu.ebx,cpu.ebx);
  printf("ebp:   0x%08x   %d\n",cpu.ebp,cpu.ebp);
	printf("esi:   0x%08x   %d\n",cpu.esi,cpu.esi);
	printf("edi:   0x%08x   %d\n",cpu.edi,cpu.edi);
	printf("esp:   0x%08x   %d\n",cpu.esp,cpu.esp);
	printf("eip:   0x%08x   %d\n", cpu.eip,cpu.eip);
}
static int cmd_info(char *args){
  switch(*args){
    case 'r':print_registers(); return 0;
		case 'w':print_all_wp();return 0;
    default:return 1;
  }
}

//扫描内存
static int cmd_x(char *args){
  int address,length;
	char *numStr=strtok(NULL," ");
	char *exp=strtok(NULL," ");
  if(numStr==NULL||exp==NULL){
    printf("You input an invalid expression\n");
    return 0;
  }
	bool s=true;
	address=expr(exp,&s);
	if(!s){
	  printf("Unknown expression '%s'\n",exp);
		return 0;
	}
  length=atoi(numStr);
  int i;
	printf("Address       Big-Endian    Little-Endian\n");
  for(i=1;i<=length;i++){
    int content=vaddr_read(address,4);
    printf("0x%08x    0x%08x    ",address,content);
		int j;
		for(j=0;j<4;j++)
		    printf("%02x ",vaddr_read(address+j,1));
		printf("\n");
    address+=4;
  }
  return 0;
}
static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },

  /* TODO: Add more commands */
  {"si","Single Step",cmd_si},
  {"info","Print message of registers",cmd_info},
  {"x","dump memory:x length address",cmd_x},
	{"p","Calculate the expression test",cmd_p},
	{"w","Add an Watchpoint",cmd_w},
	{"d","Delete the Watchpoint",cmd_d},
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}
//test expression
static int cmd_p(char *args){
	bool flag=true;
	uint32_t num=expr(args,&flag);
	if(!flag){
	  printf("Unknownknown expression '%s'!\n",args);
		return 0;
	}
	printf("%d\n",num);
  return 0;
}
//Add an Watchpoint
static int cmd_w(char *args){
  insert_wp(args);
	return 0;
}
//Delete the Watchpoint
static int cmd_d(char *args){
	if(args==NULL){
	  printf("You have input error order to delete watchpoint\n");
		return 0;
	}
	int n=atoi(args);
	delete_wp(n);
  return 0;
}
void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  while (1) {
    char *str = rl_gets();
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
