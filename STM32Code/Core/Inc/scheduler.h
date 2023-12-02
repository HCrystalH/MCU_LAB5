/*
 * scheduler.h
 *
 *  Created on: Dec 2, 2023
 *      Author: Admin
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>
typedef struct{
	void (*pTask)(void);	// Pointer to task
	uint32_t Delay;			// setTimer(1000)
	uint32_t Period;		// setTimer(1000)
	uint8_t		Runme;		// Timer Flag

	uint32_t	TaskID;
}sTasks;

#define SCH_MAX_TASKS	40

void SCH_Init(void);

// SetTimer
void SCH_Add_Task( void (*pFunction) (), uint32_t DELAY, uint32_t PERIOD);
//TimerRun
void SCH_Update(void);

// Check Timer Flag
void SCH_Dispatch_Tasks(void);

void SCH_Delete(uint32_t ID);
#endif /* INC_SCHEDULER_H_ */
