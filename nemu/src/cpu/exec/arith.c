#include "cpu/exec.h"

make_EHelper(add) {
  rtl_add(&t0,&id_dest->val,&id_src->val);
	rtl_sltu(&t1,&t0,&id_dest->val);
	operand_write(id_dest,&t0);
	rtl_update_ZFSF(&t0,id_dest->width);

	rtl_sltu(&t2,&t0,&id_dest->val);
	rtl_or(&t1,&t2,&t1);
	rtl_set_CF(&t1);

	rtl_xor(&t1, &id_dest->val, &id_src->val);
	rtl_not(&t1);
  rtl_xor(&t2, &id_dest->val, &t0);
	rtl_and(&t1, &t1, &t2);
	rtl_msb(&t1, &t1, id_dest->width);
	rtl_set_OF(&t1);
  print_asm_template2(add);
}

make_EHelper(sub) {
  // TODO();
	rtl_sub(&t2,&id_dest->val,&id_src->val);
	t1=id_dest->val;
	operand_write(id_dest,&t2);
	rtl_update_ZFSF(&t2,id_dest->width);

	t2=~id_src->val;
	int i=0,an=1,an_1=0;
	for(i=1;i<=8*id_dest->width;i++){
	  int dest=t1&0x1,src=t2&0x1;
		t1>>=1;t2>>=1;
		int a=an+dest+src;
		if(a==2||a==3)an=1;
		else an=0;
		if(i==id_dest->width*8-1)an_1=an;
	}
  t3=(!an)?1:0;rtl_set_CF(&t3);
	t3=an^an_1;rtl_set_OF(&t3);
  print_asm_template2(sub);
}

make_EHelper(cmp) {
  // TODO();
  t0=id_dest->val;
	t3=id_src->val;
	if(id_dest->width>id_src->width)rtl_sext(&t1,&t3,id_src->width);
	else t1=t3;
	if(id_dest->width==2)t1=t1&0x0000ffff;
	else if(id_dest->width==1)t1=t1&0x000000ff;
//	printf("cmp:t0:0x%x\nt1:0x%x\n",t0,t1);
	rtl_sub(&t2,&t0,&t1);

	rtl_update_ZFSF(&t2,id_dest->width);

	uint32_t at1=~t1;
	int cin=1,acin=0,i=0;
	for(i=1;i<=8*id_dest->width;i++){
	  int a1=at1&0x1;
		at1=at1>>1;
		int a2=t0&0x1;
		t0=t0>>1;
		int r=a1+a2+cin;
		if(r==2||r==3)cin=1;
		else cin=0;
		if(i==(8*id_dest->width-1))acin=cin;
	}
	rtlreg_t cf=(!cin)?1:0;rtl_set_CF(&cf);
	//printf("cmp:CF:0x%x\n",cf);
	uint32_t of=cin^acin;rtl_set_OF(&of);

  print_asm_template2(cmp);
}

make_EHelper(inc) {
	t1=1;uint32_t a=id_dest->val;
  rtl_add(&t0,&id_dest->val,&t1);

	operand_write(id_dest,&t0);
	rtl_update_ZFSF(&t0,id_dest->width);

  int cin=0,acin=0,i=0;
  for(i=1;i<=8*id_dest->width;i++){
	  int a1=t1&1;t1=t1>>1;
		int a2=a&1;a=a>>1;
		int x=a1+a2+cin;
		if(x==2||x==3)cin=1;
		else cin=0;
		if(i==(8*id_dest->width-1))acin=cin;
	}
	rtlreg_t cf=(cin)?1:0;rtl_set_CF(&cf);
	uint32_t of=cin^acin;rtl_set_OF(&of);

  print_asm_template1(inc);
}

make_EHelper(dec) {
  t1=id_dest->val;uint32_t src=1;
  rtl_sub(&t2,&t1,&src);

	operand_write(id_dest,&t2);
	rtl_update_ZFSF(&t2,id_dest->width);

 int cin=1,acin=0,i=0;uint32_t a=~src;
 for(i=1;i<=8*id_dest->width;i++){
	 int a1=t1&1;t1=t1>>1;
	 int a2=a&1;a=a>>1;
	 int x=a1+a2+cin;
	 if(x==2||x==3)cin=1;
	 else cin=0;
	 if(i==(8*id_dest->width-1))acin=cin;
 }
 rtlreg_t cf=(cin)?1:0;rtl_set_CF(&cf);
 uint32_t of=cin^acin;rtl_set_OF(&of);

  print_asm_template1(dec);
}

make_EHelper(neg) {
  TODO();
	t1=0;t2=id_dest->val;
  t0=-t2;
	operand_write(id_dest,&t0);

	rtl_update_ZFSF(&t0,id_dest->width);

  uint32_t at2=~t2; 
	int cin=1,acin=0,i=0;
	for(i=1;i<=8*id_dest->width;i++){
		int a1=at2&0x1;
		at2=at2>>1;
		int a2=t1&0x1;
		t1=t1>>1;
		int r=a1+a2+cin;
		if(r==2||r==3)cin=1;
		else cin=0;
		if(i==(8*id_dest->width-1))acin=cin;
	}
	rtlreg_t cf=(cin)?1:0;rtl_set_CF(&cf);
	uint32_t of=cin^acin;rtl_set_OF(&of);

  print_asm_template1(neg);
}

make_EHelper(adc) {
  rtl_add(&t2, &id_dest->val, &id_src->val);
  rtl_sltu(&t3, &t2, &id_dest->val);
  rtl_get_CF(&t1);
  rtl_add(&t2, &t2, &t1);
  operand_write(id_dest, &t2);

  rtl_update_ZFSF(&t2, id_dest->width);

  rtl_sltu(&t0, &t2, &id_dest->val);
  rtl_or(&t0, &t3, &t0);
  rtl_set_CF(&t0);

  rtl_xor(&t0, &id_dest->val, &id_src->val);
  rtl_not(&t0);
  rtl_xor(&t1, &id_dest->val, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  print_asm_template2(adc);
}

make_EHelper(sbb) {
  rtl_sub(&t2, &id_dest->val, &id_src->val);
  rtl_sltu(&t3, &id_dest->val, &t2);
  rtl_get_CF(&t1);
  rtl_sub(&t2, &t2, &t1);
	
  operand_write(id_dest, &t2);

  rtl_update_ZFSF(&t2, id_dest->width);

  rtl_sltu(&t0, &id_dest->val, &t2);
  rtl_or(&t0, &t3, &t0);
  rtl_set_CF(&t0);

  rtl_xor(&t0, &id_dest->val, &id_src->val);
  rtl_xor(&t1, &id_dest->val, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  print_asm_template2(sbb);
}

make_EHelper(mul) {
  rtl_lr(&t0, R_EAX, id_dest->width);
  rtl_mul(&t0, &t1, &id_dest->val, &t0);

  switch (id_dest->width) {
    case 1:
      rtl_sr_w(R_AX, &t1);
      break;
    case 2:
      rtl_sr_w(R_AX, &t1);
      rtl_shri(&t1, &t1, 16);
      rtl_sr_w(R_DX, &t1);
      break;
    case 4:
      rtl_sr_l(R_EDX, &t0);
      rtl_sr_l(R_EAX, &t1);
      break;
    default: assert(0);
  }

  print_asm_template1(mul);
}

// imul with one operand
make_EHelper(imul1) {
  rtl_lr(&t0, R_EAX, id_dest->width);
  rtl_imul(&t0, &t1, &id_dest->val, &t0);

  switch (id_dest->width) {
    case 1:
      rtl_sr_w(R_AX, &t1);
      break;
    case 2:
      rtl_sr_w(R_AX, &t1);
      rtl_shri(&t1, &t1, 16);
      rtl_sr_w(R_DX, &t1);
      break;
    case 4:
      rtl_sr_l(R_EDX, &t0);
      rtl_sr_l(R_EAX, &t1);
      break;
    default: assert(0);
  }

  print_asm_template1(imul);
}

// imul with two operands
make_EHelper(imul2) {
  rtl_sext(&id_src->val, &id_src->val, id_src->width);
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

  rtl_imul(&t0, &t1, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t1);

  print_asm_template2(imul);
}

// imul with three operands
make_EHelper(imul3) {
  rtl_sext(&id_src->val, &id_src->val, id_src->width);
  rtl_sext(&id_src2->val, &id_src2->val, id_src->width);
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

  rtl_imul(&t0, &t1, &id_src2->val, &id_src->val);
  operand_write(id_dest, &t1);

  print_asm_template3(imul);
}

make_EHelper(div) {
  switch (id_dest->width) {
    case 1:
      rtl_li(&t1, 0);
      rtl_lr_w(&t0, R_AX);
      break;
    case 2:
      rtl_lr_w(&t0, R_AX);
      rtl_lr_w(&t1, R_DX);
      rtl_shli(&t1, &t1, 16);
      rtl_or(&t0, &t0, &t1);
      rtl_li(&t1, 0);
      break;
    case 4:
      rtl_lr_l(&t0, R_EAX);
      rtl_lr_l(&t1, R_EDX);
      break;
    default: assert(0);
  }

  rtl_div(&t2, &t3, &t1, &t0, &id_dest->val);

  rtl_sr(R_EAX, id_dest->width, &t2);
  if (id_dest->width == 1) {
    rtl_sr_b(R_AH, &t3);
  }
  else {
    rtl_sr(R_EDX, id_dest->width, &t3);
  }

  print_asm_template1(div);
}

make_EHelper(idiv) {
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

  switch (id_dest->width) {
    case 1:
      rtl_lr_w(&t0, R_AX);
      rtl_sext(&t0, &t0, 2);
      rtl_msb(&t1, &t0, 4);
      rtl_sub(&t1, &tzero, &t1);
      break;
    case 2:
      rtl_lr_w(&t0, R_AX);
      rtl_lr_w(&t1, R_DX);
      rtl_shli(&t1, &t1, 16);
      rtl_or(&t0, &t0, &t1);
      rtl_msb(&t1, &t0, 4);
      rtl_sub(&t1, &tzero, &t1);
      break;
    case 4:
      rtl_lr_l(&t0, R_EAX);
      rtl_lr_l(&t1, R_EDX);
      break;
    default: assert(0);
  }

  rtl_idiv(&t2, &t3, &t1, &t0, &id_dest->val);

  rtl_sr(R_EAX, id_dest->width, &t2);
  if (id_dest->width == 1) {
    rtl_sr_b(R_AH, &t3);
  }
  else {
    rtl_sr(R_EDX, id_dest->width, &t3);
  }

  print_asm_template1(idiv);
}
