
#include "fs.h"

// Return the virtual address of this disk block.
void*
diskaddr(uint32_t blockno)
{
	if (blockno == 0 || (super && blockno >= super->s_nblocks))
		panic("bad block number %08x in diskaddr", blockno);
	return (char*) (DISKMAP + blockno * BLKSIZE);
}

// Fault any disk block that is read in to memory by
// loading it from disk.
static void
bc_pgfault(struct UTrapframe *utf)
{
	void *addr = (void *) utf->utf_fault_va;
	uint32_t blockno = ((uint32_t)addr - DISKMAP) / BLKSIZE;
	int r;

	// Check that the fault was within the block cache region
	if (addr < (void*)DISKMAP || addr >= (void*)(DISKMAP + DISKSIZE))
		panic("page fault in FS: eip %08x, va %08x, err %04x",
		      utf->utf_eip, addr, utf->utf_err);

	// Sanity check the block number.
	if (super && blockno >= super->s_nblocks)
		panic("reading non-existent block %08x\n", blockno);

	// Allocate a page in the disk map region, read the contents
	// of the block from the disk into that page.
	// Hint: first round addr to page boundary. fs/ide.c has code to read
	// the disk.
	//
	// LAB 5: you code here:

   void *algn_addr = ROUNDDOWN(addr, PGSIZE);
   uint32_t secno = ((uint32_t)algn_addr - DISKMAP) / SECTSIZE;
   int error;

   // Create a new page
   if ((error = sys_page_alloc(0, algn_addr, PTE_W | PTE_U | PTE_P)) < 0)
      panic("bc_pgfault: sys_page_alloc %e", error);
   // Read block from disk and insert into page
   if ((error = ide_read(secno, algn_addr, PGSIZE / SECTSIZE)) < 0)
      panic("bc_pgfault: ide_read %d", error);
}


void
bc_init(void)
{
	struct Super super;
	set_pgfault_handler(bc_pgfault); 
	// cache the super block by reading it once
	memmove(&super, diskaddr(1), sizeof super);
}

