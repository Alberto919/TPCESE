/*
 * Tarea.h
 *
 *  Created on: 20 mar. 2020
 *      Author: alberto
 */

#ifndef MY_PROGRAMS_E2_RTOS_II_INC_TAREA_H_
#define MY_PROGRAMS_E2_RTOS_II_INC_TAREA_H_

#include "GestorMensaje.h"
#include "FreeRTOS.h"
#include "sapi.h"
#include "task.h"
#include "LedPulsador.h"

class Tarea {
public:
	Tarea();
	void tarea_A(void* taskParmPtr);

	void tarea_B(void* taskParmPtr);

	void tarea_C(void* taskParmPtr);

	void tarea_D(void* taskParmPtr);
};

#endif /* MY_PROGRAMS_E2_RTOS_II_INC_TAREA_H_ */
