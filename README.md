# Proyecto_IOT-Domotica
Proyecto de asistencia en iluminación, temperatura y música para un usuario en el hogar

# Índice de contenidos
  1. [Descripción del proyecto](https://github.com/luicalrob/G5-Proyecto_IOT-Domotica/blob/main/README.md#descripci%C3%B3n-del-proyecto)
  2. [Prerequesitos Software](https://github.com/luicalrob/G5-Proyecto_IOT-Domotica/blob/main/README.md#prerequesitos-software)
  3. [Prerequesitos Hardware](https://github.com/luicalrob/G5-Proyecto_IOT-Domotica/blob/main/README.md#prerequesitos-hardware)
  4. [Organización del proyecto](https://github.com/luicalrob/G5-Proyecto_IOT-Domotica/blob/main/README.md#organizaci%C3%B3n-del-proyecto)
  5. [Autores](https://github.com/luicalrob/G5-Proyecto_IOT-Domotica/blob/main/README.md#autores)
  6. [Licencia](https://github.com/luicalrob/G5-Proyecto_IOT-Domotica/blob/main/README.md#licencia)
  7. [Galería](https://github.com/luicalrob/G5-Proyecto_IOT-Domotica/blob/main/README.md#galer%C3%ADa)
  8. [Referencias](https://github.com/luicalrob/G5-Proyecto_IOT-Domotica/blob/main/README.md#galer%C3%ADa)

# Descripción del Proyecto

La ESP8266 es un módulo para Arduino de bajo costo que permite la conexión Wi-Fi de la placa en cuestión con la red. Esto permitió que todos los proyectos que hasta ahora requerían conexión física pudiesen trabajar a distancia, controlar desde una puerta de garaje, hasta una alarma casera o construir un controlador LED. En nuestro caso, nos adaptaremos a la comodidad del usuario, especificamente en el hogar/vivienda. El proyecto ofrecerá un servicio de variación de luminosidad de la estancia, otro de estabilización de la temperatura en una referencia agradable y por último, un servicio sugerencias de música. El sistema incluirá el uso de la aplicación de Telegram como interfaz de interacción con el usuario. En ella el usuario podrá elegir entre utilizar las canciones, la temperatura y la luminosidad sugerida o utilizar una personalizada. Las canciones se reproducirán mediante el asistente virtual "Alexa", para la temperatura se usará un aire acondicionado y para la luminosidad una cinta de LEDs de habitación.

El algoritmo que determinará las sugerencias ofrecidas al usuario se basará en gran medida en la meteorología del exterior. En especial con la medida de la luminosidad exterior que llega a la habitación. Adicionalmente, el proyecto contará con un sistema de activación mediante sensor táctil, y un sistema de renovación de la sesión mediante un sensor de movimiento. Es decir, el sistema permanecerá apagado hasta que el usuario interaccione con el sensor táctil. El sistema, una vez pasado un tiempo, se apagará suponiendo que nadie lo está usando. No obstante, una vez activado no será estrictamente necesario renovar la sensión mediante el sensor táctil, sino que el sistema contará con un sensor de movimiento que detectará el movimiento del usuario por la habitación.

# Prerequesitos Software

- NodeRed
- MQTT Broker Mosquitto
- Arduino
- Telegram
- Youtube
- 
# Prerequesitos Hardware

# Organización del proyecto

# Autores

# Licencia

# Galería

# Referencias
[Acerca de ESP8266](https://hardzone.es/reportajes/tema/esp8266-2n2222-arduino/)
