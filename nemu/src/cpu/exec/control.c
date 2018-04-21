#include "cpu/exec.h"

make_EHelper(jmp) {
  // the target address is calculated at the decode stage
  decoding.is_jmp = 1;

  print_asm("jmp %x", decoding.jmp_eip);
}

make_EHelper(jcc) {
  // the target address is calculated at the decode stage
  uint8_t subcode = decoding.opcode & 0xf;
  rtl_setcc(&t2, subcode);
  decoding.is_jmp = t2;

  print_asm("j%s %x", get_cc_name(subcode), decoding.jmp_eip);
}

make_EHelper(jmp_rm) {
  decoding.jmp_eip = id_dest->val;
  decoding.is_jmp = 1;

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  // the target address is calculated at the decode stage
  // TODO();
	t0=*eip+5;
	printf("call:push:eip:0x%x\n",t0);
	rtl_push(&t0);
  decoding.is_jmp=1;
	decoding.jmp_eip=*eip+id_dest->val;
  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
	uint32_t a=0;
  rtl_pop(&a);
	decoding.is_jmp=1;
	decoding.jmp_eip=a;
  print_asm("ret");
}

make_EHelper(call_rm) {
  TODO();

  print_asm("call *%s", id_dest->str);
}
