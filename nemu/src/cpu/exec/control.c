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
	t0=cpu.eip+id_dest->width+1;
	  rtl_push(&t0);
		//printf("0x%x\n",cpu.eip+id_dest->width+1);
		cpu.eip=cpu.eip+id_dest->val+1+4;
	
  decoding.is_jmp=1;
	decoding.jmp_eip=cpu.eip;
  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
  rtl_li(&t0,id_dest->width);
	uint32_t a=0,b=vaddr_read(cpu.esp,4);
  rtl_pop(&a);
	printf("0x%x\nt0:0x%x\nb:0x%x\n",a,t0,b);
	cpu.eip=t1;
	decoding.seq_eip=cpu.eip;
	decoding.is_jmp=1;
  print_asm("ret");
}

make_EHelper(call_rm) {
  TODO();

  print_asm("call *%s", id_dest->str);
}
