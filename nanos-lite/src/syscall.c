#include "common.h"
#include "syscall.h"
int sys_none();
void sys_exit(int code);
int sys_write(int fd,void *buf,int len);
int sys_brk();
_RegSet* do_syscall(_RegSet *r) {
  uintptr_t a[4];
  a[0] = SYSCALL_ARG1(r);
	a[1] = SYSCALL_ARG2(r);
	a[2] = SYSCALL_ARG3(r);
	a[3] = SYSCALL_ARG4(r);

  switch (a[0]) {
		case SYS_none:SYSCALL_ARG1(r)=sys_none();break;
		case SYS_exit:sys_exit(0);break;
		case SYS_write:SYSCALL_ARG1(r)=sys_write(a[1],(void*)a[2],a[3]);break;
		case SYS_brk:SYSCALL_ARG1(r)=sys_brk();break;
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
int sys_brk(){
  return 0;
}
