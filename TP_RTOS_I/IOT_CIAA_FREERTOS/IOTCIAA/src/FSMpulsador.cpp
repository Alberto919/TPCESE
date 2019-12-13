/*
 * FSMpulsador.cpp
 *
 *  Created on: 7 dic. 2019
 *      Author: Alberto
 */

#include "FSMpulsador.h"
#include "sapi.h"

#define DEBOUNCE_TIME 20

FSMpulsador::FSMpulsador() {
	// TODO Auto-generated constructor stub

}

void FSMpulsador::fsmButtonInit(sPulsador* config)
{
	config->contFalling = 0;
	config->contRising = 0;
	config->fsmButtonState = STATE_BUTTON_UP; // estado inicial
}


// Actualiza el estado de la maquina de estados
void FSMpulsador::fsmButtonUpdate(sPulsador* config)
{

    switch( config->fsmButtonState )
    {
        case STATE_BUTTON_UP:
            /* COMPRUEBA LAS CONDICIONES DE TRANSICIÓN */
            if( !gpioRead( config->tecla ) )
            {
            	config->fsmButtonState = STATE_BUTTON_FALLING;
            }
            break;
        case STATE_BUTTON_FALLING:

            /* COMPRUEBA LAS CONDICIONES DE TRANSICIÓN */
            if( config->contFalling >= DEBOUNCE_TIME )
            {
                if( !gpioRead( config->tecla ) )
                {
                	config->fsmButtonState = STATE_BUTTON_DOWN;

                    /* ACCION DEL EVENTO*/
                    buttonPressed(config);
                }
                else
                {
                	config->fsmButtonState = STATE_BUTTON_UP;
                }

                config->contFalling = 0;
            }
            config->contFalling++;
            break;
        case STATE_BUTTON_DOWN:
			/* COMPRUEBA LAS CONDICIONES DE TRANSICIÓN */
			if( gpioRead( config->tecla ) )
			{
				config->fsmButtonState = STATE_BUTTON_RISING;
			}
			break;
        case STATE_BUTTON_RISING:

            /* COMPRUEBA LAS CONDICIONES DE TRANSICIÓN */

            if( config->contRising >= DEBOUNCE_TIME )
            {
                if( gpioRead( config->tecla ) )
                {
                	config->fsmButtonState = STATE_BUTTON_UP;

                    /* ACCION DEL EVENTO ! */
                    buttonReleased(config);
                }
                else
                {
                	config->fsmButtonState = STATE_BUTTON_DOWN;
                }
                config->contRising = 0;
            }
            config->contRising++;
            break;
        default:
            fsmButtonError(config);
            break;
    }
}

/*
 * Evento de tecla pulsada
 */
void FSMpulsador::buttonPressed(sPulsador* config){
	buttonPressed_funcion(config->tecla);
}

/*
 * Evento de tecla liberada
 */
void FSMpulsador::buttonReleased(sPulsador* config )
{
	buttonReleased_funcion(config->tecla);
}

/*
 * Evento de error
 */
void FSMpulsador::fsmButtonError(sPulsador* config )
{
	config->fsmButtonState = STATE_BUTTON_UP;
}

/*
 * Evento callback de tecla liberada
 */
void FSMpulsador::buttonReleased_callback(void (*funcionEjecutar)(gpioMap_t)) {
	buttonReleased_funcion = funcionEjecutar;
}

/*
 * Evento callback de tecla pulsada
 */
void FSMpulsador::buttonPressed_callback(void (*funcionEjecutar)(gpioMap_t)) {
	buttonPressed_funcion = funcionEjecutar;
}

