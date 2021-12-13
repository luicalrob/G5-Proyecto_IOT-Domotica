/*
 ESP-NOW based sensor using a BME280 temperature/pressure/humidity sensor

 Sends readings every 15 minutes to a server with a fixed mac address

 It takes about 215 milliseconds to wakeup, send a reading and go back to sleep, 
 and it uses about 70 milliAmps while awake and about 25 microamps while sleeping, 
 so it should last for a good year even AAA alkaline batteries. 

 Anthony Elder
 License: Apache License v2
*/
#include <ESP8266WiFi.h>
extern "C" {
  #include <espnow.h>
}
// #include "SparkFunBME280.h"
#include "DHTesp.h"

// this is the MAC Address of the remote ESP server which receives these sensor readings
uint8_t remoteMac[] = {0x36, 0x33, 0x33, 0x33, 0x33, 0x33};

#define WIFI_CHANNEL 4
#define SLEEP_SECS 15 * 60 // 15 minutes
#define SEND_TIMEOUT 245  // 245 millis seconds timeout 

// keep in sync with slave struct
struct __attribute__((packed)) SENSOR_DATA {
    float temp;
    float humidity;
} sensorData;

// BME280 bme280;
DHTesp dht;

volatile boolean callbackCalled;

void setup() {
  Serial.begin(115200); 
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");
  
  // read sensor first before awake generates heat
  // readBME280();
  dht.setup(5, DHTesp::DHT11); // Connect DHT sensor to GPIO 5
  
  WiFi.mode(WIFI_STA); // Station mode for esp-now sensor node
  WiFi.disconnect();

  Serial.printf("This mac: %s, ", WiFi.macAddress().c_str()); 
  Serial.printf("target mac: %02x%02x%02x%02x%02x%02x", remoteMac[0], remoteMac[1], remoteMac[2], remoteMac[3], remoteMac[4], remoteMac[5]); 
  Serial.printf(", channel: %i\n", WIFI_CHANNEL); 

  if (esp_now_init() != 0) {
    Serial.println("*** ESP_Now init failed");
    gotoSleep();
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(remoteMac, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, NULL, 0);

  esp_now_register_send_cb([](uint8_t* mac, uint8_t sendStatus) {
    Serial.printf("send_cb, send done, status = %i\n", sendStatus);
    callbackCalled = true;
  });

  callbackCalled = false;

  uint8_t bs[sizeof(sensorData)];
  memcpy(bs, &sensorData, sizeof(sensorData));
  esp_now_send(NULL, bs, sizeof(sensorData)); // NULL means send to all peers
}

void loop() {
  if (callbackCalled || (millis() > SEND_TIMEOUT)) {
    gotoSleep();
  }
  else {
    delay(dht.getMinimumSamplingPeriod());
    sensorData.humidity = dht.getHumidity();
    sensorData.temp = dht.getTemperature();
    Serial.printf("temp=%01f, humidity=%01f, sensorData.temp, sensorData.humidity);
    Serial.print(dht.getStatusString());
    Serial.print("\t");
    Serial.print(sensorData.humidity, 1);
    Serial.print("\t\t");
    Serial.print(sensorData.temp, 1);
    Serial.print("\t\t");
    //Serial.print(dht.toFahrenheit(temperature), 1);
    //Serial.print("\t\t");
    //Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
    //Serial.print("\t\t");
  }
}

//void readBME280() {
//  bme280.settings.commInterface = I2C_MODE;
//  bme280.settings.I2CAddress = 0x76;
//  bme280.settings.runMode = 2; // Forced mode with deepSleep
//  bme280.settings.tempOverSample = 1;
//  bme280.settings.pressOverSample = 1;
//  bme280.settings.humidOverSample = 1;
//  Serial.print("bme280 init="); Serial.println(bme280.begin(), HEX);
//  sensorData.temp = bme280.readTempC();
//  sensorData.humidity = bme280.readFloatHumidity();
//  Serial.printf("temp=%01f, humidity=%01f, pressure=%01f\n", sensorData.temp, sensorData.humidity, sensorData.pressure);
//}

void gotoSleep() {
  // add some randomness to avoid collisions with multiple devices
  int sleepSecs = SLEEP_SECS + ((uint8_t)RANDOM_REG32/2); 
  Serial.printf("Up for %i ms, going to sleep for %i secs...\n", millis(), sleepSecs); 
  ESP.deepSleep(sleepSecs * 1000000, RF_NO_CAL);
}
