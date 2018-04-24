#include "cpu/exec.h"

make_EHelper(test) {
  //TODO();
	rtl_and(&t0,&id_dest->val,&id_src->val);
	//operand_write(id_dest,&t0);
	printf("test:dest:0x%x\tsrc:0x%x\n",id_dest->val,id_src->val);
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
  rtl_sar(&t0,&id_dest->val,&id_src->val);
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
	printf("setcc:opcode:0x%x\n",decoding.opcode);
	printf("setcc:t2:0x%x\n",t2);
  rtl_setcc(&t2, subcode);
  operand_write(id_dest, &t2);
	printf("setcc:t2:0x%x\n",t2);

  print_asm("set%s %s", get_cc_name(subcode), id_dest->str);
}

make_EHelper(not) {
  t0=~id_dest->val;
	operand_write(id_dest,&t0);
	rtl_update_ZFSF(&t0,id_dest->width);
  print_asm_template1(not);
}
