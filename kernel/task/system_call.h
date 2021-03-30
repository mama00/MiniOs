#ifndef SYSTEM_CALL_H
#define SYSTEM_CALL_H
#include "../../cpu/isr.h"
#include "../kernel.h"
#include "../task/process.h"

extern process_t * __current_process__;
extern process_t * __kernel_entry_process__;

void system_call_handler(registers_t * reg);

// system call// 

void end_program();

#endif