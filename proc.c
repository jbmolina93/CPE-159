// proc.c, 159
// all processes are coded here
// processes do not use kernel space (data.h) or code (handlers, tools, etc.)
// all must be done thru system service calls

#include "spede.h"      // cons_xxx below needs
#include "data.h"       // current_pid needed below
#include "proc.h"       // prototypes of processes
#include "syscalls.h"

void SystemProc(void) {
  int i;
   
   while(1) 
   {
	 //cons_printf("0 ");

	 
	//for(i=0;i<LOOP;i++)
	//{
		
		asm("inb $0x80");
	//}
 
   }
}

void UserProc(void) {
   
   
   char my_str[]=" ";
   while(1) {
     my_str[0] = GetPid() + '0';// fill out 1st space
     Write(STDOUT , my_str);
     //cons_printf("%d", run_pid);//    // STDOUT fileno == 1
     Sleep(GetPid() % 5);       // sleep for a fes seconds(PID 5?)
   }
}
