#include "interrupLED.h"

void Interrup() {
  for (short i = 0; i < 20; i++) { // 20 es el numero de veces en que Tiraled se prende y apaga
    digitalWrite(Tiraled, HIGH);
    delay(100);
    digitalWrite(Tiraled, LOW);
    delay(100);
  }
}

void borrarEeprom() {
  int registro = 0;
  while (db.nRecs() != 0)
  { // recorre cada registro de la base de datos de forma descendente, para que no trabaje el bucle interno de deleteRec
    db.read(registro, DB_REC subereg);
    db.deleteRec(registro);
    registro++;
  }
  Serial.println("Base de Datos Borrada totalmente.");
}

/*******************/
void BorrarUno(int DniBorrar) 
{
  bool borra = false; //variable booleana que confirma si el registro fue borrado
  for (int registro = 1; registro <= db.nRecs(); registro++)
  { // recorre cada registro de la base de datos, comparando el Dni de cada registro con el Dni enviado por
    db.read(registro, DB_REC subereg);
    if (subereg.DNI == DniBorrar) {  // Compara si el Dni del registro X es igual que el Dni enviado por el servidor para borrar
      db.deleteRec(registro);
      borra = true;
      break;   //para salir del bucle for una vez que encuentre por lo menos un Dni igual a DniBorrar
    }
  }
  if (!borra) {   //Si la variable borrar es 0 o Falso, entonces salta un mensaje de error
    Serial.print("Error, el Dni ");
    Serial.print(DniBorrar);
    Serial.println(" no fue encontrado.");
  } else {   //en cambio, si es 1 o True, saldra un mensaje diciendo que fue borrado
    Serial.println("El Dni fue borrado.");
  }
}
/*******************/


int conversion(char *s) { //transforma los strings convertidos del string recibido por servidor, en hexadecimal
  int x = 0;
  for (;;) {
    char c = *s;
    if (c >= '0' && c <= '9') {
      x *= 16;
      x += c - '0';
    } else if (c >= 'A' && c <= 'F') {
      x *= 16;
      x += (c - 'A') + 10;
    }
    else break;
    s++;
  }
  return x;
}

/******************/
void imprimeClave(byte *serial) {
  Serial.print(F("CLAVE: "));
  for (int i = 0; i < 4; i++) {
    Serial.print(serial[i], HEX);
    Serial.print(" ");
  }
}
/**********************/

/************************/
boolean chekaKey(byte *serial) {
  for (int registro = 1; registro <= db.nRecs(); registro++)
  {
    db.read(registro, DB_REC subereg);
    for (byte i = 0; i < TARJETA; i++)
    {
      if (esIgual(serial, subereg.ID[i]))
        return true;
    }
    return false;
  }
}
/************************/

boolean esIgual(byte *key, byte *serial) {
  for (int i = 0; i < 4; i++) {
    if (key[i] != serial[i]) {
      return false;
    }
  }
  return true;
}

/****************************/

void Agregar(byte *serial, uint16_t *DNI) {  /*Recibe el serial(el codigo), y el DNI*/
  Serial.println(F("Agregando a la base de datos tarjeta ;"));
  imprimeClave(serial);
  memcpy(subereg.ID[0], serial, 4);
  subereg.DNI = *DNI;
  db.append(DB_REC subereg);
}

/******************************/
