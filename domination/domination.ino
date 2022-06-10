#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD... https://sminghub.github.io/Sming/api/classLiquidCrystal__I2C.html

int buzzer = 2;
int btnA = 8;
int btnB = 7;
int led = 13;

int gameTimeLeft = 60 * 30; // Game duration in seconds (30 minutes)
int timeA = 0;  // Accumulated time of team alpha
int timeB = 0;  // Accumulated time of team bravo

bool dominationA = false; // True if team alpha is dominating
bool dominationB = false; // True if team bravo is dominating

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,20,4) for 20x4 LCD.

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(btnA, INPUT_PULLUP);
  pinMode(btnB, INPUT_PULLUP);
  pinMode(led, OUTPUT);

  // Initiate the LCD:
  lcd.init();
  lcd.backlight();
  showGameStatus();
  
}

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
  lcd.print( getFormattedTime(gameTimeLeft) );
  lcd.setCursor(0,1);
  lcd.print( getFormattedTime(timeA) );
  lcd.setCursor(11,1);
  lcd.print( getFormattedTime(timeB) );
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
  gameTimeLeft--;
  if (dominationA) timeA++;
  if (dominationB) timeB++;
  showGameStatus();
}
