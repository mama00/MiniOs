
#include "system_call.h"
void system_call_handler(registers_t *registers){
    asm volatile("cli");
    if (registers->eax==0x1){
        kprint('\n');
        remove_process(__current_process__->id);
        asm volatile("sti");
    }
 
    kprint('\n');
    kprint(">");
    asm volatile("popa");
}

void end_program(){
    asm ("pusha;");
    asm ("mov $0x1, %eax");
    asm ("int $48");
}