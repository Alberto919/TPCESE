/*
 * SerialProtocol.cpp
 *
 *  Created on: 4 dic. 2019
 *      Author: Alberto
 */

#include "SerialProtocol.h"

SerialProtocol::SerialProtocol() {
	// TODO Auto-generated constructor stub
}

void SerialProtocol::write(int input) {
	Serial.write(input);
}
int SerialProtocol::read() {
	return Serial.read();
}
int SerialProtocol::available() {
	return Serial.available();
}
