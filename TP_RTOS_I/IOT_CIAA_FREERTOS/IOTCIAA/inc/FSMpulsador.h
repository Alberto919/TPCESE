/*
 * FSMpulsador.h
 *
 *  Created on: 7 dic. 2019
 *      Author: Alberto
 */

#ifndef MY_PROGRAMS_WEBCIAA_FSMPULSADOR_H_
#define MY_PROGRAMS_WEBCIAA_FSMPULSADOR_H_

#include "Pulsador.h"
#include "sapi.h"

class FSMpulsador {
public:
	FSMpulsador();
	void fsmButtonError( sPulsador* config );
	void fsmButtonInit( sPulsador* config );
	void fsmButtonUpdate( sPulsador* config );
	void buttonPressed( sPulsador* config );
	void buttonReleased( sPulsador* config );
	void buttonPressed_callback(void (*funcionEjecutar)(gpioMap_t));
	void buttonReleased_callback(void (*funcionEjecutar)(gpioMap_t));
private:
	void (*buttonPressed_funcion)(gpioMap_t);
	void (*buttonReleased_funcion)(gpioMap_t);
};

#endif /* MY_PROGRAMS_WEBCIAA_FSMPULSADOR_H_ */
