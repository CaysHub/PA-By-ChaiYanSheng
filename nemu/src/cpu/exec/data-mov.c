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
		  if(strcmp(regsw[i],"bp")==0)r_bp=i;
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
			if(strcmp(regsw[i],"ax")==0)r_ax=i;
			if(strcmp(regsw[i],"dx")==0)r_dx=i;
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
    int i=0,r_al=-1,r_ax=-1;
		for(i=0;i<8;i++){
		  if(strcmp(regsb[i],"al")==0)r_al=i;
			if(strcmp(regsw[i],"ax")==0)r_ax=i;
		}
		rtl_lr_b(&t0,r_al);
		rtl_sext(&t1,&t0,1);
		rtl_sr_w(r_ax,&t1);
  }
  else {
    int i=0,r_ax=-1;
		for(i=0;i<8;i++){
		  if(strcmp(regsw[i],"ax")==0)r_ax=i;
		}
		rtl_lr_w(&t0,r_ax);
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
	//printf("movzx:src:0x%x\n",id_src->val);
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  //printf("movzx:dest:0x%x\n",id_dest->val);
  operand_write(id_dest, &id_src->val);
	//if(id_dest->type==OP_TYPE_REG)printf("dest:reg:0x%x\n",id_dest->reg);
	//printf("movzx:dest_width:0x%x\n",id_dest->width);
	//printf("movzx:dest:0x%x\n",id_dest->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  rtl_li(&t2, id_src->addr);
  operand_write(id_dest, &t2);
  print_asm_template2(lea);
}
