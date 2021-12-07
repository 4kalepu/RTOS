#include "ex.h"

//----------------------------------Sleep state----------------------------------
void __attribute__ ((naked)) __attribute__ ((noinline)) sleep_state()
{
  asm ("sleep_forever: b sleep_forever");	// sleep, "asm" prevents loop elimination
}

//----------------dispatcher-----------
void __attribute__ ((naked)) 
dispatcher ()
{
   
    // if(queue==0){
    //     sleep_state();
    // }
    // else
    // {
        USR_MODE;
        int a=2;
    *LEDS_P = a;
   // }
}
//---------------SW exception handler, not used-------------------
void __attribute__ ((naked)) 
swi ()
{
  SVC_MODE;
  //(*LEDS_P)++; // the same as *LEDS_P=*LEDS_P+1
  IRQ_ENABLE;
  dispatcher();
  sleep_state();
}
//----------------Tasks---------------
struct task_inst
{
  void (*func) ();	// function pointer
  int run_time,release_time,period,deadline;	// parameters, can be separated for protection
	
};

void task (struct task_inst *);
struct task_inst task_list[3] = {
  {task, 3, 0, 20, 500},
  {task, 3, 1, 20, 550}
  
};



void __attribute__ ((naked)) task (struct task_inst *d)
{
    
    //int queue[3]={0,1,2};
    int a=2;
    *LEDS_P = a;
    asm("swi 0");
}
//-----------------------IRQ handler, toggle LEDs------------------------------
void __attribute__ ((naked)) irq ()	// update block
{

// Timer support
  *INT_RAW_P = 0;
  *TIMER_COMPARE_P = *TIMER_P + 1; //1ms incremented 
  SVC_MODE;
  IRQ_ENABLE;


  dispatcher();
	
}

//---------------------------Start and initialise--------------------------------
int main ()
{
//   int q[3];
//   q[0]=0;
  *LEDS_P = 0;
  *TIMER_COMPARE_P = *TIMER_P + 1;
  *INT_ENABLE_P |= INT_MASK_TIMER;
  IRQ_ENABLE;
  sleep_state();
}
