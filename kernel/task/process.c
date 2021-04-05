#include "process.h"

extern process_t * __current_process__;
extern process_t * __kernel_entry_process__;
extern int __kernel_process_id__;
extern uint32_t * current_directory;

process_t * get_next_process(){
    return __current_process__->next;
}

void create_process(void * func,char * name){
    asm volatile("cli");
    //atomic adding
    __sync_fetch_and_add(&__kernel_process_id__,1);
    process_t * new_process = (process_t *)kmalloc_a(sizeof(process_t *));
    new_process->name=name;
    kprint("hi");
   new_process->id = __kernel_process_id__;
   new_process->esp = 0x0;
   new_process->ebp = 0x0;
   new_process->eip=func;
   new_process->page_directory=current_directory;
   new_process->next=__kernel_entry_process__->next;
   __kernel_entry_process__->next= new_process;
   char d[16]="";
    kprint("PWOSESIS KREYE, ID LI SE : ");
    int_to_ascii(new_process->id,d);
    kprint(d);
    kprint("\n");
    asm volatile("sti");
}

void remove_process(int id){
    // char d[16]="";
    // int_to_ascii(id,d);
    // kprint("NAP FEMEN PWOSESUS ");
    // kprint(d);
    int is=0;
    if(__current_process__->id==id)
        is=1;

    process_t * temp=__kernel_entry_process__;
    process_t * previous_temp;
    while(temp->id !=id){
        previous_temp=temp;
        temp=temp->next;
    }
    previous_temp->next=temp->next;
    __kernel_entry_process__=previous_temp;
    //if process deleted was the current process
    if(is){
        __current_process__=previous_temp;
        switch_process();
    }

}

void context_save(registers_t *registers){
    __current_process__->eip=registers->eip;

}