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
	int k=inl(0x60),k_state=inl(0x64);
	if(k_state==1)in_key=k;
  return in_key;
}
