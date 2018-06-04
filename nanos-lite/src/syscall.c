#include "common.h"
#include "syscall.h"
int sys_none();
void sys_exit(int code);
int sys_write(int fd,void *buf,int len);
int sys_brk(void *addr);
int sys_open(char *pathname,int flags,int mode);
ssize_t sys_read(int fd,void *buf,size_t len);
off_t sys_lseek(int fd,off_t offset,int whence);
int sys_close(int fd);
int fs_open(const char *pathname, int flags, int mode);
ssize_t fs_read(int fd, void *buf, size_t len);
ssize_t fs_write(int fd, const void *buf, size_t len);
off_t fs_lseek(int fd, off_t offset, int whence);
int fs_close(int fd);
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
		case SYS_brk:SYSCALL_ARG1(r)=sys_brk((void*)a[2]);break;
		case SYS_open:SYSCALL_ARG1(r)=sys_open((char*)a[1],a[2],a[3]);break;
		case SYS_read:SYSCALL_ARG1(r)=sys_read(a[1],(void*)a[2],a[3]);break;
		case SYS_lseek:SYSCALL_ARG1(r)=sys_lseek(a[1],a[2],a[3]);break;
		case SYS_close:SYSCALL_ARG1(r)=sys_close(a[1]);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}

int sys_none(){
#ifdef DEBUG
	Log("call sys_none");
#endif
  return 1;
}
void sys_exit(int code){
#ifdef DEBUG
	Log("call sys_exit");
#endif
  _halt(code);
}
int sys_write(int fd,void *buf,int len){
#ifdef DEBUG
	Log("call sys_write");
  /*if(fd==1||fd==2){
	  int i=0;
		for(i=0;i<len;i++){
		  _putc(((char*)buf)[i]);
		}
	}*/
#endif
	return fs_write(fd,buf,len);
}

int sys_brk(void *addr){
#ifdef DEBUG
	Log("call sys_brk");
#endif
  return 0;
}
int sys_open(char *pathname,int flags,int mode){
#ifdef DEBUG
	Log("call sys_open filename: %s",pathname);
#endif
  return fs_open(pathname,flags,mode);
}
ssize_t sys_read(int fd,void *buf,size_t len){
#ifdef DEBUG
	Log("call sys_read fd: %d",fd);
#endif
  return fs_read(fd,buf,len);
}
off_t sys_lseek(int fd,off_t offset,int whence){
#ifdef DEBUG
  Log("call sys_lseek fd: %d",fd);
#endif
	return fs_lseek(fd,offset,whence);
}
int sys_close(int fd){
#ifdef DEBUG
	Log("call sys_close fd: %d",fd);
#endif
  return fs_close(fd);
}

