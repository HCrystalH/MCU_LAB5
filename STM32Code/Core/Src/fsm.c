/*
 * fsm.c
 *
 *  Created on: Nov 18, 2023
 *      Author: Admin
 */

#include "fsm.h"

//#include "main.h"
//#include "stdio.h"
//#include "softwaretimer.h"

#define MAX_BUFFER_SIZE  30
// Buffer to store received data
ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;

char response[20];
uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;



int command_flag = 0;
uint32_t command_data = 0;
uint32_t ADC_Value =0;

uint32_t previous=0; 	// to store the previous value when user type OK
int state = START;

void command_parser_fsm(){
	switch(state){
		case START:
			if(buffer[index_buffer - 1] == START_STRING) state = FIRST_CMD;
			break;
		case FIRST_CMD:
			if(buffer[index_buffer - 1] == 'R'){
				state = GOT_S;
				HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
			}
			else if(buffer[index_buffer - 1] == 'O'){
				state = GOT_K;
			}
			else state = START;
			break;
		case GOT_S:
			if(buffer[index_buffer - 1] == 'S'){
				state = GOT_T;
			}
			else state = START;
			break;
		case GOT_T:
			if(buffer[index_buffer - 1] == 'T'){
				state = SEND;
			}
			else state = START;
			break;
		case GOT_K:
			if(buffer[index_buffer - 1] == 'K'){
				state = HALT;
			}
			else state = START;
			break;
		case SEND:
			if(buffer[index_buffer - 1] == END_STRING){
				command_flag = SENDING;
//				setTimer1(100);
				state = START;
			}
			else state = START;
			break;
		case HALT:
			// !OK#
			if(buffer[index_buffer - 1] == END_STRING){
				command_flag = STOP_SEND;
				setTimer1(300);
				state = START;
			}
			break;

		default:
			state = START;
			break;
		}
}
void uart_communication_fsm(){
	switch(command_flag){
	case SENDING:
		if(timerflag2 == 1){
			previous = ADC_Value;
			HAL_UART_Transmit(&huart2, (void *)response, sprintf(response, "!ADC= %ld# \r\n", ADC_Value), 1000);
			HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_RED_Pin);
			setTimer2(100);
//			timerflag2 = 0;
		}
		break;
	case STOP_SEND:
		if(timerflag1 == 1){
			HAL_UART_Transmit(&huart2, (void *)response, sprintf(response, "!OK_Value= %ld# \r\n", previous), 1000);
			HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_RED_Pin);
			command_flag = IDLE;
//			timerflag1 = 0;
		}
		break;
	default:
		break;
	}
}
