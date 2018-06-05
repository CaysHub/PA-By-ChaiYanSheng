#include "fs.h"

typedef struct {
  char *name;
  size_t size;
  off_t disk_offset;
	off_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_EVENTS, FD_DISPINFO, FD_NORMAL};

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin (note that this is not the actual stdin)", 0, 0},
  {"stdout (note that this is not the actual stdout)", 0, 0},
  {"stderr (note that this is not the actual stderr)", 0, 0},
  [FD_FB] = {"/dev/fb", 0, 0},
  [FD_EVENTS] = {"/dev/events", 0, 0},
  [FD_DISPINFO] = {"/proc/dispinfo", 128, 0},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
	file_table[FD_FB].size=_screen.width*_screen.height*4;
}
size_t fs_filesz(int fd);
int fs_open(const char *pathname, int flags, int mode){
  //Log("fs_open filename: %s",pathname);
	int i=0;;
	for(i=0;i<NR_FILES;i++){
	  if(strcmp(file_table[i].name,pathname)==0){
		  file_table[i].open_offset=0;
			return i;
		}
	}
	assert(0);
	return 0;
}
void ramdisk_read(void *buf, off_t offset, size_t len);
void ramdisk_write(const void *buf, off_t offset, size_t len);
void dispinfo_read(void *buf, off_t offset, size_t len);
void fb_write(const void *buf, off_t offset, size_t len);
size_t events_read(void *buf, size_t len);
ssize_t fs_read(int fd, void *buf, size_t len){
	if(fd<3){
	  panic("fs_resd fd<3");
	}
	if(fd==FD_EVENTS){
	  size_t len2=events_read(buf,len);
		return len2>len?len:len2;
	}
	if(file_table[fd].open_offset+len>file_table[fd].size){
	  len=file_table[fd].size-file_table[fd].open_offset;
	}
	Log("fs_read fd: %d,len: %d",fd,len);
	if(fd==FD_DISPINFO){
		dispinfo_read(buf,file_table[fd].disk_offset+file_table[fd].open_offset,len);
	}else{
	  ramdisk_read(buf,file_table[fd].disk_offset+file_table[fd].open_offset,len);
	}
	file_table[fd].open_offset+=len;
  return len;
}
ssize_t fs_write(int fd, const void *buf, size_t len){
	if(fd==1||fd==2){
	  int i=0;
		for(i=0;i<len;i++){
		  _putc(((char*)buf)[i]);
		}
		Log("fs_write fd: %d,len: %d",fd,len);
		return len;
	}
	if(len>file_table[fd].size-file_table[fd].open_offset){
		len=file_table[fd].size-file_table[fd].open_offset;
	}
	if(fd==FD_FB){
	  fb_write(buf,file_table[fd].disk_offset+file_table[fd].open_offset,len);  
	}else{
	  ramdisk_write(buf,file_table[fd].disk_offset+file_table[fd].open_offset,len);
	}
	file_table[fd].open_offset+=len;
	Log("fs_write fd: %d,len: %d",fd,len);
  return len;
}
off_t fs_lseek(int fd, off_t offset, int whence){
	//Log("fs_lseek fd: %d,offset: %d",fd,offset);
  switch(whence){
	  case SEEK_SET:file_table[fd].open_offset=offset;break;
		case SEEK_CUR:file_table[fd].open_offset+=offset;break;
		case SEEK_END:file_table[fd].open_offset=file_table[fd].size+offset;break;
		default:panic("fs_lseek Unknown whence");break;
	}
	if(file_table[fd].open_offset<0)
		file_table[fd].open_offset=0;
	if(file_table[fd].open_offset>file_table[fd].size)
		file_table[fd].open_offset=file_table[fd].size;
	return file_table[fd].open_offset;
}
int fs_close(int fd){
	//Log("fs_close fd: %d",fd);
	file_table[fd].open_offset=0;
  return 0;
}
size_t fs_filesz(int fd){
  return file_table[fd].size;
}
