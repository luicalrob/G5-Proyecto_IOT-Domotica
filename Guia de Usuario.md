# GUIA DE USUARIO
Proyecto de asistencia en iluminación, temperatura y música para un usuario en el hogar.

# Índice de contenidos
  1. [Configuración Hardware](https://github.com/luicalrob/G5-Proyecto_IOT-Domotica/blob/main_FINAL/Guia%20de%20Usuario.md#configuraci%C3%B3n-hardware)
  2. [Configuración Software](https://github.com/luicalrob/G5-Proyecto_IOT-Domotica/blob/main_FINAL/Guia%20de%20Usuario.md#configuraci%C3%B3n-software)
  3. [Instrucciones de uso](https://github.com/luicalrob/G5-Proyecto_IOT-Domotica/blob/main_FINAL/Guia%20de%20Usuario.md#instrucciones-de-uso)
  4. [Personalización y Recursos](https://github.com/luicalrob/G5-Proyecto_IOT-Domotica/blob/main_FINAL/Guia%20de%20Usuario.md#personalizaci%C3%B3n-y-recursos)


# Configuración Hardware
En esta sección se indicarán los requisitos imprescindibles a configurar de forma previa en los equipos Hardaware del sistema para que este funcione correctamente. Estos se realizarán solo la primera vez que se instale el sistema:
-Conexiones de sensores y alimentaciones de la placa (foto de sensor de presencia)
-Conectar Alexa
# Configuración Software
En esta sección se indicarán los requisitos imprescindibles a configurar del sistema, estas configuraciones solo habrá que realizarlas la primera vez que lo queramos usar
-Activar la máquina virtual y la carpeta de OTA en la máquina virtual
-Configuración de conexión wifi en cada una de las placas (wifi y contraseña) y subir el nuevo archivo en FOTA //(explicar qué lineas de código habría que modificar, y como exportar binario a FOTA)
-Conexión del bot de Telegram: abrimos telegram en nuestro dispositivo elegido, y dentro buscamos al bot llamado "Prealexa"
-Declaración de Macs de cada una de las placas a utilizar en el proyecto que se van a utilizar (indicar que programa usar para obtener las macs y donde poner cada mac)

# Instrucciones de uso
-Encender máquina virtual y abrir nodeRed
-Abrir Dashboard en caso de querer ver los parametros del sistema
-Abrir en telegram conversación con el bot "Prealexa". Para empezar la conversación su puede usar tanto el comando /start como decir cualquier cosa. El bot te responderá saludándote y diciéndote la configuración recomendada según la climatología. Una vez hecha esta recomendación el usuario puede elegir si seguirla o coger otra más acorde con sus necesidades. Para elegir la recomendación el usuario tiene que responder al bot con el comando /continua. Para elegir cualquier otra configuración el usuario puede elegir entre los siguientes comandos /triste /alegre /chill /bailar (la configuración recomendada será una de estas 4). Estas configuraciones establecen que suene un tipo de música y que se enciendan leds de un color u otro para iluminar. Todas las configuraciones llevan incluidas un control del aire acondicionado para que el usuario esté lo más cómodo posible.

# Personalización y Recursos
-Podemos cambiar las playlist que se recomiendan por defecto (como)
-Podemos personalizar las luces de inicio del programa (como)
-Podemos modificar los tiempos de espera hasta que el sistema se apaga tras no detectar presencia en la habitación

