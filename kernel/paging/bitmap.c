
#include "bitmap.h"
// Static function to set a bit in the frames bitset
extern uint32_t *__frames__;
extern uint32_t __nframes__;

uint32_t INDEX_FROM_BIT(uint32_t a){
   return  (a/(8*4));
}

uint32_t OFFSET_FROM_BIT( uint32_t a){
    return (a%(8*4));
}

 void set_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   __frames__[idx] |= (0x1 << off);
}

 void clear_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   __frames__[idx] &= ~(0x1 << off);
}

 uint32_t test_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   return (__frames__[idx] & (0x1 << off));
}

// Static function to find the first free frame.
 uint32_t first_frame()
{
   uint32_t i, j;
   for (i = 0; i < INDEX_FROM_BIT(__nframes__); i++)
   {
       if (__frames__[i] != 0xFFFFFFFF) // nothing free, exit early.
       {
           // at least one bit is free here.
           for (j = 0; j < 32; j++)
           {
               uint32_t toTest = 0x1 << j;
               if ( !(__frames__[i]&toTest) )
               {
                   return i*4*8+j;
               }
           }
       }
   }
}