#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  //TODO();
	rtl_push(&cpu.eflags.val);
	uint32_t cs=cpu.CS.val;
	rtl_push(&cs);
	rtl_push(&ret_addr);
	GateDesc a;a.val=vaddr_read(cpu.IDTR.base+NO*8,8);
	/*a.offset_15_0=vaddr_read(cpu.IDTR.base+NO*8,2);
	a.offset_31_16=vaddr_read(cpu.IDTR.base+NO*8+6,2);*/
	printf("p:0x%x\n",a.present);
	uint32_t a1=a.offset_15_0,a2=a.offset_31_16;
	decoding.jmp_eip=(a2<<16)+a1;
	decoding.is_jmp=1;

}

void dev_raise_intr() {
}
