#include "common.h"
#define PAGE_SIZE 4096
#define DEFAULT_ENTRY ((void *)0x8048000)
void ramdisk_read(void *buf, off_t offset, size_t len);
void ramdisk_write(const void *buf, off_t offset, size_t len);
size_t get_ramdisk_size();
int fs_open(const char *pathname, int flags, int mode);
ssize_t fs_read(int fd, void *buf, size_t len);
ssize_t fs_write(int fd, const void *buf, size_t len);
off_t fs_lseek(int fd, off_t offset, int whence);
int fs_close(int fd);
size_t fs_filesz(int fd);
void *new_page();

uintptr_t loader(_Protect *as, const char *filename) {
  //TODO();
	//size_t len=get_ramdisk_size();
	//ramdisk_read(DEFAULT_ENTRY,0,len);
	int fd=fs_open(filename,0,0);
	size_t fsz=fs_filesz(fd);
	void *pa,*va;
	for(va=DEFAULT_ENTRY;va<DEFAULT_ENTRY+fsz;va+=PAGE_SIZE){
		pa=new_page();
	  Log("Map va to pa: 0x%08x to 0x%08x", va, pa);
	  _map(as, va, pa);
		fs_read(fd,pa,(DEFAULT_ENTRY+fsz-va)<PAGE_SIZE?(DEFAULT_ENTRY+fsz-va):PAGE_SIZE);
	}
	fs_close(fd);
	//fs_read(fd,DEFAULT_ENTRY,fs_filesz(fd));
	//assert(fs_close(fd)==0);
	return (uintptr_t)DEFAULT_ENTRY;
}
