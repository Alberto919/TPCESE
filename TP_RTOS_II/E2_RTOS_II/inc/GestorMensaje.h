/*
 * GestorMensaje.h
 *
 *  Created on: 20 mar. 2020
 *      Author: alberto
 */

#ifndef MY_PROGRAMS_E2_RTOS_II_INC_GESTORMENSAJE_H_
#define MY_PROGRAMS_E2_RTOS_II_INC_GESTORMENSAJE_H_

#include "FreeRTOS.h"
#include "sapi.h"
#include "queue.h"
#include "sapi_stdio.h"
#include "task.h"

#define UART_PC  UART_USB

class GestorMensaje {
	QueueHandle_t xQueue;
	BaseType_t xStatus;
public:
	GestorMensaje();
	bool init();
	bool enviar(const char *msj);
	bool mostrar();
	bool mostrar(GestorMensaje *gestor);
	void descartar();
};

#endif /* MY_PROGRAMS_E2_RTOS_II_INC_GESTORMENSAJE_H_ */
