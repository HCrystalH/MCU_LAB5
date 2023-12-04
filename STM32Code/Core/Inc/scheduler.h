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

extern
void SCH_Init(void);

// SetTimer
uint32_t SCH_Add_Task( void (*pFunction) (), uint32_t DELAY, uint32_t PERIOD);
//TimerRun
void SCH_Update(void);

// Check Timer Flag
void SCH_Dispatch_Tasks(void);

unsigned char SCH_Delete_Task(uint32_t taskID);

/*** 8 bit error code ***/
#define RETURN_NORMAL										0
#define RETURN_ERROR										1
#define ERROR_SCH_TOO_MANY_TASKS							1
#define ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK					2
#define ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER		3
#define ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START			4
#define ERROR_SCH_LOST_SLAVE								5
#define ERRROR_SCH_CAN_BUS_ERROR							6
#define ERROR_I2C_WRITE_BYTE_AT24C64						7
#define ERROR_SCH_CANNOT_DELETE_TASK						8
#endif /* INC_SCHEDULER_H_ */
