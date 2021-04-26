
#include "module.h"

extern p1_count;
extern p2_count;
void bonjour(){
    while(1){
    }

}

void p1(){
    while(1){
        p1_count+=1;
    }
    end_program();
}

void p2(){
    while(1){
        p2_count+=1;
    }
    end_program();
}

void p3(){
       uint32_t *ptr = (uint32_t*)0xA00000000000000;
   uint32_t do_page_fault = *ptr;
    end_program();
}

void bonsoir(){
    while(1){
        
        kprint("bonsoir");
        kprint("\n");
    }

}

void aurevoir(){
    while(1){
        kprint("hi");
        kprint("\n");
    }

}

void naprive(){

        kprint("");
        end_program();

}