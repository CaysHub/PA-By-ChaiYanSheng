#include "cpu/exec.h"

make_EHelper(test) {
  //TODO();
	rtl_and(&t0,&id_dest->val,&id_src->val);

	t1=0;
	rtl_update_ZFSF(&t0,id_dest->width);
	rtl_set_CF(&t1);
	rtl_set_OF(&t1);
	
  print_asm_template2(test);
}

make_EHelper(and) {
	uint32_t a=0;
	rtl_sext(&a,&id_src->val,id_src->width);
  rtl_and(&t0,&id_dest->val,&a);
	operand_write(id_dest,&t0);
  snprintf(id_src->str, OP_STR_SIZE, "$0x%x", a);	
	rtl_update_ZFSF(&t0,id_dest->width);

	t1=0;
	rtl_set_CF(&t1);
	rtl_set_OF(&t1);

  print_asm_template2(and);
}

make_EHelper(xor) {
  rtl_xor(&t0,&id_dest->val,&id_src->val);
  operand_write(id_dest,&t0);
  rtl_update_ZFSF(&t0,id_dest->width);
  t1=0;
  rtl_set_CF(&t1);
  rtl_set_OF(&t1);
  print_asm_template2(xor);
}

make_EHelper(or) {
  //TODO();
	rtl_or(&t0,&id_dest->val,&id_src->val);
	operand_write(id_dest,&t0);

	rtl_update_ZFSF(&t0,id_dest->width);

	t1=0;
	rtl_set_CF(&t1);
	rtl_set_OF(&t1);

  print_asm_template2(or);
}

make_EHelper(sar) {
  // TODO();
  // unnecessary to update CF and OF in NEMU
  if(id_dest->width==1){
	  int8_t dest=id_dest->val;
		t0=dest>>id_src->val;
	}else if(id_dest->width==2){
	  int16_t dest=id_dest->val;
		t0=dest>>id_src->val;
	}else{
	  rtl_sar(&t0,&id_dest->val,&id_src->val);
	}
	operand_write(id_dest,&t0);

	rtl_update_ZFSF(&t0,id_dest->width);

  print_asm_template2(sar);
}

make_EHelper(shl) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_shl(&t0,&id_dest->val,&id_src->val);
	operand_write(id_dest,&t0);

	rtl_update_ZFSF(&t0,id_dest->width);

  print_asm_template2(shl);
}

make_EHelper(shr) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_shr(&t0,&id_dest->val,&id_src->val);
	operand_write(id_dest,&t0);
	rtl_update_ZFSF(&t0,id_dest->width);

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint8_t subcode = decoding.opcode & 0xf;
	
  rtl_setcc(&t2, subcode);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(subcode), id_dest->str);
}

make_EHelper(not) {
  t0=~id_dest->val;
	operand_write(id_dest,&t0);
	rtl_update_ZFSF(&t0,id_dest->width);
  print_asm_template1(not);
}
make_EHelper(rol){
  t0=id_src->val;
	t1=id_dest->val;
	while(t0!=0){
	  rtl_msb(&t2,&t1,id_dest->width);
		t1=t1*2+t2;
		t0-=1;
	}
	operand_write(id_dest,&t1);
	if(id_src->val==1){
	  rtl_msb(&t2,&id_dest->val,id_dest->width);
		if(t2!=cpu.eflags.CF){
		  t3=1;
		}else t3=0;
		rtl_set_OF(&t3);
	}
}
