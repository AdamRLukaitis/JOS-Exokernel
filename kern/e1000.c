#include <kern/e1000.h>

// LAB 6: Your driver code here

int e1000_attach(struct pci_func *pcif) {

   // Enable the e1000 on PCI bus
   pci_func_enable(pcif);

   // Map MMIO region in DRAM to physical addrs on the device
   bar0addr = pcif->reg_base[0];
   bar0 = mmio_map_region(bar0addr, pcif->reg_size[0]);

   // Check the status register
   cprintf("e1000 device status register: %08x\n", bar0[REGNDX(E1000_STATUS)]);

   return 1;
}
