/*
 * SerialProtocol.h
 *
 *  Created on: 4 dic. 2019
 *      Author: Alberto
 */

#ifndef SERIALPROTOCOL_H_
#define SERIALPROTOCOL_H_

#include "SerialProtocol.h"
#include "Arduino.h"

class SerialProtocol {
public:
	SerialProtocol();
	void write(int input);
	int read();
	int available();
};

#endif /* SERIALPROTOCOL_H_ */
