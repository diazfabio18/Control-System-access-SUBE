
#ifndef InterrupLED_h
  #define Tiraled 4 // define que el led estara en el pin 4
  #define TARJETA 1
  
  #ifndef Arduino_H
    #include <Arduino.h> //remplaza a wprogram.h
  #endif

  #ifndef SPI_H
    #include <SPI.h>
  #endif

  #ifndef UIPEthernet_H
    #include <UIPEthernet.h>
  #endif

  #ifndef DB_H
    #include "DB.h"
  #endif
  
#endif

struct SUBEReg {
  uint16_t DNI; //DNI de la persona
  byte ID[1][4]; //Identificator of Sube target
} subereg;

//-------------------------DEFINES ----------------------------//
#define SAD 10
#define RST 9
#define ledPinAbierto    5
#define ledPinCerrado 6
#define LLAVE 1

#define MAX_LEN 256
#define MI_OK true

boolean agregar = false;
//----------------------------------------------------------------//

//------------------- Global variables declaration --------------- //

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //physical mac address
byte ip[] = { 192, 168, 0, 6 };                      // ip in lan (that's what you need to use in your browser. ("192.168.1.178")
byte gateway[] = { 192, 168, 0, 1 };                   // internet access via router
byte subnet[] = { 255, 255, 255, 0 };                  //subnet mask
EthernetServer server(80);                             //server port
String readString;

//------------------ End global variables declaration ------------- //

void Interrup();

void borrarEeprom();

int conversion(char *s);

void BorrarUno(int DniBorrar); //Prototipo de la funcion BorrarUno hecha para borrar un registro a partir del DNI

void imprimeClave(byte *serial);

boolean chekaKey(byte *serial);

boolean esIgual(byte *key, byte *serial);

void Agregar(byte *serial, uint16_t *DNI);

void htmlPage();

// rfid

/* Aignación de pines para la lectora de tarjetas.
  Pins  SPI    UNO
  1 (NSS) SAD (SS)   10
  2       SCK        13
  3       MOSI       11
  4       MISO       12
  5       IRQ        *
  6       GND       GND
  7       RST        5
  8      +3.3V (VCC) 3V3
  Not needed
  1 on ICPS header
*/

//--------------------------------------------------declaracion de variables necesarias para el efecto especial de Tiraled cada un minuto
  //  digitalWrite(Tiraled,LOW);
  //
  //  unsigned long Millis_previo=0;
  //  long tiempo=1000000;   //el tiempo que se quiere hacer el efecto especial es de 1 minuto.
  //  unsigned long Millis_actual= millis();
  //
  //  if(Millis_actual - Millis_previo > tiempo) {
  //    // guarda el ultimo tiempo, para la proxima vez
  //    Millis_previo = Millis_actual;
  //
  //    for(short i=0;i<20;i++){ // 20 es el numero de veces en que Tiraled se prende y apaga
  //      digitalWrite(Tiraled, HIGH);
  //      delay(100);
  //      digitalWrite(Tiraled, LOW);
  //      delay(100);
  //    }
  //  }

  //CLAVE DE LA TAJETA
//byte Autorizado[TARJETA][4] = {{0xD7, 0xE9, 0x7E, 0xB5, }};
// CLAVE DEL LLAVERO
//byte Autorizado2[LLAVE][4] = {{0x83, 0x10, 0xF6, 0xE2, }};


