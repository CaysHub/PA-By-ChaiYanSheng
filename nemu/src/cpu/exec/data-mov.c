#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  //rtl_push(&id_dest->val);
	if(id_dest->width==1){
		rtl_sext(&t0,&id_dest->val,id_dest->width);
		cpu.esp-=4;
		vaddr_write(cpu.esp,4,t0);
		
	}else{
	  cpu.esp-=id_dest->width;
	  vaddr_write(cpu.esp,id_dest->width,id_dest->val);
	}
  print_asm_template1(push);
}

make_EHelper(pop) {
  // TODO();
  t0=vaddr_read(cpu.esp,id_dest->width);
	operand_write(id_dest,&t0);
	cpu.esp+=id_dest->width;
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  //TODO();
  t0=cpu.esp;
	rtl_push(&cpu.eax);
	rtl_push(&cpu.ecx);
	rtl_push(&cpu.edx);
	rtl_push(&cpu.ebx);
	rtl_push(&t0);
	rtl_push(&cpu.ebp);
	rtl_push(&cpu.esi);
	rtl_push(&cpu.edi);

  print_asm("pusha");
}

make_EHelper(popa) {
  //TODO();
  rtl_pop(&cpu.edi);
	rtl_pop(&cpu.esi);
  rtl_pop(&cpu.ebp);
	rtl_pop(&t0);
	rtl_pop(&cpu.ebx);
	rtl_pop(&cpu.edx);
	rtl_pop(&cpu.ecx);
	rtl_pop(&cpu.eax);

  print_asm("popa");
}

make_EHelper(leave) {
  cpu.esp=cpu.ebp;
	if(decoding.is_operand_size_16){
		rtl_lm(&t0,&cpu.esp,2);
		rtl_sr_w(R_BP,&t0);
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
		rtl_lr_w(&t0,R_AX);
		if(t0&0x8000)cpu.gpr[R_DX]._16=0xffff;
		else cpu.gpr[R_DX]._16=0;
  }
  else {
    if(cpu.eax&0x80000000)cpu.edx=0xffffffff;
		else cpu.edx=0;
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
		rtl_lr_b(&t0,R_AL);
		rtl_sext(&t1,&t0,1);
		t1=t1&0x0000ffff;
		rtl_sr_w(R_AX,&t1);
  }
  else {
		rtl_lr_w(&t0,R_AX);
		rtl_sext(&t1,&t0,2);
		cpu.eax=t1;
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
  rtl_li(&t2, id_src->addr);
  operand_write(id_dest, &t2);
  print_asm_template2(lea);
}
