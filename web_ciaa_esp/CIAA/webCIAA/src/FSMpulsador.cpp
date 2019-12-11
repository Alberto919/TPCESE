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
	config->fsmButtonState = STATE_BUTTON_UP;  // Set initial state
}


// FSM Update Sate Function
void FSMpulsador::fsmButtonUpdate(sPulsador* config)
{
   // static bool_t flagFalling = FALSE;
    //static bool_t flagRising = FALSE;

    switch( config->fsmButtonState )
    {
        case STATE_BUTTON_UP:
            /* CHECK TRANSITION CONDITIONS */
            if( !gpioRead( config->tecla ) )
            {
            	config->fsmButtonState = STATE_BUTTON_FALLING;
            }
            break;

        case STATE_BUTTON_FALLING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */
            if( config->contFalling >= DEBOUNCE_TIME )
            {
                if( !gpioRead( config->tecla ) )
                {
                	config->fsmButtonState = STATE_BUTTON_DOWN;

                    /* ACCION DEL EVENTO !*/
                    buttonPressed(config);
                }
                else
                {
                	config->fsmButtonState = STATE_BUTTON_UP;
                }

                config->contFalling = 0;
            }

            config->contFalling++;

            /* LEAVE */
            break;

        case STATE_BUTTON_DOWN:
			/* CHECK TRANSITION CONDITIONS */
			if( gpioRead( config->tecla ) )
			{
				config->fsmButtonState = STATE_BUTTON_RISING;
			}
			break;

        case STATE_BUTTON_RISING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */

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

            /* LEAVE */
            break;

        default:
            fsmButtonError(config);
            break;
    }
}

/* accion de el evento de tecla pulsada */
void FSMpulsador::buttonPressed(sPulsador* config){
	buttonPressed_funcion(config->tecla);
}

/* accion de el evento de tecla liberada */
void FSMpulsador::buttonReleased(sPulsador* config )
{
	buttonReleased_funcion(config->tecla);
}

void FSMpulsador::fsmButtonError(sPulsador* config )
{
	config->fsmButtonState = STATE_BUTTON_UP;
}

void FSMpulsador::buttonReleased_callback(void (*funcionEjecutar)(gpioMap_t)) {
	buttonReleased_funcion = funcionEjecutar;
}

void FSMpulsador::buttonPressed_callback(void (*funcionEjecutar)(gpioMap_t)) {
	buttonPressed_funcion = funcionEjecutar;
}

