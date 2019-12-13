/*
 * FSMprotocol.h
 *
 *  Created on: 4 dic. 2019
 *      Author: Alberto
 */

#ifndef FSMPROTOCOL_H_
#define FSMPROTOCOL_H_

#include "BaseProtocol.h"

#define BUFFER_LENGHT 3

class FSMprotocol {
public:
	unsigned char cmd_byte;
	unsigned char opr_byte;
	FSMprotocol();
	void loop();
	void function_callback(void (*funcionEjecutar)(int));
	void sendOK();
	void send(int input);
	void setSerial(void *uart_device);
private:
	int buf[BUFFER_LENGHT];
	bool exec_cmd(unsigned int cmd);
	void sendFail();	
	void receive (int input);	
	void (*funcion)(int);
	// internal FSM
	enum State {
		WAIT, DATA, END
	} state;

};

#endif /* FSMPROTOCOL_H_ */
