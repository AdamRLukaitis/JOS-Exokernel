#ifndef JOS_INC_FLEXSC_H
#define JOS_INC_FLEXSC_H

#include <inc/types.h>
#include <inc/trap.h>
#include <inc/memlayout.h>

#define NSCENTRIES 64         // Number of syscall entries per syscall page

enum FscStatus {
   FSC_FREE = 0,
   FSC_SUBMIT,
   FSC_BUSY, 
   FSC_DONE
};

struct FscEntry {
   int32_t syscall;           // Syscall number  
   uint32_t num_args;         // Number of arguments
   uint32_t args[5];          // Arguments
   enum FscStatus status;     // Syscall status 
   int32_t ret;               // Syscall return value
   int8_t _pad[28];           // Pad it up to 64 bytes
};

// Syscall page is 4K bytes
struct FscPage {
   struct FscEntry entries[NSCENTRIES];
};

#endif