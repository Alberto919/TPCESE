/*
 * GestorMensaje.cpp
 *
 *  Created on: 20 mar. 2020
 *      Author: alberto
 */

/*=============================================================================
 * Author: Yapura Alberto <yapura.alberto313@gmail.com>
 * Date: 2020/03/20
 *===========================================================================*/


#include "GestorMensaje.h"

GestorMensaje::GestorMensaje() {
	// TODO Auto-generated constructor stub
}

bool GestorMensaje::init() {
	bool status = true;
	xQueue = xQueueCreate(5, sizeof(char *));
	if (xQueue == NULL) {
		status = false;
	}
	return status;
}

bool GestorMensaje::enviar(const char *msj) {
	bool status = true;
	char *mensaje = (char *) pvPortMalloc(sizeof(msj));
	if (mensaje == NULL) {
		status = false;
	} else {
		stdioSprintf(mensaje, "%s", msj);
		xStatus = xQueueSend(xQueue, (void * )&mensaje, 0);
		if (xStatus != pdPASS) {
			status = false;
		}
	}
	return status;
}

bool GestorMensaje::mostrar(GestorMensaje *gestor) {
	bool status = true;
	char *mensaje;
	xStatus = xQueueReceive(xQueue, &mensaje, portMAX_DELAY);
	if (xStatus != pdPASS) {
		status = false;
	} else {
		stdioPrintf(UART_PC, "%s\r\n", (char *) mensaje); // @suppress("Invalid arguments")
		vPortFree(mensaje);
		status = gestor->enviar(mensaje);
	}
	return status;
}

bool GestorMensaje::mostrar() {
	bool status = true;
	char *mensaje;
	xStatus = xQueueReceive(xQueue, &mensaje, portMAX_DELAY);
	if (xStatus != pdPASS) {
		status = false;
	} else {
		stdioPrintf(UART_PC, "%s\r\n", (char *) mensaje); // @suppress("Invalid arguments")
		vPortFree(mensaje);
	}
	return status;
}

void GestorMensaje::descartar() {
	bool status = true;
	char *mensaje;
	xStatus = xQueueReceive(xQueue, &mensaje, portMAX_DELAY);
	if (xStatus == pdPASS) {
		//printf("mensaje %s\n", (char *) mensaje);
		vPortFree(mensaje);
	}
}

