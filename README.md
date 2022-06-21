# Punto de dominio

*Hola Legionario!*
:smiley:

Aquí encontrarás la documentación para poder hacerte un punto de dominación simple con Arduino. Este repositorio contiene:
- La lista de componentes que necesitarás
- Las forma en la que deben conectarse los componentes
- El código que debe subirse al Arduino

El proyecto fue realizado con un Arduino Duemilanove, pero es de esperar que funcione exactamente igual en un Arduino UNO.

Si crees que algo se puede mejorar, puedes decírmelo en un issue de este repositorio, o a través de mi [Instagram](https://www.instagram.com/el_legionario_airsoft/).

En general si te ha parecido útil, o quieres alegrarme el día, puedes pasar por mi canal de [YouTube](https://www.youtube.com/c/ElLegionario), suscribirte y dejarme algún comentario de esos que dan ganas de seguir haciendo estas cosas :blush:

## Código

Puedes [descargarte el código](https://github.com/albe89albe/AirsoftDominationPoint/archive/refs/heads/main.zip) o [darle un vistazo](AirsoftDominationPoint.ino), tiene los comentarios en español y está hecho de una forma que considero bastante legible. El código recibirá mejoras, pero siempre contando con que el hardware no cambie, así que cuando te quieras montar el proyecto, simplemente descárgate la última versión, o si ya lo tienes montado con una versión anterior, siempre será seguro actualizar a la última versión. La compatibilidad con versiones anteriores se mantendrá siempre.

Usa el IDE oficial para subir el código `AirsoftDominationPoint.ino` a tu Arduino. Recuerda que siempre te lo puedes descargar gratuitamente desde la [web de Arduino](https://www.arduino.cc/en/software).

## Lista de componentes

1. Arduino (UNO o Duemilanove)
2. LCD 1602
3. Botones (x2)
4. Buzzer
5. Interruptor simple
6. Resistor de 220 &#x2126;
7. Potenciómetro de 10 k&#x2126;

No contaré cables, placas, herramientas y consumibles, solo componentes y sus conexiones.

## Conexiones
Esta será la faena complicada para montar este proyecto. Haz uso como entiendas del cableado y de las placas u otras cosas que tengas para conseguir las conexiones que se describen a continuación. Recuerda proteger al máximo de la posibilidad de que terminales que no se deban tocar, se toquen. o sea, asegúrate de usar termoretráctil o cinta aislante donde sea necesario. Si dos cables que no se deben tocar, se tocan, puede no funcionar bien, pero puedes también cargarte algo.

A continuación tienes a qué debe estar conectado cada terminal desde el punto de vista de cada componente:
### Desde el Arduino
Solamente se usarán los pines de I/O digitales
```
Aref -
 GND -
  13 - Rs (LCD)
  12 - E (LCD)
  11 - D4 (LCD)
  10 - D5 (LCD)
   9 - D6 (LCD)
   8 - D7 (LCD)
   7 -
   6 -
   5 - BtnA
   4 - BtnB
   3 -
   2 - P (Buzzer)
   1 -
```
### Desde la pantalla
```
Vss - GND
Vdd - 5V
 Vo - B (Potenciómetro)
 Rs - 13 (Arduino)
 Rw - GND
  E - 12 (Arduino)
 D0 -
 D1 -
 D2 -
 D3 -
 D4 - 11 (Arduino)
 D5 - 10 (Arduino)
 D6 -  9 (Arduino)
 D7 -  8 (Arduino)
  A - Resistor - 5V
  K - GND
```
### Desde el buzzer
En realidad no importa qué cable va donde, solo que uno esté a masa y el otro al pin del Arduino
```
N - GND
P - 2 (Arduino)
```
### Desde el potenciómetro
El terminal B o "base" es en el que la resistencia varía cuando se manipula el potenciómetro, entre los terminales A y B debe haber 10 k&#x2126;
```
A - 5V
B - Vo (LCD)
C - GND
```
### Desde el resistor
Simplemente nombre "1" y "2" a los terminales porque es indiferente
```
1 - A (LCD)
2 - 5V
```
### Desde BtnA
Simplemente nombre "1" y "2" a los terminales porque es indiferente
```
1 - GND
2 - 5 (Arduino)
```
### Desde BtnB
Simplemente nombre "1" y "2" a los terminales porque es indiferente
```
1 - GND
2 - 4 (Arduino)
```

Y eso es todo!
--------------

*Nos vemos en el campo de batalla!* 
:stuck_out_tongue_winking_eye: