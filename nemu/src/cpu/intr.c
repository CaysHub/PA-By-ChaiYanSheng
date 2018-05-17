#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  //TODO();
	printf("IDTR.base:0x%x\n",cpu.IDTR.base);
	rtl_push(&cpu.eflags.val);
	cpu.esp-=2;uint32_t cs=cpu.CS.val;
	rtl_sm(&cpu.esp,2,&cs);
	rtl_push(&ret_addr);
	GateDesc a;
	a.offset_15_0=vaddr_read(cpu.IDTR.base+NO*8,2);
	a.offset_31_16=vaddr_read(cpu.IDTR.base+NO*8+6,2);
	//a.p=(vaddr_read(cpu.IDRT.base+NO*8+4,1)>>15)&0x1;
	uint32_t a1=a.offset_15_0,a2=a.offset_31_16;
	decoding.jmp_eip=(a2<<16)+a1;
	decoding.is_jmp=1;

}

void dev_raise_intr() {
}
