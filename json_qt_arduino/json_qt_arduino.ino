#include <FirmataParser.h>
#include <FirmataDefines.h>
#include <FirmataConstants.h>
#include <Firmata.h>
#include <FirmataMarshaller.h>
#include <Boards.h>


#include <ArduinoJson.h>

 double coordenadaX = 0.0;
 double coordenadaY = 0.0;
 int cabezal = 0;
 int carrete = 0;
 String cadenaJSON;
 bool finCadena = false;

 
void JasonRead() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(cadenaJSON);
  // Probar si el "parsing" fue exitoso.
  if (!root.success()) {
    //Serial.println("Fallo el parsing");
    return;
  }
 coordenadaX = root["X"];
 coordenadaY = root["Y"];
 cabezal = root["Head"];
 carrete = root["Stack"];
 //Validacion
 if(coordenadaX > 10.5 )  digitalWrite(13, 1);
 else  digitalWrite(13, 0);
 //retornar informacion o acuse de recibo
  root["X"]= coordenadaX+1;
  root["Y"]= coordenadaY+1;
  root["Head"]= cabezal+1;
  root["Stack"]= carrete+1;
  root.printTo(Serial);
 
 //Serial.println("{\"Status\":\"OK\"}");
}

void setup() {
  pinMode(13,OUTPUT);
  Serial.begin(115200);
  cadenaJSON.reserve(100);

}

void loop() {
  
  if(finCadena){
    finCadena = false;
    JasonRead();
    cadenaJSON = "";
  }
}

void serialEvent(){
while (Serial.available()) {              //Si existen datos seriales, leer a todos
    char CaracterEntrada = Serial.read();   //Leer 1 byte serial recibido
    cadenaJSON += CaracterEntrada;   //Agregar el nuevo char a una cadena String 
    if (CaracterEntrada == '}') {          //Si el char o byte recibido es un fin de linea, activa la bandera
      finCadena = true;                        //Si la bandera finCadena = 1, entonces la transmision esta completa
    }
  }
}


