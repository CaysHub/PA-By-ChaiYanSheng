#include "nemu.h"
paddr_t page_translate(vaddr_t addr,bool is_write){
	if(!cpu.cr0.paging){
	  assert(0);
	}
	uint32_t page_addr=(cpu.cr3.page_directory_base<<12)+(((addr>>22)&0x000003ff)<<2);
	PDE pde;
	pde.val=paddr_read(page_addr,4);
	assert(pde.present);
	PTE pte;pte.val=paddr_read((pde.page_frame<<12)+(((addr>>12)&0x000003ff)<<2),4);
	if(pde.accessed==0){
	  pde.accessed=1;
	}
	pte.accessed=1;
	if(is_write){
	  pte.dirty=1;
	}
  paddr_t paddr=pte.page_frame<<12;
	Log("page_translate vaddr 0x%x --> paddr 0x%x ",addr,paddr);
	return paddr;
}
