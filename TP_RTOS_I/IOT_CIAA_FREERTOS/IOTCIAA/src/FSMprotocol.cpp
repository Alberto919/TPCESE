/*
 * FSMprotocol.cpp
 *
 *  Created on: 4 dic. 2019
 *      Author: Alberto
 */

#include "FSMprotocol.h"
#include "SerialProtocol.h"
#include "sapi.h"

FSMprotocol::FSMprotocol() {
	// TODO Auto-generated constructor stub
	state = WAIT;
}

/*
 *	Update Maquina de estados Se ejecuta mientras reciba
 *	datos desde otro dispositivo a traves de la UART
 */
void FSMprotocol::loop() {
	while (serialProtocol.available() > 0) {
		receive(serialProtocol.read());
	}
}

/*
 *	Maquina de estados del protocolo
 */
void FSMprotocol::receive(int input) {
	switch (state) {
	case WAIT:
		if (input == START_CMD) {
			cmd_byte = input; // store valid command
			state = DATA;
		}
		break;
	case DATA:
		if (input > -1) {
			opr_byte = input; // store valid command
			state = END;
		} else {
			state = WAIT;
		}
		break;
	case END:
		if (input == END_CMD) {
			if (exec_cmd(opr_byte)) {
				sendOK();
			} else {
				sendFail();
			}
			state = WAIT;
		} else {
			state = WAIT;
		}
		break;
	}
}

/*
 *	Se ejecuta si los datos corresponden al formato
 *	admitido por este protocolo
 */
bool FSMprotocol::exec_cmd(unsigned int cmd) {
	bool check = true;
	switch (cmd) {
	case READY:
		funcion(READY);
		break;
	case LED_1_OFF:
		funcion(LED_1_OFF);
		break;
	case LED_1_ON:
		funcion(LED_1_ON);
		break;
	case LED_2_OFF:
		funcion(LED_2_OFF);
		break;
	case LED_2_ON:
		funcion(LED_2_ON);
		break;
	case LED_3_OFF:
		funcion(LED_3_OFF);
		break;
	case LED_3_ON:
		funcion(LED_3_ON);
		break;
	case LED_4_OFF:
		funcion(LED_4_OFF);
		break;
	case LED_4_ON:
		// ENCENDER LED
		funcion(LED_4_ON);
		break;
	default:
		check = false;
		break;
	}
	return check;
}

/*
 *  Si el formato del mensaje no es correcto
 *	envia al dispositivo que envio un mensaje
 *	de error
 */
void FSMprotocol::sendFail() {
	send(FAIL);
}

/*
 *  Si el formato del mensaje es correcto
 *	envia al dispositivo que envio un mensaje
 *	de aceptacion
 */
void FSMprotocol::sendOK() {
	send(OK);
}

/*
 *  Permite enviar un mensaje utilizando
 *	las instrucciones propias de este protocolo
 *	a otro dispositivo en donde se ejecute este
 *	protocolo
 */
void FSMprotocol::send(int input) {
	int written = 0;
	buf[0] = START_CMD;
	buf[1] = input;
	buf[2] = END_CMD;
	while (written < BUFFER_LENGHT) {
		serialProtocol.write(buf[written]);
		written++;
	}
}

/*
 *  Vincula la funcion callback
 *  para ejecutar el evento
 */
void FSMprotocol::function_callback(void (*funcionEjecutar)(int)) {
	funcion = funcionEjecutar;
}
