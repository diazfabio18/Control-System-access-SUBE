# Control System access SUBE
#### Control System for access to different places, using the unique identificator of each SUBE target and Arduino software.

![tarjeta SUBE](/img/sube-tarjeta.jpeg)

 Description
 ===============
 The system of argentinien transport utilies the SUBE target for each people. Therefore, each people have an unique identificator can use in a registry for differets actions. In this case, we used like access control. 
 
 This software have a database with CRUD operators (Create, Read, Update, Delete), so much of manual way like by a RFID lector.

 Also owns two indicators LED (Red and Green), identifier the validity of the user.

 For tests and fails we have a Master Target always is valite and cann't is delete.




History
================
When i was in secondary school in Argentina, the authorities of establishment required a special system for a bathroom recent remodeled back then. With our team of three persons propused make it the software and hardware to install and to demostrate.

Hardware and Software used
====================

#### Software
- Language Arduino, based on __C__ __++__
- Library Ethernet Shield
- Library MFRC522 for use RFID
- Library DB for CRUD
- Library TimerOne for Interruptions
- HTML and CSS basic

#### Hardware
- Base board __Arduino UNO__
- __Ethernet Shield__ board 
- Board feed 12 DC Volts
- RFID target for read
- Comunication cable Ethernet and __USB type B__
- Electroimans for demostration at public
- Box container of acrilyc for demostration


 Terminated proyect
 =================
This proyect was implement through an Base Board Arduino UNO by his low requeriments.

![Proyecto terminado](/img/sube_puerta.jpg)