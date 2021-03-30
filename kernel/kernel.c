#include <stdint.h>
#include "paging/paging.h"
#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "paging/bitmap.h"
#include "task/scheduler.h"
#include "task/module.h"
uint8_t kernel_main() {
    initialise_kernel_struct();
    clear_screen();
    // create_process(bonsoir);
    // create_process(naprive);
    // create_process(aurevoir);
    kprint("EKRI KOMAND OU AN\n"
        "EKRI FINI POU FEMEN OS LA\n> ");
    isr_install();
    irq_install();
    initialise_paging();


    return 0x0;
}


void initialise_kernel_struct(){
    // free mem adrr must be paged alligned
   __free_mem_addr__ = 0x1000*0xFFF;
   __mem_end_page__ = 0xF0000000;
   __kernel_process_id__=0;
   __nframes__ = __mem_end_page__ / 0x1000;
   __frames__ = (uint32_t*)kmalloc(INDEX_FROM_BIT(__nframes__));
   memory_set(__frames__, 0, INDEX_FROM_BIT(__nframes__));
   __tick__=0;


   //stuff for process simulation 
    uint32_t esp, ebp;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));
   __kernel_entry_process__ = (process_t *)kmalloc_a(sizeof(process_t *));
   __kernel_entry_process__->id = 0;
   __kernel_entry_process__->esp = esp;
   __kernel_entry_process__->ebp = ebp;
   __kernel_entry_process__->eip=bonjour;
   __kernel_entry_process__->page_directory=current_directory;
   __kernel_entry_process__->next=__kernel_entry_process__;
   __current_process__=__kernel_entry_process__;




}
