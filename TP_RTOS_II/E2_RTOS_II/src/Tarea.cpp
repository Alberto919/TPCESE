/*
 * Tarea.cpp
 *
 *  Created on: 20 mar. 2020
 *      Author: alberto
 */

#include "Tarea.h"


Tarea::Tarea() {
	// TODO Auto-generated constructor stub

}

void Tarea::tarea_A(void* taskParmPtr) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	LedPulsador *led = (LedPulsador *) taskParmPtr;

	// ---------- REPETIR POR SIEMPRE --------------------------
	while (TRUE) {

		led->procesarLed();
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
	}
}

// Debounce de cada tecla instanciada (en este caso 2) y envío de mensaje (lo hace dentro de máquina de estados)
void Tarea::tarea_B(void* taskParmPtr) {
	LedPulsador *pulsador = (LedPulsador *) taskParmPtr;

	while (TRUE) {
		pulsador->procesarButton();
		vTaskDelay(1 / portTICK_RATE_MS);
	}
}

// Recibe los mensajes, los imprime, y libera la memoria alocada
void Tarea::tarea_C(void* taskParmPtr) {
	GestorMensaje *mensajes = (GestorMensaje *) taskParmPtr;
	while (TRUE) {
		mensajes->mostrar();
	}
}

// Recibe los mensajes, los imprime, y libera la memoria alocada
void Tarea::tarea_D(void* taskParmPtr) {
	GestorMensaje *mensajes = (GestorMensaje *) taskParmPtr;
	while (TRUE) {
		mensajes->descartar();
	}
}
