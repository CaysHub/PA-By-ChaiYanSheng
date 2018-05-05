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
	//printf("jcc:t2:0x%x\n",t2);

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
	if(decoding.is_operand_size_16){
	  t0=(*eip)&0x0000ffff;
		cpu.esp-=2;
		vaddr_write(cpu.esp,2,t0);
		decoding.jmp_eip=(*eip+id_dest->val)&0x0000ffff;
	}else{
	  t0=*eip;
	  rtl_push(&t0);
		decoding.jmp_eip=*eip+id_dest->val;
	}
  decoding.is_jmp=1;
  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
	if(decoding.is_operand_size_16){
	  rtl_lm(&t0,&cpu.esp,2);
		cpu.esp+=2;
	}else{
	  rtl_pop(&t0);
	}
	decoding.is_jmp=1;
	decoding.jmp_eip=t0;
  print_asm("ret");
}

make_EHelper(call_rm) {
  //TODO();	
  /*if(decoding.is_operand_size_16){
	  t0=(*eip)&0x0000ffff;
		cpu.esp-=2;
		rtl_sm(&cpu.esp,2,&t0);
		rtl_lm(&t0,&id_dest->addr,2);
		decoding.jmp_eip=t0&0x0000ffff;
	}else{*/
	  cpu.esp-=4;
		rtl_sm(&cpu.esp,4,eip);
		uint32_t rm=0;
		rtl_lm(&rm,&id_dest->addr,4);
		decoding.jmp_eip=rm;
	//}
	decoding.is_jmp=1;
  printf("call_rm:opcode:0x%x\tdest:0x%x\n",decoding.opcode,id_dest->val);
	printf("call_rm:edx:0x%x\n",cpu.edx);
  print_asm("call *%s", id_dest->str);
}
