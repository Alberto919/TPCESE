# TP RTOS II
## EJERCICIO II

Implementar una librería para utilizar bajo el sistema FreeRTOS.
La librería debería implementar un mecanismo que permita intercambiar mensajes de
tamaño variable entre varias tareas productoras y una tarea consumidora de datos. El
contenido de cada mensaje puede ser cualquier tipo de dato. Se debe utilizar memoria
dinámica para alocar los espacios de memoria.
Para probar la librería, implemente un sistema de 3 tareas, en donde 2 sean productores y 1
consumidora.
- Tarea A: Encender periódicamente un LED. Cada vez que el LED encienda, se
deberá enviar un mensaje "LED ON"
- Tarea B: Lectura con antirrebote de 2 teclas y medición de tiempo de pulsación (sin
ISR). Al medirse el tiempo de pulsación, enviar el mensaje "TECx Tyyyy" donde x es
el índice de tecla e yyyy la cantidad de ms que fué pulsada.
- Tarea C: Deberá obtener mensajes de texto, y enviarlos por la uart.
Parte 2
Modificar la librería para que permita instanciar varias veces este mecanismo para poder
utilizarse concurrentemente en un mismo sistema (una instancia comunicando tareas con
un consumidor A y otra instancia comunicando tareas con un consumidor B).
Esta modificación debe ser probada modificando el ejercicio anterior de la siguiente manera:
- Instanciando 2 veces al objeto que gestiona la librería.
- La primera instancia, utilizarla tal cual la parte 1.
- Cada vez que la tarea C, envíe el texto por puerto serie, además deberá enviarlo
dicho texto a la tarea D a través de esta nueva instancia.
- Defina una Tarea D que consuma las cadenas de texto (que simplemente la
descarte).
