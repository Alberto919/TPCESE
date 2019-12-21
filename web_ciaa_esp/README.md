# WEB-CIAA WEBSOCKETS
## Alcance

El presente trabajo práctico consiste en el desarrollo de un protocolo de comunicaciones que permitirá la comunicación entre el módulo WIFI y la placa EDU-CIAA a través del puerto UART y la implementación de funciones de comunicación utilizando Websockets que permitirán a un usuario mediante una página web la cual se encuentra almacenada en una tarjeta SD con una imagen de la placa EDU-CIAA, encender/apagar el LED correspondiente, como también visualizar el LED encendido cuando el evento se haya generado en la placa EDU-CIAA al presionar una TECLA.

## Funcionalidad

- Consiste en utilizar las 4 teclas de la placa EDU-CIAA usando algoritmos anti-rebote que permitan encender/apagar el led correspondiente.
- Para lograr la comunicación entre la EDU-CIAA y el Esp8266 se desarrolló un protocolo de comunicación que se ejecuta en ambos módulos (CIAA-Esp8266) utilizando el puerto UART.
- Se extiende la funcionalidad de los LED, para que este sea accedido de manera remota por medio Websockets.
- El Módulo WIFI deberá acceder a una tarjeta SD la cual contendrá una página web que será visualizada al usuario, la cual incluye una imagen de la EDU-CIAA con los eventos implementador utilizando websockets de cada tecla para el encendido/apagado de los LED correspondientes en la pantalla, este evento será ejecutado en la placa EDU-CIAA.

## Archivos
- La carpeta EspWebSocket contiene el codigo, librerias que permiten la comunicacion  y la conexión WIFI y Websockets y el codigo de la maquina de estados encagado de procesar los mensajes recibidos por el puerto Serie del Esp8266.
- La carpeta webCIAA contiene el codigo, librerias, algoritmos anti-rebote y el codigo de la maquina de estados encagado de procesar los mensajes recibidos por el puerto UART de la placa EDU-CIAA.

## Demostración
Se adjunto el video "WEB-CIAA _Demostracion.webm" demostrativo de la funcionalidad en este repositorio para el mismo se utilizó tres pantallas:
- Primera pantalla de la izquierda corresponde al navegador donde se visualiza la interfaz web.
- Pantalla central, es la captura de la pantalla del celular utilizando el servicio online **airmore** que permite visualizar la pantalla de un celular en un navegador web, visualizando la ejecución de una aplicación y utilizando la cámara del celular para filmar el comportamiento de la placa EDU-CIAA a los diferentes eventos.
- Pantalla derecha corresponde al software **Terminal** incluido en las herramientas de software CIAA para obtener los eventos del Serial del Esp8266

## Documentos
En este [link]( https://docs.google.com/document/d/1cE_czVjspbigCebbqGLYz8IlUxmQaNFyqvu13gk_8ew/edit?usp=sharing "link") se encuentra la propuesta del proyecto.

En este [link]( https://docs.google.com/presentation/d/1GnTxWdWebBpX4P5ZR5dRbzzVVnTeeLz8wzPD-vYMu_E/edit?usp=sharing "Propuesta TP") se encuentra la presentación del proyecto.
