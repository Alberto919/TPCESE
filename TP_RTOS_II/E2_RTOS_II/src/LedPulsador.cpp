/*
 * LedPulsador.cpp
 *
 *  Created on: 20 mar. 2020
 *      Author: alberto
 */

#include "LedPulsador.h"

// Constructor - parametros del gestor de mensajes
LedPulsador::LedPulsador(GestorMensaje *gm) {
	// TODO Auto-generated constructor stub
	gestorMensaje = gm;
}

// Inicializa el objeto de acuerdo al numero de botones a controlar y su maquina de estados
bool LedPulsador::init() {
	for (uint8_t var = 0; var < size; ++var) {
		switch (var) {
		case 0:
			pulsadores[var] = new FSMpulsador(gestorMensaje, TEC1);
			pulsadores[var]->fsmButtonInit();
			break;
		case 1:
			pulsadores[var] = new FSMpulsador(gestorMensaje, TEC2);
			pulsadores[var]->fsmButtonInit();
			break;
		case 2:
			pulsadores[var] = new FSMpulsador(gestorMensaje, TEC3);
			pulsadores[var]->fsmButtonInit();
			break;
		case 3:
			pulsadores[var] = new FSMpulsador(gestorMensaje, TEC4);
			pulsadores[var]->fsmButtonInit();
			break;
		default:
			return false;
		}
	}
	return true;
}

// Ejecuta la maquina de estados de cada boton
void LedPulsador::procesarButton() {
	for (uint8_t var = 0; var < size; ++var) {
		pulsadores[var]->fsmButtonUpdate();
	}
}

// Ejecuta la funcionalidad del led
void LedPulsador::procesarLed() {
	if (!led) {
		led = ON;
		gpioWrite(LEDB, led);
		gestorMensaje->enviar("LED ON");
	} else {
		led = OFF;
		gpioWrite(LEDB, led);
	}
}
