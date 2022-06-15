// Librería para trabajo con la pantalla
#include <LiquidCrystal.h>

// Inicializamos la pantalla
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Pin out
int buzzer = 2;
int btnA = 5;
int btnB = 4;

// Variables globales
int gameTime = 60 * 30; // Duración por defecto del juego (30 minutos)
int timeA = 0;  // Tiempo acumulado por el equipo Alpha
int timeB = 0;  // Tiempo acumulado por el equipo Bravo
bool dominationA = false; // True si el equipo Alpha está dominando
bool dominationB = false; // True si el equipo Bravo está dominando
int gameType = 0; // Tipo de juego
/*
 * Tipos de juego:
 * 1 - TIEMPO: Durante un tiempo determinado, ver qué equipo domina más
 * 2 - DOMINACION: Ver qué equipo es el primero en conseguir dominar un tiempo determinado
*/

/**
 * String getFormattedTime(int totalSeconds)
 * 
 * Recibe como argumento un entero que es la cantidad de segundos y devuelve un string de la forma "MM:SS"
 */
String getFormattedTime(int totalSeconds){
  // Obtain minutes and seconds from the total amount of seconds
  int minutes = totalSeconds / 60;
  int seconds = totalSeconds - (minutes * 60);

  // Obtain the proper string representation with two digits
  String strMin = minutes < 10 ? "0" + String(minutes) : String(minutes);
  String strSec = seconds < 10 ? "0" + String(seconds) : String(seconds);

  // Return the formmated time string
  return strMin + ":" + strSec;
}

/**
 * void showGameStatus()
 * 
 * Presenta en pantalla el estado actual de los contadores del juego
 */
void showGameStatus() {
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print( getFormattedTime(gameTime) );
  lcd.setCursor(0,1);
  lcd.print( getFormattedTime(timeA) );
  lcd.setCursor(11,1);
  lcd.print( getFormattedTime(timeB) );
}

/**
 * int getUserInput()
 * 
 * Espera a que el usuario presione un botón y devuelve qué botón ha presionado:
 * 1 - BtnA
 * 2 - BtnB
 * 3 - Ambos a la vez
 */
int getUserInput() {
  int userInput = 0;
  while(true) {
    if( digitalRead(btnA) == LOW ) {
      tone(buzzer, 5000, 100);
      // check if it is dual press
      delay(150);
      if( digitalRead(btnB) == LOW ) {
        tone(buzzer, 6000, 100);
        userInput = 3;
        break;
      }
      userInput = 1;
      break;
    }
    if( digitalRead(btnB) == LOW ) {
      tone(buzzer, 5000, 100);
      // check if it is dual press
      delay(150);
      if( digitalRead(btnA) == LOW ) {
        tone(buzzer, 6000, 100);
        userInput = 3;
        break;
      }
      userInput = 2;
      break;
    }
  }
  // Wait for button release
  while(digitalRead(btnA) == LOW || digitalRead(btnB) == LOW) {}
  return userInput;
}

/**
 * void errorTone()
 * 
 * Reproduce un par de tonos graves seguidos en el buzzer que transmiten la idea de que ha ocurrido un error
 */
void errorTone() {
  tone(buzzer, 250, 50);
  delay(100);
  tone(buzzer, 250, 50);
  delay(100);
}

/**
 * void initialSetup()
 * 
 * Gestiona el setup inicial donde se establece el tipo de partida y el tiempo que va a durar
 */
void initialSetup() {

  // Request user to set the game type
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("A: TIEMPO");
  lcd.setCursor(0,1);
  lcd.print("B: DOMINACION");
  // User input is game type
  int userInput = 0;
  while(userInput != 1 && userInput != 2) {
    userInput = getUserInput();
  }
  gameType = userInput;

  // Request user to set game time
  lcd.clear();
  lcd.setCursor(0,0);
  String msg = gameType == 1 ? "TIEMPO MAX" : "OBJETIVO";
  lcd.print(msg);

  bool setupDone = false;
  while(!setupDone) {
    lcd.setCursor(0,1);
    lcd.print( getFormattedTime(gameTime) );

    int userInput = getUserInput();
    switch (userInput) {
      case 1:
        if (gameTime > 300)
          gameTime -= 300;
        else
          errorTone();
        break;
      case 2:
        if (gameTime < 30000)
          gameTime += 300;
        else
          errorTone();
        break;
      case 3:
        setupDone = true;
        break;
      default:
        break;
    }
  }
}

/**
 * bool isGameOver()
 * 
 * Comprueba el estado actual del juego y el tipo de juego para definir si ya se ha terminado.
 * Devuelve true si el juego ha terminado.
 */
bool isGameOver() {
  
  if(gameType == 1) {
    if ( timeA > gameTime + timeB || timeB > gameTime + timeA ) return true;
  } else if (gameType == 2) {
    if ( timeA >= gameTime || timeB >= gameTime ) return true;
  }
  
  return false;
}

/**
 * void showGameResult()
 * 
 * Pita fin de partida y presenta el equipo ganador en pantalla
 */
void showGameResult() {
  String winner = timeA > timeB ? "A" : "B";
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("FIN DE PARTIDA");
  lcd.setCursor(3,1);
  lcd.print("GANADOR: " + winner);
  
  while(true) {
    tone(buzzer, 2000);
    delay(500);
    noTone(buzzer);
    tone(buzzer, 4000);
    delay(500);
    noTone(buzzer);
  }
}

/**
 * bool conqer(int teamButton)
 * 
 * Proceso de conquista del punto. Devuelve true si el punto fue conquistado
 */
bool conqer(int teamButton) {
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("CONQUISTANDO");
  int progress = 0;
  while (digitalRead(teamButton) == LOW) {
    lcd.setCursor(progress,1);
    lcd.print("*");
    tone(buzzer, 500, 100);
    delay(400);
    progress++;
    if (progress == 16)
      return true;
  }
  return false;
}

/**
 * void setup()
 * 
 * Punto de entrada al programa
 */
void setup() {
  
  // Pin del buzzer como salida
  pinMode(buzzer, OUTPUT);

  // Se configuran con resistores de pull-up internos los pines por donde se atiende a los botones
  pinMode(btnA, INPUT_PULLUP);
  pinMode(btnB, INPUT_PULLUP);
  
  // Inicialización de LCD
  lcd.begin(16, 2);

  // Tres beeps para anunciar que el dispositivo se ha encendido
  for(int i = 0; i < 3; i++) {
    tone(buzzer, 5000, 50);
    delay(100);
  }

  // Cargamos el menú inicial
  initialSetup();
}

/**
 * void loop()
 * 
 * Ciclo eterno de ejecución
 */
void loop() {

  // Comprobar si se está presionando algún botón
  if( digitalRead(btnA) == LOW ) {
    if ( dominationA ) {
      tone(buzzer, 100, 200);
    } else if ( conqer(btnA) ) {
      dominationA = true;
      dominationB = false;
      tone(buzzer, 100, 200);
    }
  } else if ( digitalRead(btnB) == LOW ) {
    if ( dominationB ) {
      tone(buzzer, 100, 200);
    } else if ( conqer(btnB) ) {
      dominationA = false;
      dominationB = true;
      tone(buzzer, 100, 200);
    }
  }

  // Actualiza los contadores
  if (gameType == 1) gameTime--;  // El tiempo del juego solo se decrementa si el tipo de juego es TIEMPO
  if (dominationA) timeA++;
  if (dominationB) timeB++;

  // Mostrar el estado de los contadores en pantalla
  showGameStatus();

  // Comprobar si el juego ha terminado
  if( isGameOver() ) {
    showGameResult();
  }

  // Espera un segundo para la próxima iteración
  delay(1000);
  
}
