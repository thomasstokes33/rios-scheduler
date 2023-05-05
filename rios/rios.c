#include "rios.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/sync.h"
#define MAX_TASKS 20

typedef struct task {  //struc name
	uint8_t running; //1 shows running
	int state; //state machine
	uint32_t period; //how often to tick
	uint32_t elapsedTime; //time since previous tick
	int (*TickFct) (int); //pointer to the function to call
	
} task; //type name


task tasks[MAX_TASKS];
int8_t tasksNum=-1;

uint32_t tasksPeriodGCD = 0;  /* Timer tick rate */



uint8_t runningTasks[MAX_TASKS+1] = {255}; /* Track running tasks, [0] always idleTask */
const uint32_t idleTask = 255;             /* 0 highest priority, 255 lowest */
uint8_t currentTask = 0;                   /* Index of highest priority task in runningTasks */

unsigned schedule_time = 0;

bool ISR(struct repeating_timer *t) {
	uint8_t i;
	for (i=0; i<=tasksNum;i++) {
		if (tasks[i].elapsedTime>tasks[i].period && i< runningTasks[currentTask] && !tasks[i].running){ //&&currenttask<MAX_TASKS
				uint32_t flag = save_and_disable_interrupts();
				
				tasks[i].elapsedTime = 0;
				tasks[i].running = 1;
				runningTasks[++currentTask] = i;
				
				restore_interrupts(flag);
				
				tasks[i].state = tasks[i].TickFct(tasks[i].state); 
				
				flag = save_and_disable_interrupts();
				tasks[i].running=0;
				runningTasks[currentTask--]=idleTask;
				
				restore_interrupts(flag);
		}
		
		tasks[i].elapsedTime+=tasksPeriodGCD;
	}
}

void add_task(int (*TickFct)(int), uint32_t period) {
	tasks[++tasksNum].running=0;
	tasks[tasksNum].state=-1;
	tasks[tasksNum].elapsedTime=0;
	tasks[tasksNum].period=period;
	tasks[tasksNum].TickFct=TickFct;
	
	
}

void start_rios(int32_t tickRate) {
	
	tasksPeriodGCD = tickRate;
	struct repeating_timer timer;
	add_repeating_timer_ms(tickRate,ISR,NULL,&timer);
	while(1) {}
}
