#include "common.h"

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t len) {
  return 0;
}

static char dispinfo[128] __attribute__((used));

void dispinfo_read(void *buf, off_t offset, size_t len) {
	memcpy(buf,dispinfo+offset,len);
}

void fb_write(const void *buf, off_t offset, size_t len) {
	int i=(offset/_screen.height)%_screen.width,j=offset-i*_screen.height;
	Log("fb_write x:%d\ty:%d",i,j);
	_draw_rect(buf,0,0,1,1);

}

void init_device() {
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
	strcpy(dispinfo,"WIDTH:400\nHEIGHT:300\n");
	Log("init_device dispinfo:%s",dispinfo);
}
