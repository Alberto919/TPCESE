/*
 * FSMpulsador.h
 *
 * Created on: 12 Mar. 2020
 *      Author: Alberto
 */

#ifndef MY_PROGRAMS_TP_SOTR_II_FSMPULSADOR_H_
#define MY_PROGRAMS_TP_SOTR_II_FSMPULSADOR_H_

#include "sapi.h"
#include "semphr.h"

class FSMpulsador {
public:
	FSMpulsador();
	void fsmButtonError( sPulsador* config );
	void fsmButtonInit( sPulsador* config );
	void fsmButtonUpdate( sPulsador* config );
	void buttonPressed( sPulsador* config );
	void buttonReleased( sPulsador* config );
};

#endif /* MY_PROGRAMS_WEBCIAA_FSMPULSADOR_H_ */
