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
	  uint16_t ip=0;int i=0;
		for(i=0;i<8;i++)
			if(strcmp(regsw[i],"ip")==0){
			  ip=reg_w(i);break;
			}
		cpu.esp-=2;
		vaddr_write(cpu.esp,2,ip);
		decoding.jmp_eip=(*eip+id_dest->val)&0x0000ffff;
	}else{
	  t0=*eip;
	  rtl_push(&t0);
		decoding.jmp_eip=*eip+id_dest->val;
	}
  decoding.is_jmp=1;
	//decoding.jmp_eip=*eip+id_dest->val;
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
  //TODO();
	
  if(decoding.is_operand_size_16){
	  uint16_t ip=0;int i=0;
		for(i=0;i<8;i++)
			if(strcmp(regsw[i],"ip")==0){
				ip=reg_w(i);break;
			}
		cpu.esp-=2;
		vaddr_write(cpu.esp,2,ip);
		uint16_t rm=vaddr_read(id_dest->addr,2);
		decoding.jmp_eip=rm&0x0000ffff;
	}else{
	  cpu.esp-=4;
		printf("ret:esp:0x%x\n",cpu.esp);
		vaddr_write(cpu.esp,4,*eip);
		//printf("dest:0x%x\n",id_dest->addr);
		uint32_t rm=vaddr_read(id_dest->addr,4);
		decoding.jmp_eip=rm;
	}
	decoding.is_jmp=1;

  print_asm("call *%s", id_dest->str);
}
