/*
 * LedPulsador.h
 *
 *  Created on: 20 mar. 2020
 *      Author: alberto
 */

#ifndef MY_PROGRAMS_CESE_TP_ALBERTO_YAPURA_INC_LEDPULSADOR_H_
#define MY_PROGRAMS_CESE_TP_ALBERTO_YAPURA_INC_LEDPULSADOR_H_

#include "FSMpulsador.h"
#include "sapi.h"

#define BOTONES 2

class LedPulsador {
	const uint8_t size = BOTONES;
	FSMpulsador *pulsadores[BOTONES];
	GestorMensaje* gestorMensaje;
	uint8_t led = OFF;
public:
	LedPulsador(GestorMensaje *gm);
	bool init();
	void procesarLed();
	void procesarButton();
};

#endif /* MY_PROGRAMS_CESE_TP_ALBERTO_YAPURA_INC_LEDPULSADOR_H_ */
