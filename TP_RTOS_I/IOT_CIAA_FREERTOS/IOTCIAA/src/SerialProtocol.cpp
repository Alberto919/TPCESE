/*
 * SerialProtocol.cpp
 *
 *  Created on: 4 dic. 2019
 *      Author: Alberto
 */

#include "SerialProtocol.h"
#include "sapi.h"
#define UART_PROTOCOL UART_232

SerialProtocol::SerialProtocol() {
	// TODO Auto-generated constructor stub
}

void SerialProtocol::write(int input) {
	uartWriteByte(UART_PROTOCOL, input);
}
int SerialProtocol::read() {
	uint8_t byteReceived;
	uartReadByte(UART_PROTOCOL, &byteReceived);
	return byteReceived;

}
int SerialProtocol::available() {
	return uartRxReady(UART_PROTOCOL);
}
