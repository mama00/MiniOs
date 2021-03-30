
#include "system_call.h"
void system_call_handler(registers_t *registers){
    if (registers->eax==0x1){
        kprint("end of program");
        kprint("\n");
        remove_process(__current_process__->id);
    }
 
    asm volatile("popa");
}

void end_program(){
    asm ("pusha;");
    asm ("mov $0x1, %eax");
    asm ("int $48");
}