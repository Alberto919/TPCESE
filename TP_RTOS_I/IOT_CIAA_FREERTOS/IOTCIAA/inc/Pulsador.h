/*
 * OPulsador.h
 *
 *  Created on: 7 dic. 2019
 *      Author: Alberto
 */

#ifndef MY_PROGRAMS_WEBCIAA_INC_PULSADOR_H_
#define MY_PROGRAMS_WEBCIAA_INC_PULSADOR_H_

#include "sapi.h"

#define CANT	4


typedef enum
{
    STATE_BUTTON_UP,
    STATE_BUTTON_DOWN,
    STATE_BUTTON_FALLING,
    STATE_BUTTON_RISING
} fsmButtonState_t;

typedef struct
{
	gpioMap_t tecla;			//config

	fsmButtonState_t fsmButtonState;//variables

	uint8_t contFalling  ;		//variables
	uint8_t contRising  ;		//variables


} sPulsador;



#endif /* MY_PROGRAMS_WEBCIAA_INC_PULSADOR_H_ */
