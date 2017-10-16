// handlers.c, 159

#include "spede.h"
#include "types.h"
#include "data.h"
#include "tools.h"
#include "proc.h"
#include "handlers.h"

// to create process, 1st alloc PID, PCB, and process stack space
// build process frame, initialize PCB, record PID to run_q (if not 0)
void NewProcHandler(func_p_t p) {  // arg: where process code starts
   int pid;

   if(0 == ready_q.size) { // may occur as too many processes been created
      cons_printf("Kernel Panic: cannot create more process!\n");
      return;                   // alternative: breakpoint() into GDB
   }

   pid = DeQ(&ready_q);
   MyBzero((char *)&pcb[pid], sizeof(pcb_t));
   pcb[pid].state = RUN;
   EnQ(pid, &run_q);

   pcb[pid].proc_frame_p = (proc_frame_t *)&proc_stack[pid][4096];
   //pcb[pid].proc_frame_p = (proc_frame_t *)&proc_stack[pid][PROC_STACK_SIZE-sizeof(proc_frame_t)];

   pcb[pid].proc_frame_p->EFL = EF_DEFAULT_VALUE|EF_INTR;
   pcb[pid].proc_frame_p->EIP =(int)p;
   pcb[pid].proc_frame_p->CS = get_cs();
}

// count run_time of running process and preempt it if reaching time slice
void TimerHandler(void) {
int i;

  timer_ticks++;
   //if(timer_ticks++ == 75) 
     //cons_printf(".");

for (i=0; i<PROC_NUM; i++)
{
if(pcb[i].state == SLEEPING && pcb[i].wake_time == timer_ticks){
  pcb[i].state=RUN;
  EnQ(i, &run_q);
}
pcb[run_pid].run_time++;
}

   outportb(0x20, 0x60); // 0x61 0x62

  if(run_pid == 0) return;

   pcb[run_pid].run_time++;
   if(pcb[run_pid].run_time == TIME_SLICE) {
      if(pcb[run_pid].state == READY){
        pcb[run_pid].state=RUN;

    }else if(pcb[run_pid].state ==RUN)
      pcb[run_pid].state = READY; 
    }
      EnQ(run_pid, &run_q);
      run_pid = -1;
//outportb(0x20,0x60);
}
void WriteHandler(proc_frame_t p){
  char *str;
  
  if (p.EBX == STDOUT){
    str = (char *)p.ECX;
    cons_printf("%s ",str);
  }
}
void GetPidHandler(void){
  outportb(0x20,0x60);
  pcb[run_pid].proc_frame_p ->EAX = run_pid;
  return;
}
void SleepHandler(proc_frame_t p){
  timer_ticks++;
  outportb(0x20, 0x60);
  pcb[run_pid].wake_time = timer_ticks + (100 * p.EBX);// seconds to sleep EBX
  pcb[run_pid].state = SLEEPING;
  run_pid = -1;
  return;
  
}





