#include "common.h"
#include "syscall.h"
int sys_none();
void sys_exit(int code);
int sys_write(int fd,void *buf,int len);
_RegSet* do_syscall(_RegSet *r) {
  uintptr_t a[4];
  a[0] = SYSCALL_ARG1(r);

  switch (a[0]) {
		case SYS_none:printf("none");SYSCALL_ARG1(r)=sys_none();break;
		case SYS_exit:printf("exit");sys_exit(0);break;
		case SYS_write:printf("write");break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}

int sys_none(){
	printf("sys_none\n");
  return 1;
}
void sys_exit(int code){
	printf("sys_exit\n");
  _halt(code);
}
int sys_write(int fd,void *buf,int len){
	printf("sys_write\n");
  if(fd==1||fd==2){
	  int i=0;
		for(i=0;i<len;i++){
		  _putc(((char*)buf)[i]);
		}
	}
	return len;
}
