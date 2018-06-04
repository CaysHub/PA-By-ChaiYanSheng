#include "common.h"

#define DEFAULT_ENTRY ((void *)0x4000000)
void ramdisk_read(void *buf, off_t offset, size_t len);
void ramdisk_write(const void *buf, off_t offset, size_t len);
size_t get_ramdisk_size();
int fs_open(const char *pathname, int flags, int mode);
ssize_t fs_read(int fd, void *buf, size_t len);
ssize_t fs_write(int fd, const void *buf, size_t len);
off_t fs_lseek(int fd, off_t offset, int whence);
int fs_close(int fd);
size_t fs_filesz(int fd);
uintptr_t loader(_Protect *as, const char *filename) {
  //TODO();
	//size_t len=get_ramdisk_size();
	//ramdisk_read(DEFAULT_ENTRY,0,len);
	int fd=fs_open(filename,0,0);
	fs_read(fd,DEFAULT_ENTRY,fs_filesz(fd));
	assert(fs_close(fd)==0);
	return (uintptr_t)DEFAULT_ENTRY;
}
