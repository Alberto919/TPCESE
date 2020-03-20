/*
 * E2RTOSII.cpp
 *
 *  Created on: 20 mar. 2020
 *      Author: alberto
 */

/*=============================================================================
 * Author: Yapura Alberto <yapura.alberto313@gmail.com>
 * Date: 2020/03/20
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "FreeRTOS.h"
#include "sapi.h"
#include "sapi_stdio.h"
#include "LedPulsador.h"
#include "GestorMensaje.h"
#include "task.h"

/*==================[definiciones de datos internos]=========================*/

#define UART_PC       UART_USB

//Instancio un objeto con la funcionalidad del gestor de mensajes
GestorMensaje* gestorMensaje = new GestorMensaje();

//Instancio un objeto con la funcionalidad del Led y los pulsadores
LedPulsador* ledPulsador = new LedPulsador(gestorMensaje);

//Instancio un objeto con la funcionalidad del Led y los pulsadores para la tarea_D
GestorMensaje* gestorMensaje_2 = new GestorMensaje();


/*==================[Prototipo de funciones internas]=========================*/

void tarea_A(void* taskParmPtr);
void tarea_B(void* taskParmPtr);
void tarea_C(void* taskParmPtr);
void tarea_D(void* taskParmPtr);

/*==================[definiciones de funciones internas]=====================*/
// Envia a la cola el texto LED_ON y enciende un led
void tarea_A(void* taskParmPtr) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	// ---------- REPETIR POR SIEMPRE --------------------------
	while (TRUE) {
		ledPulsador->procesarLed();
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
	}
}

// Debounce de cada tecla instanciada (en este caso 2 pueden ser hasta cuatro) y envío de mensaje lo hace dentro de máquina de estados
void tarea_B(void* taskParmPtr) {

	while (TRUE) {
		ledPulsador->procesarButton();
		vTaskDelay(1 / portTICK_RATE_MS);
	}
}

// Recibe los mensajes, los imprime, y envia la instancia (gestorMensaje_2) del gestor de mensajes que obtiene la tarea_D
void tarea_C(void* taskParmPtr) {
	while (TRUE) {
		gestorMensaje->mostrar(gestorMensaje_2);
	}
}

// Recibe los mensajes y libera la memoria alocada
void tarea_D(void* taskParmPtr) {
	while (TRUE) {
		gestorMensaje_2->descartar(); // @suppress("Method cannot be resolved")
	}
}

/*==================[funcion principal]======================================*/
// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main(void) {

	boardConfig();

	uartConfig( UART_PC, 115200);

	stdioPrintf(UART_PC, "Inicio");

	if ((gestorMensaje->init() == true) &&(gestorMensaje_2->init() == true) ){
		if (ledPulsador->init() == true) {

			xTaskCreate(
				tarea_A,                     	// Funcion de la tarea a ejecutar
				( const char * )"tarea_A",   	// Nombre de la tarea como String amigable para el usuario
				configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
				NULL,        					// Parametros de tarea
				tskIDLE_PRIORITY+1,         	// Prioridad de la tarea
				0                           	// Puntero a la tarea creada en el sistema
			);

			xTaskCreate(
				tarea_B,                     	// Funcion de la tarea a ejecutar
				( const char * )"tarea_B",   	// Nombre de la tarea como String amigable para el usuario
				configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
				NULL,        					// Parametros de tarea
				tskIDLE_PRIORITY+1,         	// Prioridad de la tarea
				0                           	// Puntero a la tarea creada en el sistema
			);

			xTaskCreate(
				tarea_C,                     	// Funcion de la tarea a ejecutar
				( const char * )"tarea_C",   	// Nombre de la tarea como String amigable para el usuario
				configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
				NULL,        					// Parametros de tarea
				tskIDLE_PRIORITY+2,         	// Prioridad de la tarea
				0                           	// Puntero a la tarea creada en el sistema
			);

			xTaskCreate(
				tarea_D,                     	// Funcion de la tarea a ejecutar
				( const char * )"tarea_D",   	// Nombre de la tarea como String amigable para el usuario
				configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
				NULL,        					// Parametros de tarea
				tskIDLE_PRIORITY+2,         	// Prioridad de la tarea
				0                           	// Puntero a la tarea creada en el sistema
			);
		}
	}


	 vTaskStartScheduler();

	// ---------- REPETIR POR SIEMPRE --------------------------
	while ( TRUE) {

	}

	// NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
	// directamenteno sobre un microcontroladore y no es llamado por ningun
	// Sistema Operativo, como en el caso de un programa para PC.
	return 0;
}

