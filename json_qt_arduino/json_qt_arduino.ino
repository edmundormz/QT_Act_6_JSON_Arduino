#include <FirmataParser.h>
#include <FirmataDefines.h>
#include <FirmataConstants.h>
#include <Firmata.h>
#include <FirmataMarshaller.h>
#include <Boards.h>
#include <ArduinoJson.h>

String cadenaJSON;
bool finCadena = false;
const int LED1  = 13;
int STATUS_LED1 = 0;
const int LED2  = 9;
int STATUS_LED2 = 0;
const int ADC1  = 0;
const int ADC2  = 1;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Serial.begin(115200);
  cadenaJSON.reserve(100);
}

void JsonRead() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(cadenaJSON);
  // Probar si el "parsing" fue exitoso.
  if (!root.success()) {
    //Serial.println("Fallo el parsing");
    return;
  }
  digitalWrite(LED1, root["LED1"]);
  digitalWrite(LED2, root["LED2"]);

  root.printTo(Serial);
}

void loop() {
  if (finCadena) {
    finCadena = false;
    JsonRead();
    cadenaJSON = "";
  }
}

void serialEvent() {
  while (Serial.available()) {              //Si existen datos seriales, leer a todos
    char CaracterEntrada = Serial.read();   //Leer 1 byte serial recibido
    cadenaJSON += CaracterEntrada;   //Agregar el nuevo char a una cadena String
    if (CaracterEntrada == '}') {          //Si el char o byte recibido es un fin de linea, activa la bandera
      finCadena = true;                        //Si la bandera finCadena = 1, entonces la transmision esta completa
    }
  }
}


