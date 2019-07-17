/** Librerías **/

#include <SoftwareSerial.h>

/** Constantes **/

const int PIN_1 = 3;
const int PIN_2 = 5;
const int PIN_3 = 6;
const int PIN_4 = 2;

const byte numChars = 20;

/** Variables Globales **/

SoftwareSerial bt(10,11);

boolean newData = false;
char receivedChars[numChars];

String command;
int id = 0;
int value = 0;

void setup() {
  
  Serial.begin(9600);
  bt.begin(9600);
  Serial.println("Arduino listo");

  pinMode(PIN_1, OUTPUT);
  pinMode(PIN_2, OUTPUT);
  pinMode(PIN_3, OUTPUT);
  pinMode(PIN_4, OUTPUT);
}

void loop() {

  if(bt.available()) {
    recvWithStartEndMarkers();
  }

  if (newData) { 
    Serial.println("Leyendo");
    parseData(); 
  }

  if(Serial.available()) {
    Serial.println("Escribiendo");
    bt.write(Serial.read());
  }
}

void parseData() {
    
    newData = false;    
    Serial.println( receivedChars );
    command = receivedChars;
    char delim[] = "_";
    char *ptr = strtok(receivedChars, delim);

    Serial.println("**** SPLIT ****");
    String str = ptr;
    id = str.toInt();
    Serial.print(id);
    Serial.print(" - ");
    ptr = strtok(NULL, delim);
    str = ptr;
    value = str.toInt();
    Serial.println(ptr);

    switch(id) {
      case 1:
        analogWrite(PIN_1, value);
        break;
      case 2:
        analogWrite(PIN_2, value);
        break;
      case 3:
        analogWrite(PIN_3, value);
        break;
      case 4:
        digitalWrite(PIN_4, value);
        break;
    }
}

void recvWithStartEndMarkers() 
{
     
/** 
 *  function recvWithStartEndMarkers by Robin2 of the Arduino forums
 *  See  http://forum.arduino.cc/index.php?topic=288234.0
 **/

     static boolean recvInProgress = false;
     static byte ndx = 0;
     char startMarker = '<';
     char endMarker = '>';
     char rc;

     if (bt.available() > 0) 
     {
          rc = bt.read();
          if (recvInProgress == true) 
          {
               if (rc != endMarker) 
               {
                    receivedChars[ndx] = rc;
                    ndx++;
                    if (ndx >= numChars) { ndx = numChars - 1; }
               }
               else 
               {
                     receivedChars[ndx] = '\0'; // terminate the string
                     recvInProgress = false;
                     ndx = 0;
                     newData = true;
               }
          }

          else if (rc == startMarker) { recvInProgress = true; }
     }
}
