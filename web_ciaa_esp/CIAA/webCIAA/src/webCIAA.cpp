/*=============================================================================
 * Author: Yapura Alberto <yapura.alberto313@gmail.com>
 * Date: 2019/12/04
 * Version: 1.0
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "sapi.h"
#include "task.h"
#include "semphr.h"
#include "Pulsador.h"
#include "FSMpulsador.h"
#include "FSMprotocol.h"
#include "sapi_stdio.h"

#ifndef ON
#define ON     1
#endif
#ifndef OFF
#define OFF    0
#endif

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

#define UART_PROTOCOL UART_232
#define UART_PC       UART_USB

sPulsador pulsador_config[CANT];
FSMprotocol* fsmProtocol = new FSMprotocol();
FSMpulsador* fsmPulsador = new FSMpulsador();

/*==================[definiciones de datos externos]=========================*/

/*==================[Prototipo de funciones internas]=========================*/

void tarea_led(void* taskParmPtr);
void tarea_pulsador(void* taskParmPtr);

/*==================[definiciones de funciones internas]=====================*/

void tarea_pulsador(void* taskParmPtr) {
	sPulsador* config = (sPulsador*) taskParmPtr;

	fsmPulsador->fsmButtonInit(config);

	while (1) {
		fsmPulsador->fsmButtonUpdate(config);
	}
}

// Implementacion de funcion de la tarea
void tarea_led(void* taskParmPtr) {

	// ---------- REPETIR POR SIEMPRE --------------------------
	while ( TRUE) {
		fsmProtocol->loop();
	}
}
/*==================[declaraciones de funciones internas]====================*/

void event_led(int cmd) {
	switch (cmd) {
	case LED_1_OFF:
		gpioWrite(LEDB, OFF);
		break;
	case LED_1_ON:
		gpioWrite(LEDB, ON);
		break;
	case LED_2_OFF:
		gpioWrite(LED1, OFF);
		break;
	case LED_2_ON:
		gpioWrite(LED1, ON);
		break;
	case LED_3_OFF:
		gpioWrite(LED2, OFF);
		break;
	case LED_3_ON:
		gpioWrite(LED2, ON);
		break;
	case LED_4_OFF:
		gpioWrite(LED3, OFF);
		break;
	case LED_4_ON:
		gpioWrite(LED3, ON);
		break;
	}
	stdioPrintf(UART_PC, "%d\r\n", cmd);
}

void event_buttonPressed(gpioMap_t pulsador) {
	switch (pulsador) {
	case TEC1:
		fsmProtocol->send(LED_1_ON);
		gpioWrite(LEDB, ON);
		break;
	case TEC2:
		fsmProtocol->send(LED_2_ON);
		gpioWrite(LED1, ON);
		break;
	case TEC3:
		fsmProtocol->send(LED_3_ON);
		gpioWrite(LED2, ON);
		break;
	case TEC4:
		fsmProtocol->send(LED_4_ON);
		gpioWrite(LED3, ON);
		break;
	}
}

void event_buttonReleased(gpioMap_t pulsador) {
	switch (pulsador) {
	case TEC1:
		fsmProtocol->send(LED_1_OFF);
		gpioWrite(LEDB, OFF);
		break;
	case TEC2:
		fsmProtocol->send(LED_2_OFF);
		gpioWrite(LED1, OFF);
		break;
	case TEC3:
		fsmProtocol->send(LED_3_OFF);
		gpioWrite(LED2, OFF);
		break;
	case TEC4:
		fsmProtocol->send(LED_4_OFF);
		gpioWrite(LED3, OFF);
		break;
	}
}
/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/
// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main(void) {
	// ----- Setup -----------------------------------

	// Inicializar la placa
	boardConfig();

	uartConfig(UART_PROTOCOL, 115200);
	uartConfig( UART_PC, 115200);

	pulsador_config[0].tecla = TEC1;
	pulsador_config[1].tecla = TEC2;
	pulsador_config[2].tecla = TEC3;
	pulsador_config[3].tecla = TEC4;

	fsmProtocol->function_callback(&event_led);
	fsmPulsador->buttonPressed_callback(&event_buttonPressed);
	fsmPulsador->buttonReleased_callback(&event_buttonReleased);

	xTaskCreate(tarea_led,                     // Funcion de la tarea a ejecutar
			(const char *) "tarea_led", // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE * 2, 	// Cantidad de stack de la tarea
			NULL,        	// Parametros de tarea
			tskIDLE_PRIORITY + 1,         	// Prioridad de la tarea
			0                         // Puntero a la tarea creada en el sistema
			);

	xTaskCreate(tarea_pulsador,                // Funcion de la tarea a ejecutar
			(const char *) "tarea_tecla1", // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE * 2, 	// Cantidad de stack de la tarea
			&pulsador_config[0],           // Parametros de tarea
			tskIDLE_PRIORITY + 1,         	// Prioridad de la tarea
			0                         // Puntero a la tarea creada en el sistema
			);

	xTaskCreate(tarea_pulsador,                // Funcion de la tarea a ejecutar
			(const char *) "tarea_tecla2", // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE * 2, 	 // Cantidad de stack de la tarea
			&pulsador_config[1],            // Parametros de tarea
			tskIDLE_PRIORITY + 1,         	// Prioridad de la tarea
			0                         // Puntero a la tarea creada en el sistema
			);
	xTaskCreate(tarea_pulsador,                // Funcion de la tarea a ejecutar
			(const char *) "tarea_tecla3", // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE * 2, 	// Cantidad de stack de la tarea
			&pulsador_config[2],           // Parametros de tarea
			tskIDLE_PRIORITY + 1,         	// Prioridad de la tarea
			0                         // Puntero a la tarea creada en el sistema
			);

	xTaskCreate(tarea_pulsador,                // Funcion de la tarea a ejecutar
			(const char *) "tarea_tecla4", // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE * 2, 	 // Cantidad de stack de la tarea
			&pulsador_config[3],            // Parametros de tarea
			tskIDLE_PRIORITY + 1,         	// Prioridad de la tarea
			0                         // Puntero a la tarea creada en el sistema
			);

	// Iniciar scheduler
	vTaskStartScheduler();

	// ----- Repeat for ever -------------------------
	while (true) {

	}

	// YOU NEVER REACH HERE, because this program runs directly or on a
	// microcontroller and is not called by any Operating System, as in the
	// case of a PC program.
	return 0;
}

/*==================[fin del archivo]========================================*/
