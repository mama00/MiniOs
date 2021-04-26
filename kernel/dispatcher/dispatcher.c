#include "dispatcher.h"
#include "../../libc/string.h"
#include "../../drivers/screen.h"
#include "../kernel.h"
#include "../task/process.h"
#include "../task/module.h"
void dispatch(char * command_string){
    asm volatile("cli");
    // char *command=command_string;
    char command[16]="";
    char param[16]="";
    int i=0;
    while(command_string[i]!=' ' && command_string[i]!='\0'){
        command[i]=command_string[i];
        i++;
    }
    command[i]='\0';
    int j=0;
    i++;
   if(command_string[i-2]!='\0'){

        while(command_string[i]!='\0'){
            param[j]=command_string[i];
            i++;
            j++;
        }
   }

    param[j]='\0';

    if(strcmp(command,"FINI")==0){
        kprint("OS LA AP FEMEN OREVWA");
        asm volatile("hlt");
    }
    else if(strcmp(command,"KILL")==0){
        remove_process(string_to_int(param));
        kprint("\n");
        kprint(">");
    }
    else if(strcmp(command,"LS")==0){
        kprint(" LIS PWOSESIS :  P1  P2\n");
        kprint("  TAPE EXEC P1 OUBYEN EXEC P2 POU LANSE YO\n");
        kprint(">");
    }
    else if(strcmp(command,"PS")==0){
        kprint("LIS PWOSESIS");
        kprint("\n");
        process_list();
        kprint("\n");
        kprint(">");
    }
    else if (strcmp(command,"SH")==0){
        kprint("p1 count:  ");
        char ca[32]="";
        int_to_ascii(p1_count,ca);
        kprint(ca);
        kprint("\n");
        kprint("p2 count:  ");
        int_to_ascii(p2_count,ca);
        kprint(ca);
        kprint("\n");

    }
    else if(strcmp(command,"EXEC")==0){
        if(strcmp(param,"P1")==0){
            char name[32]="P1";
            create_process(p1,name);
            kprint(">");

        }else if(strcmp(param,"P2")==0){
            char name[32]="P2";
            create_process(p2,name);
            kprint(">");
        }
        else if(strcmp(param,"P3")==0){
            char name[32]="P3";
            create_process(p3,name);
            kprint(">");       
        }
        else{
            kprint("Prosesis sa ap existe");
            kprint("\n");
            kprint(">");

        }
    }

    else if(strcmp(command,"ED")==0){
        kprint("liste komand ou ka pase");
        kprint("\n");
        kprint(">");
        kprint("\n");
    }

    else{
        kprint("Pa gen komand sa <");
        kprint(command);
        kprint(">");
        kprint("\n");
        kprint(">");
    }

 asm volatile("sti");
}