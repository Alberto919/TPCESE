/*=============================================================================
 * Author: Yapura Alberto <yapura.alberto313@gmail.com>
 * Date: 2020/03/12
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "FreeRTOS.h"
#include "sapi.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "Pulsador.h"
#include "FSMpulsador.h"
#include "sapi_stdio.h"

/*==================[definiciones de datos internos]=========================*/

#define UART_PC       UART_USB


sPulsador pulsador_config[CANT];

// Creamos un objeto del tipo  fsmPulsador para obtener
// los eventos de las teclas
FSMpulsador* fsmPulsador = new FSMpulsador();

QueueHandle_t cola;


/*==================[Prototipo de funciones internas]=========================*/
void tarea_A(void* taskParmPtr);
void tarea_B(void* taskParmPtr);
void tarea_C(void* taskParmPtr);

uint8_t led = OFF;
const char * const pcLED_ON = "LED ON";
const char * const pcTECLA_1 = "TECLA 1 T";
const char * const pcTECLA_2 = "TECLA 2 T";

/*==================[definiciones de funciones internas]=====================*/

// Implementacion de funcion
// para la tarea que enciende
// el led de forma periodica
void tarea_A(void* taskParmPtr) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	char * message;

	while (1) {
		if( !led ){
			led = ON;
			gpioWrite(LEDB, led);
			message = (char *)pvPortMalloc(7);
			stdioSprintf(message, "%s",pcLED_ON);
			xQueueSend(cola, &message,0); // @suppress("Invalid arguments")
			vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
		}else{
			led = OFF;
			gpioWrite(LEDB, led);
		}

		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
	}
}

// Lectura con antirrebote
// de 2 teclas y medición de
// tiempo de pulsación
void tarea_B(void* taskParmPtr) {
	sPulsador* config = (sPulsador*) taskParmPtr;
	fsmPulsador->fsmButtonInit(config);
	char * message;

	while (1) {
		TickType_t dif;
		fsmPulsador->fsmButtonUpdate(config);
		dif = config->tiempo_medido;

		if (dif > 0){
			message = (char *)pvPortMalloc(10);
			if (config->tecla == TEC1){
				stdioSprintf(message, "%s%d",pcTECLA_1, dif);
			}else{
				stdioSprintf(message, "%s%d",pcTECLA_2, dif);
			}

			xQueueSend(cola, (void *) &message,0);
			config->tiempo_medido = 0;
			vTaskDelay(pdMS_TO_TICKS(500));
		}
	}
}

// Implementacion de funcion
// para la tarea que enciende
// el led de forma periodica
void tarea_C(void* taskParmPtr) {
	char * pReceivedString;

	while (1) {
		xQueueReceive(cola, &pReceivedString, portMAX_DELAY); // @suppress("Invalid arguments")
		stdioPrintf(UART_PC, "%s\r\n", (char *)pReceivedString); // @suppress("Invalid arguments")
		vPortFree(pReceivedString);
	}
}

/*==================[funcion principal]======================================*/
// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main(void) {

	boardConfig();

    uartConfig( UART_PC, 115200);

    stdioPrintf(UART_PC, "Inicio");

	cola = xQueueCreate(1, sizeof(char *));

	if(cola != NULL){

		pulsador_config[0].tecla 				= TEC1;
		pulsador_config[0].mutex 				= xSemaphoreCreateMutex();
		pulsador_config[0].tecla_pulsada_sem 	= xSemaphoreCreateBinary();

		pulsador_config[1].tecla 				= TEC2;
		pulsador_config[1].mutex 				= xSemaphoreCreateMutex();
		pulsador_config[1].tecla_pulsada_sem 	= xSemaphoreCreateBinary();

	    xTaskCreate(
			tarea_A,                     	// Funcion de la tarea a ejecutar
			( const char * )"tarea_A",   	// Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
			NULL,        					// Parametros de tarea
			tskIDLE_PRIORITY+1,         	// Prioridad de la tarea
			0                           	// Puntero a la tarea creada en el sistema
		);

	    xTaskCreate(
	    	tarea_B		,                   // Funcion de la tarea a ejecutar
	        ( const char * )"tarea_B",      // Nombre de la tarea como String amigable para el usuario
	        configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
			&pulsador_config[0],            // Parametros de tarea
	        tskIDLE_PRIORITY+1,         	// Prioridad de la tarea
	        0                           	// Puntero a la tarea creada en el sistema
	    );

	    xTaskCreate(
	    	tarea_B,                        // Funcion de la tarea a ejecutar
	        ( const char * )"tarea_",      // Nombre de la tarea como String amigable para el usuario
	        configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
			&pulsador_config[1],            // Parametros de tarea
	        tskIDLE_PRIORITY+1,         	// Prioridad de la tarea
	        0                           	// Puntero a la tarea creada en el sistema
	    );

	    xTaskCreate(
	   	    	tarea_C,                    // Funcion de la tarea a ejecutar
	   	        ( const char * )"tarea_C",  // Nombre de la tarea como String amigable para el usuario
	   	        configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
	   			NULL			,           // Parametros de tarea
	   	        tskIDLE_PRIORITY+2,         // Prioridad de la tarea
	   	        0                           // Puntero a la tarea creada en el sistema
	   	    );

	    // Iniciar scheduler
	    vTaskStartScheduler();
	}

	// ---------- REPETIR POR SIEMPRE --------------------------
	while( TRUE )
	{
		// Si cae en este while 1 significa que no pudo iniciar el scheduler
	}

	// NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
	// directamenteno sobre un microcontroladore y no es llamado por ningun
	// Sistema Operativo, como en el caso de un programa para PC.
	return 0;
}
