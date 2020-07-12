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

void htmlPage(){
  client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/html"));
          client.println();
          client.println(F("<HTML>"));
          client.println(F("<HEAD>"));
          client.println(F("<style type='text/css'> h1 { color: green;  font-family: Arial;   font-size: large;  } p { color: gray; font-family: Verdana; font-size: medium; } a { text-decoration:none; width:75px; height:50px; border-color:black; border-top:2px solid; border-bottom:2px solid; border-right:2px solid; border-left:2px solid; border-radius:10px 10px 10px; -o-border-radius:10px 10px 10px; -webkit-border-radius:10px 10px 10px; font-family:'Trebuchet MS',Arial, Helvetica, sans-serif; -moz-border-radius:10px 10px 10px; background-color:#293F5E; padding:8px; text-align:center; } a:link {color:white;} a:active {color:white;} a:hover {color:white;} a:visited {color:white;} </style>" ));
          client.println(F("<TITLE>EEST 1</TITLE>")); //Nombre que aparece en la barra de título o pestaña del  navegador.
          client.println(F("</HEAD>"));
          client.println(F("<BODY>"));
          client.println(F("<H1>EEST 1 \"Manuel Belgrano\"</H1>")); //Titulo de la página.
          client.println(F("<HEAD>"));

          client.println(F("<fieldset><legend>Agregado</legend><FORM METHOD=GET/ACTION=''> <div><label>DNI</label> <INPUT NAME='DNI' maxlength='4' placeholder='Ultimos 4 digitos'></div><BR> <div><label>TARJETA</label> <INPUT NAME='TRJ' maxlength='8' placeholder='Codigo a agregar'></div>  <BR> <div><INPUT class='btn' TYPE=SUBMIT value='Agregar'></div> </FORM></fieldset>"));
          client.println(F("<fieldset><legend>Borrado</legend><FORM METHOD=GET/ACTION=''> <div><label>Dni a borrar</label> <INPUT NAME='BORRAR' maxlength='4' placeholder='Ultimos 4 digitos'></div> <BR> <div><INPUT class='btn' TYPE=SUBMIT VALUE='Borrar'></div> </FORM></fieldset>"));
          client.println(F("<style type='text/css'>fieldset{float: left; width: 50%;} label {display: block;margin: .5em 0 0 0;}  div{ margin: .4em 0;} div label{width:25%; float:left;} btn {display: block;margin: 1em 0;} </style>"));

          //client.println(F("<hr />"));
          //client.println(F("<br />"));
          //client.println(F("<br />"));
          client.println(F("<br />"));

          client.println(F("<center><H2> Tarjetas SUBE Autorizadas</H2></center>"));
          
          client.println(F("<table class='normal'  style='text-align:center; margin:0 auto; font-size:20px;' summary='Tabla generica'>"));
          client.println(F("<tr><th scope='col'>  Registro  </th>"));
          client.println(F("<th scope='col'>  Codigo de Tarjeta  </th>"));
          client.println(F("<th scope='col'>  Dni  </th></tr>"));
}

/*********************************/