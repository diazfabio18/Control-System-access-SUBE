/*
  Arduino with Ethernet Shield
*/
//#include <SPI.h>
#include "MFRC522.h"
#define ENC28J60_CONTROL_CS 2 // Define que el Chip Select para la lan es pin 2
//#include <UIPEthernet.h>
#include <string.h>

//------------------------------------------------base de datos define----------
#include <EEPROM.h> //Libreria para manejo de la EEPROM
#include <TimerOne.h> //Libreria para manejo de interrupciones

#include "InterrupLED.h"

#define MY_TBL 1

DB db; // crea la base de datos de clase EEPROM DB
///---------------------------------------------fin base de datos----
#define buzzer 3


//------------------------------------------- RFID --- INICIO definiciones Globales-----------------------------------
MFRC522 nfc(SAD, RST);
//--------------------------------------------RFID ---- FIN---------------------------------


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // while (!Serial) {
  //  ; // wait for serial port to connect. Needed for Leonardo only
  //}
  pinMode(Tiraled, OUTPUT);
  Timer1.initialize(30000000); //La libreía TimerOne crea automaticamente el objeto Timer1 sin necesidad de declararlo, y este es programado con un intervalo de 30 segundos(en micro segundos)
  Timer1.attachInterrupt(Interrup);

  //---------------------------------------------DB------------------
  db.open(MY_TBL);

  //Check if database is empty
  if (db.nRecs() == 0)
  { // verifica si la base de datos esta vacia
    // crea una base de datos con la tarjeta maestra
    db.create(MY_TBL, sizeof(subereg));
    Serial.println (F("Creando llave maestra"));
    subereg.ID[0][0] = 0x1B;
    subereg.ID[0][1] = 0xF3;
    subereg.ID[0][2] = 0xD5;
    subereg.ID[0][3] = 0x8F;
    subereg.DNI = 1194;
    db.append(DB_REC subereg);
  };
  //---------------------------------------------DB Fin-------------------

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  //Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  //---------------------------------------------- RFID inicio setup
  //Declaro los pines y el buzzer
  pinMode(buzzer, OUTPUT);
  pinMode(ledPinAbierto  , OUTPUT);
  pinMode(ledPinCerrado, OUTPUT);
  SPI.begin();
  //Serial.println(F("BUSCANDO MFRC522."));
  
  //nfc.begin();
  nfc.PCD_Init();

  /////////////////---------------------------
  /////////////////--------------------------- byte version = nfc.getFirmwareVersion();
/********************  if (! version) {
    Serial.print(F("NO SE ENCONTRO MFRC522 "));
    while (1); //halt
  }  ***********************/

  Serial.print(F("BUSCANDO CHIP MFRC522 "));
  Serial.print(F("FIRMWARE VERSION. 0x"));
  //////////////////////////--------------------------- Serial.print(version, HEX);
  Serial.println(".");
}
//-----------------------------------RFID fin de Setup------------------------------

void loop() {
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
          //Serial.print(c);
        }

        //if HTTP request has ended
        if (c == '\n') {
          Serial.println(readString); //print to serial monitor for debuging

          void htmlPage();

          for (int registro = 1; registro <= db.nRecs(); registro++)
          { // recorre cada registro de la base de datos de tarjetas
            db.read(registro, DB_REC subereg);
            client.print(F("<tr><td>"));
            client.print(registro);
            client.print(F("</td><td>"));
            for (byte i = 0; i < 4; i++)
            {
              client.print(subereg.ID[0][i], HEX);
              client.print(" ");
            }
            client.print(F("</td><td>"));
            client.print(subereg.DNI);
            client.println(F("</td></tr>"));
          }
          client.println(F("</table>"));
          // Las líneas siguientes son las que pasan la referencia al Arduino.
          //  client.println(F("<a href=\"/?boton1on\"\">On LED</a>"));
          //  client.println(F("<a href=\"/?boton1off\"\">Off LED</a><br />"));
          client.println(F("<br />"));
          //  client.println(F("<p> presionando los botones se enciende o apaga el pin 4 </p>"));
          //           client.println(F("</BODY>"));
          //           client.println(F("</HTML>"));
          // -----------  Fin de la página HTML y CCS ------------

          uint16_t encontrado, encontrado2; String tarj;
          uint16_t dniRecibido; byte tarjetaRec[4]; //para guardar los datos obtenidos por el metodo get del servidor

          client.println(F("</BODY>"));
          client.println(F("</HTML>"));
          delay(1);
          //Deteniendo el cliente web
          //client.stop();
          //controla la respuesta de la web
          if (readString.indexOf(F("DNI=")) > 0) {
            //Aca recibe los codigos proporcionados por el metodo GET, y después los envía a la funcion Agregar
            encontrado = readString.indexOf("DNI="); //si encuentra la cadena DNI= , indexOf nos dice que posicion está
            dniRecibido = readString.substring(encontrado + 4, encontrado + 8).toInt(); //extraemos la subcadena entre 4 y 8 caracteres a partir de donde comienza "DNI="
            encontrado2 = readString.indexOf("TRJ=");
            tarj = readString.substring(encontrado2 + 4, encontrado2 + 12);

            String trj1 = tarj.substring(0, 2); //Serial.println(trj1);
            String trj2 = tarj.substring(2, 4); //Serial.println(trj2);
            String trj3 = tarj.substring(4, 6); //Serial.println(trj3);
            String trj4 = tarj.substring(6, 8); //Serial.println(trj4);

            tarjetaRec[0] = conversion((char *)trj1.c_str()); //Serial.println(tarjetaRec[0],HEX);
            tarjetaRec[1] = conversion((char *)trj2.c_str()); //Serial.println(tarjetaRec[1],HEX);
            tarjetaRec[2] = conversion((char *)trj3.c_str()); //Serial.println(tarjetaRec[2],HEX);
            tarjetaRec[3] = conversion((char *)trj4.c_str()); //Serial.println(tarjetaRec[3],HEX);

            Agregar(tarjetaRec, &dniRecibido); // Se llama a la funcion Agregar, pasandole el vector tarjetaRec, y la direccion de la variable del DNI a agregar
            client.println(F("<meta http-equiv='refresh' content='2; url=http://192.168.0.6/'>"));   // Redirecciona a la pagina principal despues de 2 segundos
          }
          if (readString.indexOf(F("BORRAR=")) > 0) { //Recibe mediante el metodo Get el DNI a borrar

            encontrado = readString.indexOf("BORRAR="); int DniBorrar; //Busca la posicion de la constante 'Borrar=' para recortar el string
            if (encontrado >= 0) DniBorrar = readString.substring(encontrado + 7, encontrado + 11).toInt(); //Recorta el string mediante substring, despues este es convertido a entero, y es almacenado en DniBorrar
            Serial.println(DniBorrar); //Muestra en el Serial Monitor el dni ingresado en el servidor para borrar
            BorrarUno(DniBorrar);  //Llama a la funcion BorrarUno y le pasa como argumento el contenido de la variable entera DniBorrar
            // redirecciona();//client.println(F("<meta http-equiv=\"Refresh\" content=\"5; url=http://192.168.0.6\">"));  //redirecciona a la pagina principal despues de 2 segundos
            client.println(F("<meta http-equiv=\"refresh\" content=\"2; url=http://192.168.0.6/\">"));   // Redirecciona a la pagina principal despues de 2 segundos
          }
          if (readString.indexOf(F("/borrartodo")) > 0) {
            borrarEeprom(); // si se envia el comando borrar todo desde el navegador limpia la base de datos.
            client.println(F("<meta http-equiv=\"refresh\" content=\"2; url=http://192.168.0.6/\">"));   // Redirecciona a la pagina principal despues de 2 segundos
          }
          client.stop();
          //limpia el string para la proxima lectura web
          readString = "";
        }
      }
    }
  }
  ////-------------------------------------------------------------RFID Inicio Loop----------
  byte status;
  byte data[MAX_LEN];
  byte serial[4];
  byte identificacion[4];
  boolean Abierto = false;
  digitalWrite(ledPinAbierto, Abierto);
  //digitalWrite(ledPinCerrado, !Abierto);   /*led rojo prendido*/
////////////////////////////////-----------------------------------  status = nfc.requestTag(MF1_REQIDL, data);

  bool sonido = true;

  
  digitalWrite(Tiraled, LOW); // aquí es donde la tira led prende y apaga permanentemente sin interrupciones
  delay(500);
  digitalWrite(Tiraled, HIGH);
  //--------------------------------------------------

  if (status == MI_OK) {
    Serial.println(F("se detecto una tarjeta"));
///////////////////------------------------------    status = nfc.antiCollision(data);
    memcpy(serial, data, 4);
    //Serial.println(db.nRecs());

    // imprimeClave(serial);
    for (int registro = 1; (registro <= db.nRecs()) && (Abierto == false); registro++)
    { // recorre cada registro de la base de datos de tarjetas
      db.read(registro, DB_REC subereg);
      //memcpy(identificacion, subereg.ID[0], 4);
      // imprimeClave(identificacion);

      if (esIgual(serial, subereg.ID[0]))
      {
        if (registro == 1)
        {
          if (agregar == false)
          {
            Serial.println(F("entrando en modo Agregado"));
            agregar = true;
          } else {
            agregar = false;
            Serial.println(F("saliendo modo Agregado"));
          }
        } else
        {
          Serial.println(F("AUTORIZADO"));
          Serial.print(F("DNI "));
          Serial.print(subereg.DNI);
          Serial.print(F(" "));
          imprimeClave(serial);
          Serial.println();
          Abierto = true;

          if (sonido == true) {
            tone(buzzer, 660, 500); //emite sonido
            delay(50);
            tone(buzzer, 100, 50);
            delay(50);
            tone(buzzer, 660, 500); //emite sonido

          }
        }
      } else
      {
        Abierto = false;
      }
    }
    if (Abierto == false)
    {
      Serial.print(F("NO AUTORIZADO "));
      imprimeClave(serial);
      Serial.println(F(""));
      digitalWrite(ledPinCerrado, HIGH);
      if (sonido == true) {
        tone(buzzer, 70, 1000); //emite sonido
      }
      //buzzer con opcion NO

      if (agregar == true)
      {
        Agregar(serial, &subereg.DNI); /*Aquí llama a la función Agregar en vez de hacer el agregado desarrollado en el if*/
        agregar = false;
      }
    }
////////////////////////////---------------------------------    nfc.haltTag(); // limpia registros de la lectora
    digitalWrite(ledPinAbierto, Abierto); // enciende o apaga el led de abierto segun corresponda
    //digitalWrite(ledPinCerrado, !Abierto); // enciende o apaga el led de cerrado segun corresponda
    delay(1500); //espera 1 segundo y medio
    analogWrite(buzzer, 0); //deja de emitir
    digitalWrite(ledPinCerrado, LOW);
  }
  delay(500);

  //-------------------------------------------------------RFID Fin Loop------------
}
