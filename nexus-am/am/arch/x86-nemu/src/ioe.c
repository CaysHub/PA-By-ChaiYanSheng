#include <am.h>
#include <x86.h>

#define RTC_PORT 0x48   // Note that this is not standard
static unsigned long boot_time;

void _ioe_init() {
  boot_time = inl(RTC_PORT);
}

unsigned long _uptime() {
	unsigned long current_seconds=inl(RTC_PORT)-boot_time;
  return current_seconds;
}

uint32_t* const fb = (uint32_t *)0x40000;

_Screen _screen = {
  .width  = 400,
  .height = 300,
};

extern void* memcpy(void *, const void *, int);

void _draw_rect(const uint32_t *pixels, int x, int y, int w, int h) {
  int i;
  for (i = 0; i < _screen.width * _screen.height; i++) {
    fb[i] = i;
  }
}

void _draw_sync() {
}

int _read_key() {
	int in_key=_KEY_NONE;
	unsigned int a=0x64,b=0x60;
	unsigned int k_state=inl(a);
	if((k_state&0x1)==1)in_key=inl(b);
  return in_key;
}
