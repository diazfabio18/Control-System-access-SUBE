
#ifndef InterrupLED_h
  #define Tiraled 4 // define que el led estara en el pin 4
  #define TARJETA 1
  
  #ifndef Arduino_H
    #include <Arduino.h> //remplaza a wprogram.h
  #endif

  #ifndef DB_H
    #include "DB.h"
  #endif
  
#endif

struct SUBEReg {
  uint16_t DNI;
  byte ID[1][4];
} subereg;

void Interrup();

void borrarEeprom();

int conversion(char *s);

void BorrarUno(int DniBorrar); //Prototipo de la funcion BorrarUno hecha para borrar un registro a partir del DNI

void imprimeClave(byte *serial);

boolean chekaKey(byte *serial);

boolean esIgual(byte *key, byte *serial);

void Agregar(byte *serial, uint16_t *DNI);
