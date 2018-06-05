#include "common.h"

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};
unsigned long _uptime();
size_t events_read(void *buf, size_t len) {
  int key=_read_key();
	if(key==0){
	  unsigned long time=_uptime(); 
	  return sprintf(buf,"t %lu",time)-1;
	}
	if((key&(0x8000))!=0){
	  return sprintf(buf,"kd %s",keyname[key^0x8000])-1;
	}else if((key&(0x8000))==0){
	  return sprintf(buf,"ku %s",keyname[key])-1;
	}
	return 0;
}

static char dispinfo[128] __attribute__((used));

void dispinfo_read(void *buf, off_t offset, size_t len) {
	memcpy(buf,dispinfo+offset,len);
}

void fb_write(const void *buf, off_t offset, size_t len) {
	int y=(offset/4)/_screen.width,x=offset/4-_screen.width*y;
	//Log("fb_write offset:%d\tlen:%d",offset,len);
	_draw_rect(buf,x,y,len/4,1);

}

void init_device() {
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
	strcpy(dispinfo,"WIDTH:400\nHEIGHT:300\n");
	Log("init_device dispinfo:%s",dispinfo);
}
