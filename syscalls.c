// syscalls.c
// API calls to kernel system services

// GetPid() call
int GetPid(void) {          // no input, has return
   int pid;

   asm("pushl %%EAX;        
        movl $100, %%EAX;  
        int $128;          
        movl %%EAX, %0;    
        popl %%EAX"        // restore original EAX
       : "=g" (pid)         // output syntax, for output argument
       :                    // no input items
    );

   return pid;
}

// Write() call
void Write(int fileno, char *p) {
 
  asm("
      pushl %%EAX;
      pushl %%EBX;
      pushl %%ECX;
      movl $4, %%EAX;
      movl %0, %%EBX;
      movl %1, %%ECX;
      int $128;
      popl %%ECX;
      popl %%EBX;
      popl %%EAX;" // asm("
   //   save registers that will be used here
   //   send in service #, fileno, and p via
   //   three suitable registers
   //   issue syscall
   //   recover those saved registers
       :          // no outputs, otherwise, use "=g" (...)
       : "g" (fileno), "g" ((int)p)  // inputs, %0 and %1
       );
}

// Sleep() call
void Sleep(int sleep_time){

  asm("pushl %%EAX;
       pushl %%EBX;
       movl %0 , %%EBX;
       movl $101, %%EAX;
       int $128;
       popl %%EBX;
       popl %%EAX;"
    :
    :"g" (sleep_time) 
  );
}
