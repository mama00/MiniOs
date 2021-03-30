#ifndef KERNEL_H
#define KERNEL_H
#include "paging/paging.h"
#include "task/process.h"
void user_input(char *input);
void initialise_kernel_struct();
uint32_t *__frames__;
uint32_t __nframes__;
uint32_t __mem_end_page__;
uint32_t __free_mem_addr__;
uint32_t __tick__;
int __kernel_process_id__; //to keep track of the last kernel process id assigned

extern page_directory_t *current_directory;

process_t * __kernel_entry_process__;
process_t * __current_process__;



#endif
