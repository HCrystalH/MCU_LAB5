/*
 * scheduler.c
 *
 *  Created on: Dec 2, 2023
 *      Author: Admin
 */


#include "scheduler.h"

// Should be declare here to avoid user can access and change data structure in main.c
sTasks SCH_tasks_G[SCH_MAX_TASKS];
uint32_t current_index_task =0;

void SCH_Init(void){
	for(unsigned char i=0;i<SCH_MAX_TASKS; i++){
		SCH_Delete_Task(i);
	}
}

//
//void SCH_Update(void){
//	for(int i = 0; i < current_index_task; i++){
//		if (SCH_tasks_G[i].Delay > 0){
//			SCH_tasks_G[i].Delay --;
//		}else{
//			SCH_tasks_G[i].Delay = SCH_tasks_G[i].Period;
//			SCH_tasks_G[i].Runme += 1;
//		}
//	}
//}

uint32_t SCH_Add_Task(void (*pFunction) (), uint32_t DELAY, uint32_t PERIOD){
	uint32_t index = 0;

	while( (SCH_tasks_G[index].pTask != 0) && (index < SCH_MAX_TASKS)){
		index++;
	}

	if(index == SCH_MAX_TASKS){
		return SCH_MAX_TASKS;
	}
	SCH_tasks_G[index].pTask = pFunction;
	SCH_tasks_G[index].Delay = DELAY;
	SCH_tasks_G[index].Period = PERIOD;
	SCH_tasks_G[index].Runme = 0;

	SCH_tasks_G[index].TaskID = current_index_task;
	current_index_task++;
	return index;
}


void SCH_Update(void){
	for(unsigned char i=0;i<SCH_MAX_TASKS;i++){
		// check if there is a task at this location
		if(SCH_tasks_G[i].pTask){

			if(SCH_tasks_G[i].Delay > 0){
				// Not ready to run, just decrease delay
				SCH_tasks_G[i].Delay--;
			}else{
				// delay == 0
				SCH_tasks_G[i].Runme += 1;
				if(SCH_tasks_G[i].Period){
					//Schedule periodic tasks to run again
					SCH_tasks_G[i].Delay = SCH_tasks_G[i].Period;
				}
			}
		}
	}
}


void SCH_Dispatch_Tasks(void){
	for(int index=0;index < SCH_MAX_TASKS;index++){
		if(SCH_tasks_G[index].Runme > 0){
			SCH_tasks_G[index].Runme--;		// Reset Runme flag
			(*SCH_tasks_G[index].pTask)();	// Run the task

			//If this is a 1-shot task, remove it from array
			if(SCH_tasks_G[index].Period == 0)	SCH_Delete_Task(index);
		}
	}
//	// Enter low-power mode (Sleep mode). The MCU will wake up on the next interrupt
//	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

unsigned char SCH_Delete_Task(uint32_t taskID){
	unsigned char Return_code;
	if(SCH_tasks_G[taskID].pTask == 0){
		// No task at this location
		// Set the golbal error variable
//		Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;

		// also return an error code
		Return_code = RETURN_ERROR;
	}else	Return_code = RETURN_NORMAL;

	SCH_tasks_G[taskID].pTask = 0x0000;
	SCH_tasks_G[taskID].Delay = 0;
	SCH_tasks_G[taskID].Period = 0;
	SCH_tasks_G[taskID].Runme = 0;

	return Return_code;
}
