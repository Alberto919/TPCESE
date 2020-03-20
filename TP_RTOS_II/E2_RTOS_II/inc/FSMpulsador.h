/*
 * FSMpulsador.h
 *
 *  Created on: 20 mar. 2020
 *      Author: alberto
 */

#ifndef MY_PROGRAMS_E2_RTOS_II_INC_FSMPULSADOR_H_
#define MY_PROGRAMS_E2_RTOS_II_INC_FSMPULSADOR_H_

#include "FreeRTOS.h"
#include "sapi.h"
#include "task.h"
#include "GestorMensaje.h"

typedef enum {
	STATE_BUTTON_UP,
	STATE_BUTTON_DOWN,
	STATE_BUTTON_FALLING,
	STATE_BUTTON_RISING
} fsmButtonState_t;


class FSMpulsador {
private:
	gpioMap_t tecla;
	fsmButtonState_t fsmButtonState;
	uint8_t contFalling;
	uint8_t contRising;
	TickType_t tiempo_medido;
	TickType_t tiempo_down;
	TickType_t tiempo_up;
	const int DEBOUNCE_TIME = 20;
	GestorMensaje *gestorMensaje;
	void enviarMensaje();
public:
	FSMpulsador(GestorMensaje *gm, gpioMap_t pTecla);
	void fsmButtonError();
	void fsmButtonInit();
	void fsmButtonUpdate();
	void buttonPressed();
	void buttonReleased();
};

#endif /* MY_PROGRAMS_E2_RTOS_II_INC_FSMPULSADOR_H_ */
