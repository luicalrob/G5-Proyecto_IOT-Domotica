//-------------------------INCLUIR LIBRERIÍAS-----------------------//
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include <PubSubClient.h>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>

#include "DHTesp.h"

// datos para actualización   >>>> SUSTITUIR IP <<<<<
#define HTTP_OTA_ADDRESS      F("192.168.1.146")         // Address of OTA update server
#define HTTP_OTA_PATH         F("/esp8266-ota/update") // Path to update firmware
#define HTTP_OTA_PORT         1880                     // Port of update server
                                                       // Name of firmware
                                                        // Name of firmware
#define HTTP_OTA_VERSION      String(__FILE__).substring(String(__FILE__).lastIndexOf('\\')+1) + ".nodemcu"                                                       
DHTesp dht;

WiFiClient wClient;
PubSubClient mqtt_client(wClient);

// funciones para progreso de OTA
void progreso_OTA(int,int);
void final_OTA();
void inicio_OTA();
void error_OTA(int);

//-----------------------VARIABLES ACCESO WIFI----------------------//
// Update these with values suitable for your network.
                                                       // Name of firmware
const char* ssid = "Redmi";
const char* password = "ordavidro";
//const char* ssid = "MiFibra-1738";
//const char* password = "zn6TAwZx";
const char* mqtt_server = "iot.ac.uma.es";
const char* mqtt_user = "infind";
const char* mqtt_pass = "zancudo";


//-----------------------VARIABLES TOPICS--------------------------//
// cadenas para topics e ID
char ID_PLACA[16];
char topic_PUB[256];
char topic_SUB[256];

char topic_PUB_2[256];
char topic_SUB_1[256];
char topic_PUB_3[256];
char topic_PUB_4[256];


//--------------------VARIABLES GLOBALES--------------------------//
// GPIOs
int LED1 = 2;  
int LED2 = 16; 
float valor_led;;
int pwm;

#define TAMANHO_MENSAJE 128
unsigned long ultimo_mensaje=0;

struct registro_datos {
  float temperatura;
  float humedad;
  unsigned long tiempo;
  float vcc;
};

ADC_MODE(ADC_VCC);


//------------------------------------------------------------------
//                         FUNCIONES
//------------------------------------------------------------------
//-----------------------------------------------------
void intenta_OTA()
{ 
  Serial.println( "------------------------------" );  
  Serial.println( "Comprobando actualización:" );
  Serial.print(HTTP_OTA_ADDRESS);Serial.print(":");Serial.print(HTTP_OTA_PORT);Serial.println(HTTP_OTA_PATH);
  Serial.println( "--------------------------" );  
  ESPhttpUpdate.setLedPin(LED2, LOW);
  ESPhttpUpdate.onStart(inicio_OTA);
  ESPhttpUpdate.onError(error_OTA);
  ESPhttpUpdate.onProgress(progreso_OTA);
  ESPhttpUpdate.onEnd(final_OTA);
  WiFiClient wClient;
  switch(ESPhttpUpdate.update(wClient, HTTP_OTA_ADDRESS, HTTP_OTA_PORT, HTTP_OTA_PATH, HTTP_OTA_VERSION)) {
    case HTTP_UPDATE_FAILED:
      Serial.printf(" HTTP update failed: Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println(F(" El dispositivo ya está actualizado"));
      break;
    case HTTP_UPDATE_OK:
      Serial.println(F(" OK"));
      break;
    }
}

//-----------------------------------------------------
void final_OTA()
{
  Serial.println("Fin OTA. Reiniciando...");
}

void inicio_OTA()
{
  Serial.println("Nuevo Firmware encontrado. Actualizando...");
}

void error_OTA(int e)
{
  char cadena[64];
  snprintf(cadena,64,"ERROR: %d",e);
  Serial.println(cadena);
}

void progreso_OTA(int x, int todo)
{
  char cadena[256];
  int progress=(int)((x*100)/todo);
  if(progress%10==0)
  {
    snprintf(cadena,256,"Progreso: %d%% - %dK de %dK",progress,x/1024,todo/1024);
    Serial.println(cadena);
  }
}

//---------------- SERIALIZACIÓN DE MENSAJES POR JSON------------------------//
String serializa_JSON (struct registro_datos datos)
{
  StaticJsonDocument<192> jsonRoot;
  String jsonString;
 
  jsonRoot["Uptime"]= datos.tiempo;
  jsonRoot["Vcc"]= datos.vcc/1000;
  JsonObject DHT11=jsonRoot.createNestedObject("DHT11");
  DHT11["temp"] = datos.temperatura;
  DHT11["hum"] = datos.humedad;
  jsonRoot["LED"]= valor_led;
  JsonObject Wifi=jsonRoot.createNestedObject("Wifi");
  Wifi["SSId"] = ssid;
  Wifi["IP"] = WiFi.localIP().toString();
  long rssi=WiFi.RSSI();
  Wifi["RSSI"] = rssi;
  
  
  serializeJson(jsonRoot,jsonString);
  return jsonString;
}


//---------------- SERIALIZACIÓN DE MENSAJES POR JSON2------------------------//
String serializa_JSON2 (float valor_led)
{
  StaticJsonDocument<32> jsonRoot;
  String jsonString;
          
  jsonRoot["led"]= valor_led;
  
  serializeJson(jsonRoot,jsonString);
  return jsonString;
}

//---------------- SERIALIZACIÓN DE MENSAJES POR JSON3------------------------//
String serializa_JSON3 (String estado)
{
  StaticJsonDocument<32> jsonRoot;
  String jsonString;
 
  jsonRoot["Online"]= estado;
  
  serializeJson(jsonRoot,jsonString);
  return jsonString;
}

//---------------------------CONECTAR WIFI------------------------------//
void conecta_wifi() {
  Serial.printf("\nConnecting to %s:\n", ssid);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.printf("\nWiFi connected, IP address: %s\n", WiFi.localIP().toString().c_str());
}

//------------------------------CONECTAR MQTT---------------------------//
void conecta_mqtt() {
  
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
      // Publicamos el estado de la conexion en el topic
      char buffer[512];
      String dato_conexion="false";
      dato_conexion=serializa_JSON3(dato_conexion);
      strcpy(buffer,dato_conexion.c_str());
      //mqtt_client.publish(topic_PUB_2, buffer);
      
    // Attempt to connect
    if (mqtt_client.connect(ID_PLACA, mqtt_user, mqtt_pass,topic_PUB_2,1,true,buffer,true)) {
      Serial.printf(" conectado a broker: %s\n",mqtt_server);
      mqtt_client.subscribe(topic_SUB);
      mqtt_client.subscribe(topic_SUB_1);
      
      // Publicamos el estado de la conexion en el topic
      char buffer[512];
      String dato_conexion="true";
      dato_conexion=serializa_JSON3(dato_conexion);
      strcpy(buffer,dato_conexion.c_str());
      mqtt_client.publish(topic_PUB_2, buffer, true);
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      

      
      //mqtt_client.publish(topic_PUB_2, "Offline", true);
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  
}

//---------------------------------CALLBACK-------------------------------//
void callback(char* topic, byte* payload, unsigned int length) {
  char *mensaje=(char *)malloc(length+1); // reservo memoria para copia del mensaje
  strncpy(mensaje,(char*)payload,length); // copio el mensaje en cadena de caracteres
  mensaje[length]='\0';

String dato_led;

    char buffer[512];
//    strcpy(buffer,dato_led.c_str());
  
  Serial.printf("Message arrived [%s]: %s\n",topic, mensaje);
  
  // compruebo que es el topic adecuado
  if(strcmp(topic,"infind/GRUPO5/led/cmd")==0)
  {
    StaticJsonDocument<512> root; // el tamaño tiene que ser adecuado para el mensaje
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(root, mensaje,length);
    // Compruebo si no hubo error
    if (error) {
      Serial.print("Error deserializeJson() failed: ");
      Serial.println(error.c_str());
    }
    else
    if(root.containsKey("level"))  // comprobar si existe el campo/clave que estamos buscando
    { 
     valor_led = root["level"];
    // valor1=valor;
     valor_led = ((255.0/100.0)*(100-valor_led));
     Serial.print("Mensaje OK, level = ");
     Serial.println(valor_led);
     analogWrite(LED1,valor_led);

     valor_led = 100-((100.0/255.0)*valor_led); //Conversión para volver a mostrar el estado del led entre 0 y 100
  
  //Serial.print("El valor del LED ES:: ");
  //0Serial.print(valor_led); 
    dato_led=serializa_JSON2(valor_led);
    Serial.println(dato_led);
    strcpy(buffer,dato_led.c_str());
    mqtt_client.publish(topic_PUB_4, buffer);
    
    }
    else
    {
      Serial.print("Error : ");
      Serial.println("\"level\" key not found in JSON");
    }
  } // if topic
  else
  {
    Serial.println("Error: Topic desconocido");
  }

  free(mensaje); // libero memoria
}



//-------------------------------------------------------------
//                             SETUP
//-------------------------------------------------------------
void setup() {

  // Inicializamos el Serial
  Serial.begin(115200);
  Serial.println();
  Serial.println("Empieza setup...");

  pinMode(LED1, OUTPUT);    // inicializa GPIO como salida
  pinMode(LED2, OUTPUT);    
  digitalWrite(LED1, HIGH); // apaga el led
  digitalWrite(LED2, HIGH); 

  
  // Crea topics usando id de la placa
  sprintf(ID_PLACA, "ESP_%d", ESP.getChipId());
  sprintf(topic_PUB, "infind/%s/pub", ID_PLACA);
  sprintf(topic_SUB, "infind/%s/sub", ID_PLACA);
  
  sprintf(topic_PUB_2, "infind/%s/conexion", "GRUPO5"); // PUBLICAR
  sprintf(topic_PUB_3, "infind/%s/datos", "GRUPO5"); // PUBLICAR
  sprintf(topic_SUB_1, "infind/%s/led/cmd", "GRUPO5"); // SUBSCRIBIR
  sprintf(topic_PUB_4, "infind/%s/led/status", "GRUPO5"); // PUBLICAR

  // Conectamos la placa al Broker
  conecta_wifi();
   intenta_OTA();
  mqtt_client.setServer(mqtt_server, 1883);
  mqtt_client.setBufferSize(512); // para poder enviar mensajes de hasta X bytes
  mqtt_client.setCallback(callback);
  conecta_mqtt();

  // Inicializamos los Topics
  Serial.printf("Identificador placa: %s\n", ID_PLACA );
  Serial.printf("Topic publicacion  : %s\n", topic_PUB);
  Serial.printf("Topic subscripcion : %s\n", topic_SUB);

  Serial.printf("Topic publicacion conexion : %s\n", topic_PUB_2);
  Serial.printf("Topic publicacion datos : %s\n", topic_PUB_3);
  Serial.printf("Topic subscripcion led : %s\n", topic_SUB_1);
  Serial.printf("Topic publicacion estado led : %s\n", topic_PUB_4);


  // Inicializamos el Sensor
  dht.setup(5, DHTesp::DHT11); // Connect DHT sensor to GPIO 5

  // Terminamos el SetUp
  Serial.printf("Termina setup en %lu ms\n\n",millis());
}

unsigned long ultimo = 0;

//-----------------------------------------------------------
//                           LOOP
//-----------------------------------------------------------
void loop() {

  // Comprobamos la conexión a MQTT
  if (!mqtt_client.connected()) conecta_mqtt(); // Y si no lo está lo conectamos
  
  mqtt_client.loop(); // esta llamada para que la librería funcione
  //mqtt_client.subscribe(topic_SUB_1);
  
  // Condición de envío de mensajes cada 30 segundos
  unsigned long ahora = millis();
  if (ahora - ultimo_mensaje >= 5000) {
    char mensaje[TAMANHO_MENSAJE];
    ultimo_mensaje = ahora;
    snprintf (mensaje, TAMANHO_MENSAJE, "Mensaje Enviado desde %s en %6lu ms", ID_PLACA, ahora);
    Serial.println(mensaje);
    mqtt_client.publish(topic_PUB, mensaje);
    digitalWrite(LED2, LOW); // enciende el led al enviar mensaje

    //vin=analogRead(A0);
    //callback("infind/GRUPO5/led/cmd", (byte*)cadena, strlen(cadena));



    // Declaración de variables
    //char mensaje2[128];  // cadena de 128 caracteres
    struct registro_datos sensor;
    String datos_sensor;
    String dato_led;
    
    // Obtenemos los datos del Sensor DH11
    sensor.humedad = dht.getHumidity();
    sensor.temperatura = dht.getTemperature();
    sensor.tiempo=millis();
    sensor.vcc= ESP.getVcc();

    // Serializamos el mensaje
    datos_sensor=serializa_JSON(sensor);
    Serial.println(datos_sensor);
    //dato_led=serializa_JSON2(valor);
    //Serial.println(dato_led);
    
    // Pasamos de String a char para poder publicarlo
    char buffer[512];
    strcpy(buffer,datos_sensor.c_str());

    // snprintf(mensaje2, 128, "{\"temperatura\": %f, \"humedad\": %f}", temp, hum);
    //mqtt_client.publish(topic_PUB_2, mensaje2);
    mqtt_client.publish(topic_PUB_3, buffer);

    //char buffer[100];
   // strcpy(buffer,dato_led.c_str());
   // mqtt_client.publish(topic_PUB_4, buffer);
  
  }
  if (millis()-ultimo >= 50)
  {
    ultimo = millis();
    digitalWrite(LED1, !digitalRead(LED1));   // Toggle the LED 
  }
    
}
