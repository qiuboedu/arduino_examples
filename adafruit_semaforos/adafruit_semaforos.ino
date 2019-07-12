// Librerías

#include "QiuboESP8266.h"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"      // io.adafruit.com
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "qiuboedu"
#define AIO_KEY         "22d175fcd21b459887afb26fbd5d5cea"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

/**************************** Feeds ***************************************************************/
Adafruit_MQTT_Publish publisherVerde1     = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/verde1");
Adafruit_MQTT_Publish publisherAmarillo1  = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/yellow1");
Adafruit_MQTT_Publish publisherRojo1      = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/rojo1");

Adafruit_MQTT_Publish publisherVerde2     = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/verde2");
Adafruit_MQTT_Publish publisherAmarillo2  = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/amarillo2");
Adafruit_MQTT_Publish publisherRojo2      = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/rojo2");

//-------------------VARIABLES GLOBALES--------------------------

QiuboESP8266 qiubo;

unsigned long prevMillis = 0;

//Semaforo 1

const int amarillo  = 0;
const int rojo      = 2;
const int verde     = 4;

//Semaforo 2

const int amarillo2 = 12;
const int rojo2     = 13;
const int verde2    = 14;

const int LUZ_DURACION      = 15000;
const int AMARILLO_DURACION = 4000;
const int BLINK_DURACION    = 4000;

int estadoVerde1    = -1;
int estadoAmarillo1 = -1;
int estadoRojo1     = -1;

int estadoVerde2    = -1;
int estadoAmarillo2 = -1;
int estadoRojo2     = -1;


void setupSemaforo1(int ledVerde, int ledAmarillo, int ledRojo) {
  
  if(estadoVerde1 != ledVerde) {
    estadoVerde1 = ledVerde;
    publisherVerde1.publish(ledVerde);
  }
  
  if(estadoAmarillo1 != ledAmarillo) {
    estadoAmarillo1 = ledAmarillo;
    publisherAmarillo1.publish(ledAmarillo);
  }

  if(estadoRojo1 != ledRojo) {
    estadoRojo1 = ledRojo;
    publisherRojo1.publish(ledRojo);
  }

  digitalWrite(verde, ledVerde);        // HIGH LED encendido - LOW LED apagado
  digitalWrite(amarillo, ledAmarillo);  // HIGH LED encendido - LOW LED apagado
  digitalWrite(rojo, ledRojo);          // HIGH LED encendido - LOW LED apagado
}

void setupSemaforo2(int ledVerde, int ledAmarillo, int ledRojo) {
  
  if(estadoVerde2 != ledVerde) {
    estadoVerde2 = ledVerde;
    publisherVerde2.publish(ledVerde);
  }
  
  if(estadoAmarillo2 != ledAmarillo) {
    estadoAmarillo2 = ledAmarillo;
    publisherAmarillo2.publish(ledAmarillo);
  }

  if(estadoRojo2 != ledRojo) {
    estadoRojo2 = ledRojo;
    publisherRojo2.publish(ledRojo);
  }
  
  digitalWrite(verde2, ledVerde);       // HIGH LED encendido - LOW LED apagado
  digitalWrite(amarillo2, ledAmarillo); // HIGH LED encendido - LOW LED apagado
  digitalWrite(rojo2, ledRojo);         // HIGH LED encendido - LOW LED apagado
}

void setup() {
  
  Serial.begin(115200);
  
  // definimos el LED como pin digital de salida
  
  pinMode(rojo, OUTPUT);
  pinMode(amarillo, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(rojo2, OUTPUT);
  pinMode(amarillo2, OUTPUT);
  pinMode(verde2, OUTPUT);
  
  qiubo.connectToWiFi("IPLANLiv-lcermeno-2.4Ghz", "01112014");
}

void loop() {

  if(qiubo.isConnected()) {

    qiubo.connectToMQTT(&mqtt);

    setupSemaforo1(HIGH, LOW, LOW);
    setupSemaforo2(LOW, LOW, HIGH);

    delay(LUZ_DURACION);
    
    /** amarillo **/
    
    setupSemaforo1(LOW, HIGH, LOW);
    setupSemaforo2(LOW, HIGH, LOW);

    delay(BLINK_DURACION);
    
    setupSemaforo1(LOW, LOW, LOW);
    setupSemaforo2(LOW, LOW, LOW);

    delay(BLINK_DURACION);
    
    setupSemaforo1(LOW, HIGH, LOW);
    setupSemaforo2(LOW, HIGH, LOW);

    delay(AMARILLO_DURACION);
    
    /** amarillo **/

    setupSemaforo1(LOW, LOW, HIGH);
    setupSemaforo2(HIGH, LOW, LOW);

    delay(LUZ_DURACION);

    /** amarillo **/
    
    setupSemaforo1(LOW, HIGH, LOW);
    setupSemaforo2(LOW, HIGH, LOW);

    delay(BLINK_DURACION);
    
    setupSemaforo1(LOW, LOW, LOW);
    setupSemaforo2(LOW, LOW, LOW);

    delay(BLINK_DURACION);
    
    setupSemaforo1(LOW, HIGH, LOW);
    setupSemaforo2(LOW, HIGH, LOW);

    delay(AMARILLO_DURACION);
  }
}
