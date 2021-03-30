
#include "scheduler.h"

extern page_directory_t *current_directory;
process_t * __kernel_entry_process__;
process_t * __current_process__;


void switch_process(){
    asm volatile("cli;");
    // Read esp, ebp now for saving later on.
    uint32_t esp, ebp, eip;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    eip = read_eip();
   if (eip == 0x12345){
        __current_process__=__current_process__->next;

   }
    //save the register of current process
    __current_process__->esp=esp;
    __current_process__->ebp=ebp;
//     __current_process__->eip=eip;

    //load the register of next process
    __current_process__=get_next_process();
    eip=__current_process__->eip;
    
    //switch task
       asm volatile("         \
     mov %0, %%ecx;       \
     mov %1, %%esp;       \
     mov %2, %%ebp;       \
     mov %3, %%cr3;       \
     mov $0x12345, %%eax; \
     sti;                 \
     jmp *%%ecx           "
                : : "r"(eip), "r"(esp), "r"(ebp), "r"(current_directory->physicalAddr));


}