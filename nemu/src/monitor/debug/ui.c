#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
uint32_t expr(char* e,bool* success);
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
static int cmd_expr(char *args);
//单步执行
static int cmd_si(char *args) {
  //printf("args:%s\n",args);
  if(args==NULL){
    cpu_exec(1);
    return 0;
  }
  int n=0,i;
  for(i=0;i<strlen(args);i++){
    n=10*n+args[i]-'0';
  }
  //printf("n:%d\n",n);
  cpu_exec(n);
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
    default:return 1;
  }
}

//扫描内存
static int cmd_x(char *args){
  int address,length;
  bool flag=true;
  if(!sscanf(args,"%d 0x%x",&length,&address)){
    flag=false;
  }
  if(!flag){
    printf("You input an invalid expression\n");
    return 0;
  }
  //printf("length:%d address:0x%x\n",length,address);
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
	{"expr","Calculate the expression test",cmd_expr},

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
static int cmd_expr(char *args){
	bool flag=true;
	uint32_t num=expr(args,&flag);
	if(!flag){
	  printf("Unknownknown expression '%s'!\n",args);
		return 0;
	}
	printf("%d\n",num);
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
