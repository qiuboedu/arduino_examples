  /*** Librerías ***/

  #include <DHTesp.h>
  #include <SFE_BMP180.h>
  #include <Wire.h>
  #include "QiuboESP8266.h"

  /*** Sensores ***/

  DHTesp dht;         // creamos una variable donde guardamos el pin(señal digital) y el modelo del sensor
  SFE_BMP180 bmp180;  // Creación de un objeto

  /*** Constantes ***/

  const int PIN_LLUVIA                  = 4;
  const double PRESION_AL_NIVEL_DEL_MAR = 1013.25; //presión real al nivel del mar en mbar

  /*** Variables Globales ***/

  QiuboESP8266 qiubo;

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"      // io.adafruit.com
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "<username>"           // Usuario de Adafruit
#define AIO_KEY         "<API KEY>"            // API Key de Adafruit

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

Adafruit_MQTT_Publish publisherPresion = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/presion");
Adafruit_MQTT_Publish publisherHumedad = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humedad");
Adafruit_MQTT_Publish publisherTemperatura = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperatura");
Adafruit_MQTT_Publish publisherLluvia = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/lluvia");

void setup() {
  
  pinMode(PIN_LLUVIA,INPUT);    // definimos el FC-37 como pin de entrada
  pinMode(5,INPUT);             //definimos como pin de entrada señal SCL
  pinMode(4,INPUT);             //definimos como pin de entrada señal SDA

  Serial.begin(9600);           // puerto de comunicación serial
  dht.setup(D5, DHTesp::DHT11); //llamado de la librería que se comunica con el sensor

  if(bmp180.begin())
   Serial.println("Sensor iniciado correctamente"); // inicialización del BMP180

   /** Indica el nombre de red y la contraseña **/
 
  qiubo.connectToWiFi("<SSID>", "<PASSWORD>");
}


void loop() {

if(qiubo.isConnected()) {

    qiubo.connectToMQTT(&mqtt);
  
    /*** Lectura del Sensor de Humedad y Temperatura ***/

    float temperatura = dht.getTemperature();  // lectura de la temperatura
    float humedad = dht.getHumidity();         // lectura de la humedad

    /***  texto que se imprime en el puerto serial ***/

    Serial.print("Temperatura ");
    Serial.print(temperatura);
    Serial.println("°C");
    Serial.print("Humedad ");
    Serial.print(humedad);
    Serial.println("%");

    publisherTemperatura.publish(temperatura);
    publisherHumedad.publish(humedad);

    /*** Lectura del Sensor de Lluvia ***/

    int nivelLluvia = digitalRead(PIN_LLUVIA);  // lectura del sensor

     if (nivelLluvia == LOW) {
        Serial.println("Con PIN_LLUVIA");       // texto que imprime
        publisherLluvia.publish("w:rain");
     } else {
       Serial.println("Sin PIN_LLUVIA");        // texto que imprime
       publisherLluvia.publish("cloud");
     }

    /** Inicialización de variables **/
    
    char status             = 0;
    double temperaturaBMP   = 0;
    double presionBMP       = 0;
    double altitudBMP       = 0;

    /*** Lectura del Sensor de Presión atmosférica ***/

    status = bmp180.startTemperature(); //Inicio de la lectura de la temperatura

    if (status != 0){
      
      delay(status); //pausa en la lectura
      status = bmp180.getTemperature(temperaturaBMP); //Obtención de la temperatura

        if ( status != 0) {
         status = bmp180.startPressure(3); //Inicio de la lectura de la presión

            if (status != 0) {

              delay(status);
              status= bmp180.getPressure(presionBMP,temperaturaBMP); // Obtención de la presión

                 if (status != 0) {

                   Serial.print("Presion atmosferica ");
                   Serial.print(presionBMP,2);
                   Serial.println("mb");

                   publisherPresion.publish(presionBMP);
    
                   altitudBMP = bmp180.altitude(presionBMP,PRESION_AL_NIVEL_DEL_MAR);
                   Serial.print("Altitud ");
                   Serial.print(altitudBMP);
                   Serial.println(" m.s.n.m");
                   Serial.println("");
        }
       }
      }
     }
  }
  delay(10000);
}
