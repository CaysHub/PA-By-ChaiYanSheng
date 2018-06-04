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
}
size_t fs_filesz(int fd);
int fs_open(const char *pathname, int flags, int mode){
  Log("fs_open");
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
ssize_t fs_read(int fd, void *buf, size_t len){
	if(fd<3){
	  panic("fs_resd fd<3");
	}
	if(file_table[fd].open_offset+len>file_table[fd].size){
	  len=file_table[fd].size-file_table[fd].open_offset;
	}
	ramdisk_read(buf,file_table[fd].disk_offset+file_table[fd].open_offset,len);
	file_table[fd].open_offset+=len;
  return len;
}
ssize_t fs_write(int fd, const void *buf, size_t len){
  return len;
}
int fs_close(int fd){
  return 0;
}
size_t fs_filesz(int fd){
  return file_table[fd].size;
}
