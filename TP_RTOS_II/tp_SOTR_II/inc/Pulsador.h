/*
 * OPulsador.h
 *
 *  Created on: 12 Mar. 2020
 *      Author: Alberto
 */

#ifndef MY_PROGRAMS_TP_SOTR_II_INC_PULSADOR_H_
#define MY_PROGRAMS_TP_SOTR_II_INC_PULSADOR_H_

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "sapi.h"
#include "semphr.h"

#define CANT	2

typedef enum {
	STATE_BUTTON_UP,
	STATE_BUTTON_DOWN,
	STATE_BUTTON_FALLING,
	STATE_BUTTON_RISING
} fsmButtonState_t;

typedef struct {
	gpioMap_t tecla;			//config

	fsmButtonState_t fsmButtonState;			//variables

	uint8_t contFalling;		//variables
	uint8_t contRising;			//variables

	TickType_t tiempo_medido;	//variables
	TickType_t tiempo_down;		//variables
	TickType_t tiempo_up;		//variables


	SemaphoreHandle_t mutex;
	SemaphoreHandle_t tecla_pulsada_sem;

} sPulsador;

#endif /* MY_PROGRAMS_WEBCIAA_INC_PULSADOR_H_ */
