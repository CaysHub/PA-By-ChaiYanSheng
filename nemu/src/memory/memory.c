#include "nemu.h"

#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
    guest_to_host(addr); \
    })

uint8_t pmem[PMEM_SIZE];
int is_mmio(paddr_t addr);
uint32_t mmio_read(paddr_t addr, int len, int map_NO);
void mmio_write(paddr_t addr, int len, uint32_t data, int map_NO);
/* Memory accessing interfaces */

uint32_t paddr_read(paddr_t addr, int len) {
	int map_NO=is_mmio(addr);
	if(map_NO>-1)return mmio_read(addr,len,map_NO);
  return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
}

void paddr_write(paddr_t addr, int len, uint32_t data) {
	int map_NO=is_mmio(addr);
	if(map_NO>-1)return mmio_write(addr,len,data,map_NO);
  memcpy(guest_to_host(addr), &data, len);
}
//D:\PAHappy\ics2017\nemu/src/memory/memory.c
paddr_t page_translate(vaddr_t addr,bool is_write);

#define CROSS_PAGE(addr, len) \
	((((addr) + (len) - 1) & ~PAGE_MASK) != ((addr) & ~PAGE_MASK))
uint32_t vaddr_read(vaddr_t addr, int len) {
	//assert(cpu.cr0.paging==1);
  if(cpu.cr0.paging==1){
	  return paddr_read(page_translate(addr,false), len);
	}else{
		return paddr_read(addr,len);
	}
}

void vaddr_write(vaddr_t addr, int len, uint32_t data) {
  if(cpu.cr0.paging==1){
	  paddr_write(page_translate(addr,true), len, data);
		return;
	}else{
		paddr_write(addr, len, data);
	}
}
paddr_t page_translate(vaddr_t addr,bool is_write){
  if(cpu.cr0.paging==0){
	  assert(0);
	}
	uint32_t page_addr=(cpu.cr3.page_directory_base<<12)+(((addr>>22)&0x000003ff)<<2);
	Log("1");
	PDE pde;pde.val=paddr_read(page_addr,4);
	assert(pde.present);
	Log("1-");
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
