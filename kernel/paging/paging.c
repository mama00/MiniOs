#include "paging.h"
#include "../../cpu/isr.h"
#include "../../drivers/keyboard.h"
#include "../../drivers/screen.h"
#include "../../libc/mem.h"
#include "bitmap.h"
#include "../../libc/string.h"
// A bitset of __frames__ - used or free.
extern uint32_t *__frames__;
extern uint32_t __nframes__;
page_directory_t *current_directory;


// Defined in lib/mem.c
extern uint32_t __free_mem_addr__;
// Macros used in the bitset algorithms.


// Function to allocate a frame.
void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
   if (page->frame != 0)
   {
       return; // Frame was already allocated, return straight away.
   }
   else
   {
       uint32_t idx = first_frame(); // idx is now the index of the first free frame.
       if (idx == (uint32_t)-1)
       {

           kprint("No free __frames__!");
       }
       set_frame(idx*0x1000); // this frame is now ours!
       page->present = 1; // Mark it as present.
       page->rw = (is_writeable)?1:0; // Should the page be writeable?
       page->user = (is_kernel)?0:1; // Should the page be user-mode?
       page->frame = idx;
   }
}

// Function to deallocate a frame.
void free_frame(page_t *page)
{
   uint32_t frame;
   if (!(frame=page->frame))
   {
       return; // The given page didn't actually have an allocated frame!
   }
   else
   {
       clear_frame(frame); // Frame is now free again.
       page->frame = 0x0; // Page now doesn't have a frame.
   }
}

void page_fault(registers_t regs)
{
   // A page fault has occurred.
   // The faulting address is stored in the CR2 register.
   uint32_t faulting_address;
   asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

   // The error code gives us details of what happened.
   int present   = !(regs.err_code & 0x1); // Page not present
   int rw = regs.err_code & 0x2;           // Write operation?
   int us = regs.err_code & 0x4;           // Processor was in user-mode?
   int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
   int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

   // Output an error message.
   kprint("Page fault! ( ");
   if (present) {kprint("present ");}
   if (rw) {kprint("read-only ");}
   if (us) {kprint("user-mode ");}
   if (reserved) {kprint("reserved ");}
   kprint(") at ");
   char d[160]="";
   hex_to_ascii(faulting_address,d);
   kprint(d);
       kprint("\n");
       
   asm volatile("hlt");
}

void initialise_paging()
{

   // Let's make a page directory.
   page_directory_t* kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
   memory_set(kernel_directory, 0, sizeof(page_directory_t));
//    current_directory = kernel_directory;

// identity map for the kernel code
   int i = 0;
   while (i < __free_mem_addr__* 0x1000)
   {
       // Kernel code is readable but not writeable from userspace.
       alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
       i += 0x1000;
   }
   // Before we enable paging, we must register our page fault handler.
   register_interrupt_handler(14, page_fault);
   register_interrupt_handler(13,fault_13);

   // Now, enable paging!
   switch_page_directory(kernel_directory);

}

void switch_page_directory(page_directory_t *dir)
{
//    current_directory = dir;
   asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
   uint32_t cr0;
   asm volatile("mov %%cr0, %0": "=r"(cr0));
   cr0 |= 0x80000000; // Enable paging!
   asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(uint32_t address, int make, page_directory_t *dir)
{
   // Turn the address into an index.
   address /= 0x1000;
   // Find the page table containing this address.
   uint32_t table_idx = address / 1024;
   if (dir->tables[table_idx]) // If this table is already assigned
   {
       return &dir->tables[table_idx]->pages[address%1024];
   }
   else if(make)
   {
       uint32_t tmp;
       dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
       memory_set(dir->tables[table_idx], 0, 0x1000);
       dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
       return &dir->tables[table_idx]->pages[address%1024];
   }
   else
   {
       return 0;
   }
}

void fault_13(registers_t regs){
    kprint("hello 13");
}