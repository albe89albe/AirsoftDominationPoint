#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD... https://sminghub.github.io/Sming/api/classLiquidCrystal__I2C.html

// pin out
int buzzer = 2;
int btnA = 8;
int btnB = 7;
int led = 13;

// global variables
int gameTime = 60 * 30; // Game duration in seconds (30 minutes)
int timeA = 0;  // Accumulated time of team alpha
int timeB = 0;  // Accumulated time of team bravo
bool dominationA = false; // True if team alpha is dominating
bool dominationB = false; // True if team bravo is dominating
int gameType = 0; // Type of game
/*
 * Game types:
 * 1 - Fixed duration: given a maximum game lenght, each team fight to have the more time
 * 2 - Domain objective: each team fight to reach a given time goal
*/

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,20,4) for 20x4 LCD.

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

void showGameStatus() {
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print( getFormattedTime(gameTime) );
  lcd.setCursor(0,1);
  lcd.print( getFormattedTime(timeA) );
  lcd.setCursor(11,1);
  lcd.print( getFormattedTime(timeB) );
}

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

void initialSetup(){
  
  lcd.backlight();

  // Request user to set the game type
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("A: Duracion fija");
  lcd.setCursor(0,1);
  lcd.print("B: Dominacion");
  // User input is game type
  int userInput = 0;
  while(userInput != 1 && userInput != 2) {
    userInput = getUserInput();
  }
  gameType = userInput;

  // Request user to set game time
  lcd.clear();
  lcd.setCursor(0,0);
  String msg = gameType == 1 ? "Tiempo max" : "Objetivo";
  lcd.print(msg);

  bool setupDone = false;
  while(!setupDone) {
    lcd.setCursor(0,1);
    lcd.print( getFormattedTime(gameTime) );

    int userInput = getUserInput();
    switch (userInput) {
      case 1:
        gameTime -= 300;
        break;
      case 2:
        gameTime += 300;
        break;
      case 3:
        setupDone = true;
        break;
      default:
        break;
    }
  }
}

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(btnA, INPUT_PULLUP);
  pinMode(btnB, INPUT_PULLUP);
  pinMode(led, OUTPUT);

  // Initiate the LCD:
  lcd.init();
  lcd.backlight();
  initialSetup();
  
}

void loop() {

  if( digitalRead(btnA) == LOW ) {
    dominationA = true;
    dominationB = false;
    tone(buzzer, 250, 250);
    digitalWrite(led, HIGH);
  } else if ( digitalRead(btnB) == LOW ) {
    dominationA = false;
    dominationB = true;
    tone(buzzer, 250, 250);
  }

  // At the end of each loop wait one second and update the counters
  delay(1000);
  gameTime--;
  if (dominationA) timeA++;
  if (dominationB) timeB++;
  showGameStatus();
}
