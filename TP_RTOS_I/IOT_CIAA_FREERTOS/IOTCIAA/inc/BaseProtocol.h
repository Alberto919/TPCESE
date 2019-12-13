/*
 * BaseProtocol.h
 *
 *  Created on: 4 dic. 2019
 *      Author: Nino
 */

#ifndef BASEPROTOCOL_H_
#define BASEPROTOCOL_H_

#define START_CMD       0xF0
#define END_CMD 		0xFF

/* Result codes */
enum Result {
	FAIL = 0xF7,
	OK   = 0xF8,
	READY = 0x00,
	LED_1_OFF = 0x01,
	LED_1_ON = 0x02,
	LED_2_OFF = 0x03,
	LED_2_ON = 0x04,
	LED_3_OFF = 0x05,
	LED_3_ON = 0x06,
	LED_4_OFF = 0x07,
	LED_4_ON = 0x08,
};
#endif /* BASEPROTOCOL_H_ */

