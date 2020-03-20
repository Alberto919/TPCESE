/*
 * FSMpulsador.cpp
 *
 *  Created on: 20 mar. 2020
 *      Author: alberto
 */

#include "FSMpulsador.h"

// Constructor - parametros del gestor de mensajes y la tecla
FSMpulsador::FSMpulsador(GestorMensaje *gm, gpioMap_t pTecla){
	// TODO Auto-generated constructor stub
	tecla = pTecla;
	gestorMensaje = gm;
}

// Constructor - inicializa cada objeto pulsador
void FSMpulsador::fsmButtonInit() {
	contFalling = 0;
	contRising = 0;
	fsmButtonState = STATE_BUTTON_UP; // estado inicial
}

// Actualiza el estado de la maquina de estados
void FSMpulsador::fsmButtonUpdate() {

	switch (fsmButtonState) {
	case STATE_BUTTON_UP:
		/* COMPRUEBA LAS CONDICIONES DE TRANSICI�N */
		if (!gpioRead(tecla)) {
			fsmButtonState = STATE_BUTTON_FALLING;
		}
		break;
	case STATE_BUTTON_FALLING:

		/* COMPRUEBA LAS CONDICIONES DE TRANSICI�N */
		if (contFalling >= DEBOUNCE_TIME) {
			if (!gpioRead(tecla)) {
				fsmButtonState = STATE_BUTTON_DOWN;

				/* ACCION DEL EVENTO*/
				buttonPressed();
			} else {
				fsmButtonState = STATE_BUTTON_UP;
			}

			contFalling = 0;
		}
		contFalling++;
		break;
	case STATE_BUTTON_DOWN:
		/* COMPRUEBA LAS CONDICIONES DE TRANSICI�N */
		if (gpioRead(tecla)) {
			fsmButtonState = STATE_BUTTON_RISING;
		}
		break;
	case STATE_BUTTON_RISING:

		/* COMPRUEBA LAS CONDICIONES DE TRANSICI�N */

		if (contRising >= DEBOUNCE_TIME) {
			if (gpioRead(tecla)) {
				fsmButtonState = STATE_BUTTON_UP;

				/* ACCION DEL EVENTO ! */
				buttonReleased();
			} else {
				fsmButtonState = STATE_BUTTON_DOWN;
			}
			contRising = 0;
		}
		contRising++;
		break;
	default:
		break;
	}
}

/*
 * Evento de tecla pulsada
 */
void FSMpulsador::buttonPressed() {
	tiempo_down = xTaskGetTickCount();
}

/*
 * Evento de tecla liberada
 */
void FSMpulsador::buttonReleased() {
	tiempo_up = xTaskGetTickCount();
	TickType_t dif = tiempo_up - tiempo_down;
	tiempo_medido = dif;
	enviarMensaje();
}

/*
 * Construyo el mensaje y lo envio
 * a la cola
 */
void FSMpulsador::enviarMensaje() {
	char msj[] = "TECx Tyyyy";
	sprintf(msj, "TECx T%04d", tiempo_medido);
	if (tecla == TEC1) {
		msj[3] = (const char) '1';
	} else if (tecla == TEC2) {
		msj[3] = (const char) '2';
	} else if (tecla == TEC3) {
		msj[3] = (const char) '3';
	} else {
		msj[3] = (const char) '4';
	}
	gestorMensaje->enviar(msj);
}
