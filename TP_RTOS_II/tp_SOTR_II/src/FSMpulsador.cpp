/*
 * FSMpulsador.cpp
 *
 *  Created on: 12 Mar. 2020
 *      Author: Alberto
 */

#include "FSMpulsador.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#define DEBOUNCE_TIME 20

FSMpulsador::FSMpulsador() {
	// TODO Auto-generated constructor stub

}

void FSMpulsador::fsmButtonInit(sPulsador* config) {
	config->contFalling = 0;
	config->contRising = 0;
	config->fsmButtonState = STATE_BUTTON_UP; // estado inicial
}

// Actualiza el estado de la maquina de estados
void FSMpulsador::fsmButtonUpdate(sPulsador* config) {

	switch (config->fsmButtonState) {
	case STATE_BUTTON_UP:
		/* COMPRUEBA LAS CONDICIONES DE TRANSICIÓN */
		if (!gpioRead(config->tecla)) {
			config->fsmButtonState = STATE_BUTTON_FALLING;
		}
		break;
	case STATE_BUTTON_FALLING:

		/* COMPRUEBA LAS CONDICIONES DE TRANSICIÓN */
		if (config->contFalling >= DEBOUNCE_TIME) {
			if (!gpioRead(config->tecla)) {
				config->fsmButtonState = STATE_BUTTON_DOWN;

				/* ACCION DEL EVENTO*/
				buttonPressed(config);
			} else {
				config->fsmButtonState = STATE_BUTTON_UP;
			}

			config->contFalling = 0;
		}
		config->contFalling++;
		break;
	case STATE_BUTTON_DOWN:
		/* COMPRUEBA LAS CONDICIONES DE TRANSICIÓN */
		if (gpioRead(config->tecla)) {
			config->fsmButtonState = STATE_BUTTON_RISING;
		}
		break;
	case STATE_BUTTON_RISING:

		/* COMPRUEBA LAS CONDICIONES DE TRANSICIÓN */

		if (config->contRising >= DEBOUNCE_TIME) {
			if (gpioRead(config->tecla)) {
				config->fsmButtonState = STATE_BUTTON_UP;

				/* ACCION DEL EVENTO ! */
				buttonReleased(config);
			} else {
				config->fsmButtonState = STATE_BUTTON_DOWN;
			}
			config->contRising = 0;
		}
		config->contRising++;
		break;
	default:
		break;
	}
}

/*
 * Evento de tecla pulsada
 */
void FSMpulsador::buttonPressed(sPulsador* config) {
	config->tiempo_down = xTaskGetTickCount();
}

/*
 * Evento de tecla liberada
 */
void FSMpulsador::buttonReleased(sPulsador* config) {
	config->tiempo_up = xTaskGetTickCount();

	TickType_t dif = config->tiempo_up - config->tiempo_down;

	//abrir seccion critica
	xSemaphoreTake(config->mutex, portMAX_DELAY);

	config->tiempo_medido = dif;

	//cerrar seccion critica
	xSemaphoreGive(config->mutex);

	/* señalizo al semaforo para que la tarea del led se ponga en ready */
	xSemaphoreGive(config->tecla_pulsada_sem);
}

