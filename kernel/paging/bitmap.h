#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>


 void set_frame(uint32_t frame_addr);
 void clear_frame(uint32_t frame_addr);
 uint32_t test_frame(uint32_t frame_addr);

uint32_t INDEX_FROM_BIT( uint32_t a);
uint32_t OFFSET_FROM_BIT(uint32_t a);





#endif