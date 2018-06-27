#include "cpu/exec.h"

void diff_test_skip_qemu();
void diff_test_skip_nemu();
void raise_intr(uint8_t NO, vaddr_t ret_addr);

make_EHelper(lidt) {
  //TODO();
	cpu.IDTR.limit=vaddr_read(cpu.eax,2);
	cpu.IDTR.base=vaddr_read(cpu.eax+2,4);
	//printf("lidt:0x%x\n",vaddr_read(id_dest->addr,2));
  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  if(id_dest->reg==0){
	  cpu.cr0.val=id_src->val;
	}else if(id_dest->reg==3){
	  cpu.cr3.val=id_src->val;
	}else{
	  Assert(0, "unsupported control register");
	}

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  if(id_src->reg==0){
	  t0=cpu.cr0.val;
	}else if(id_src->reg==3){
	  t0=cpu.cr3.val;
	}else{
	  Assert(0, "unsupported control register");
	}
	operand_write(id_dest, &t0);

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}

make_EHelper(int) {
  //TODO();
	raise_intr(id_dest->val,*eip);
  print_asm("int %s", id_dest->str);

#ifdef DIFF_TEST
  diff_test_skip_nemu();
#endif
}

make_EHelper(iret) {
  //TODO();
	rtl_pop(&t0);
	rtl_pop(&t1);
	cpu.CS.val=t1&0xffff;
	rtl_pop(&cpu.eflags.val);
	decoding.jmp_eip=t0;
	decoding.is_jmp=1;
  print_asm("iret");
}

uint32_t pio_read(ioaddr_t, int);
void pio_write(ioaddr_t, int, uint32_t);

make_EHelper(in) {
  // TODO();
  if(decoding.is_operand_size_16)t0=pio_read(id_src->val,2);
	else t0=pio_read(id_src->val,id_src->width);
	operand_write(id_dest,&t0);
	
  print_asm_template2(in);

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}

make_EHelper(out) {
  // TODO();
	if(decoding.is_operand_size_16)pio_write(id_dest->val,2,id_src->val);
  else pio_write(id_dest->val,id_src->width,id_src->val);
  print_asm_template2(out);

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}
