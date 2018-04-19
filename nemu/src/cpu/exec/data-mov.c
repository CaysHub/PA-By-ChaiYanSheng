#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  rtl_push(&id_dest->val);

  print_asm_template1(push);
}

make_EHelper(pop) {
  // TODO();
  rtl_pop(&id_dest->val);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  cpu.esp=cpu.ebp;
	if(decoding.is_operand_size_16){
	  uint16_t src=vaddr_read(cpu.esp,2);
		int r_bp=0,i;
		for(i=0;i<8;i++){
		  if(strcmp(regsb[i],"bp")==0)r_bp=i;
		}
		cpu.gpr[r_bp]._16=src;
		cpu.esp+=2;
	}else{
	  uint32_t src;
		rtl_lm(&src,&cpu.esp,4);
		cpu.ebp=src;
		cpu.esp+=4;
	}

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    int r_ax=0,i,r_dx=0;
		for(i=0;i<8;i++){
			if(strcmp(regsb[i],"ax")==0)r_ax=i;
			if(strcmp(regsb[i],"dx")==0)r_dx=i;
		}
		uint16_t ax=cpu.gpr[r_ax]._16;
		if(ax&0x8000)cpu.gpr[r_dx]._16=0xffff;
		else cpu.gpr[r_dx]._16=0;
  }
  else {
    if(cpu.eax&0x80000000)cpu.edx=0xffffffff;
		else cpu.edx=0;
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t2, &id_src->val, id_src->width);
  operand_write(id_dest, &t2);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
	printf("lea:ebp:0x%x\n",cpu.ebp);
  rtl_li(&t2, id_src->addr);
  operand_write(id_dest, &t2);
  print_asm_template2(lea);
}
