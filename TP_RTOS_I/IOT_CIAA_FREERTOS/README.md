# [EDU-CIAA IOT](http://https://docs.google.com/presentation/d/11pMomkNtEciBdtx1zsFsMKl1bDqaIaTrHMHVl72e4FU/edit?usp=sharing "EDU-CIAA IOT")

## Alcance

El presente trabajo práctico consiste en la utilización de FreeRTOS, el desarrollo de un protocolo de comunicaciones que permitirá la comunicación entre el módulo WIFI y la placa EDU-CIAA a través del puerto UART y la implementación de funciones de comunicación con un Broker MQTT por medio de suscripciones y publicaciones que permitirá encender/apagar el LED RGB como también capturar el evento de la TECLA_1 de manera remota.

## Funcionalidad

- Consiste en utilizar las 4 teclas de la placa EDU-CIAA usando algoritmos anti-rebote que permitan encender/apagar el led correspondiente.
- Se extiende la funcionalidad del led RGB, para que este sea accedido de manera remota por medio de la subscribe/publish a un topic con un servidor MQTT.
- El  Módulo WIFI deberá publicar un topic en el servidor MQTT cada vez que la TECLA_1 sea pulsada y el LED RGB sea encendido/apagado.
- Para lograr la comunicación entre la EDU-CIAA y el Esp8266 se desarrolló un protocolo de comunicación que se ejecuta en ambos módulos (CIAA-Esp8266) utilizando el puerto UART.

## Archivos
- La carpeta EspClientMQTT contiene el codigo, librerias que permiten la conexion a un servidor MQTT y la conexión WIFI y el codigo de la maquina de estados encagado de procesar los mensajes recibidos por el puerto Serie del Esp8266.
- La carpeta IOTCIAA contiene el codigo, librerias, algoritmos anti-rebote y el codigo de la maquina de estados encagado de procesar los mensajes recibidos por el puerto UART de la placa EDU-CIAA.

## Demostración
Se adjunto el video "RTOS_Demostracion.webm" demostrativo de la funcionalidad en este repositorio, ([para mas detalle ver presentacion](httphttps://docs.google.com/presentation/d/11pMomkNtEciBdtx1zsFsMKl1bDqaIaTrHMHVl72e4FU/edit?usp=sharing:// "para mas detalle ver presentacion")) para el mismo se utilizó tres pantanllas:
- Primera pantalla de la izquierda corresponde al servidor MQTT en la nube **CloudMQTT**.
- Pantalla central, es la captura de la pantalla del celular utilizando el servicio online **airmore** que permite visualizar la pantalla de un celular en un navegador web, visualizando la ejecución de una aplicacion.
- Pantalla derecha corresponde al software **Terminal** incluido en las herramientas de software CIAA para obtener los eventos del Serial del Esp8266

## Documentos
En este [link](https://docs.google.com/presentation/d/11pMomkNtEciBdtx1zsFsMKl1bDqaIaTrHMHVl72e4FU/edit?usp=sharing "Propuesta TP") se encuntra la propuesta del proyecto.

En este [link](https://docs.google.com/document/d/1IpkCeg_G4Kr9UTiBjQHjAVO8RafuIOYNcInUGQhz4nY/edit?usp=sharing "link") se encuntra la presentacion del proyecto.
