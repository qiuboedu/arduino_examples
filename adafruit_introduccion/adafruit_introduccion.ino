/*
 *  Mi Primera Aplicación con IoT
 */

// Bibliotecas

#include "DHTesp.h"
#include "QiuboESP8266.h"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"       // io.adafruit.com
#define AIO_SERVERPORT  1883                    // use 8883 for SSL
#define AIO_USERNAME    "<username>"           // Usuario de Adafruit
#define AIO_KEY         "<API KEY>"            // API Key de Adafruit

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

/**************************** Feeds ***************************************************************/

/*** Publishers ***/
Adafruit_MQTT_Publish temp1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperatura1");
Adafruit_MQTT_Publish temp2 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperatura2");

/*** Subscribers ***/
Adafruit_MQTT_Subscribe salidaDigital = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/boton", MQTT_QOS_1);

/*** Variables Globales ***/

DHTesp dht;
unsigned long prevMillis = 0;
QiuboESP8266 qiubo;

/*** Subscriber Callback ***/

void digitalCallback(char *data, uint16_t len) {
  Serial.print("Hey we're in a onoff callback, the button value is: ");
  Serial.println(data);

     String message = String(data);
      message.trim();
      if (message == "ON") {digitalWrite(12, HIGH);}
      if (message == "OFF") {digitalWrite(12, LOW);} 
}

void setup() {

  dht.setup(D5, DHTesp::DHT11);
  pinMode(12, OUTPUT);          // D6 salida digital
  digitalWrite(12, LOW);

  Serial.begin(115200);
  Serial.println("");
  
  salidaDigital.setCallback(digitalCallback);
  mqtt.subscribe(&salidaDigital);

  /** Indica el nombre de red y la contraseña **/
 
  qiubo.connectToWiFi("<SSID>", "<PASSWORD>");
  
  Serial.println("Arduino Ready");
}

void loop() {

  if(qiubo.isConnected()) {

    qiubo.connectToMQTT(&mqtt); 
      
    mqtt.processPackets(500);
    unsigned long currentMillis = millis();
    
    if(currentMillis - prevMillis >= 5000) {
      prevMillis = currentMillis;
      float t = dht.getTemperature();
      temp1.publish(t);
      temp2.publish(t);
      Serial.print("Temp: ");
      Serial.println(t);
    }
  }
}
