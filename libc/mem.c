#include "mem.h"


extern uint32_t __free_mem_addr__;
void memory_copy(uint8_t *source, uint8_t *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void memory_set(uint8_t *dest, uint8_t val, uint32_t len) {
    uint8_t *temp = (uint8_t *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}


uint32_t kmalloc(uint32_t sz)
{
  uint32_t tmp = __free_mem_addr__;
  __free_mem_addr__ += sz;
  return tmp;
}

uint32_t kmalloc_a(uint32_t sz)
{
  if ((__free_mem_addr__ & 0xFFFFF000)) // If the address is not already page-aligned
  {
    // Align it.
    __free_mem_addr__ &= 0xFFFFF000;
    __free_mem_addr__ += 0x1000;
  }
  uint32_t tmp = __free_mem_addr__;
  __free_mem_addr__ += sz;
  return tmp;
}

uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys)
{
  if ((__free_mem_addr__ & 0xFFFFF000)) // If the address is not already page-aligned
  {
    // Align it.
    __free_mem_addr__ &= 0xFFFFF000;
    __free_mem_addr__ += 0x1000;
  }
  if (phys)
  {
    *phys = __free_mem_addr__;
  }
  uint32_t tmp = __free_mem_addr__;
  __free_mem_addr__ += sz;
  return tmp;
}

uint32_t kmalloc_p(uint32_t sz, uint32_t *phys)
{
  if (phys)
  {
    *phys = __free_mem_addr__;
  }
  uint32_t tmp = __free_mem_addr__;
  __free_mem_addr__ += sz;
  return tmp;
}


