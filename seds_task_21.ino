// C++ code
//
#include <Adafruit_LiquidCrystal.h>

int AnchorState = 0;

int ShipCondition = 0;

int DangerTimer = 0;

int LastButtonState = 0;

Adafruit_LiquidCrystal lcd_1(0);

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

void setup()
{
  lcd_1.begin(16, 2);
  pinMode(2, INPUT);
  pinMode(A0, INPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop()
{
  if (digitalRead(2) == HIGH) {
    if (AnchorState == 0) {
      AnchorState = 1;
    } else {
      AnchorState = 0;
      delay(200); // Wait for 200 millisecond(s)
    }
  }
  if (ShipCondition == 0) {
    if (analogRead(A0) < 200) {
      ShipCondition = 1;
    }
    if (0.01723 * readUltrasonicDistance(8, 9) < 100) {
      ShipCondition = 2;
    }
  } else {
  }
  if (ShipCondition == 3) {
    lcd_1.clear();
    lcd_1.print("SHIP WRECKED");
    noTone(11);
    digitalWrite(13, LOW);
  } else {
    if (AnchorState == 1) {
      lcd_1.clear();
      lcd_1.print("ANCHOR DROPPED");
      DangerTimer = 0;
      ShipCondition = 0;
      noTone(11);
      digitalWrite(13, LOW);
    } else {
      if (ShipCondition == 1) {
        if (analogRead(A0) < 200) {
          lcd_1.clear();
          lcd_1.print("STORM WARNING");
          digitalWrite(13, HIGH);
          delay(500); // Wait for 500 millisecond(s)
          digitalWrite(13, LOW);
          delay(500); // Wait for 500 millisecond(s)
          DangerTimer += 1;
          if (DangerTimer >= 5) {
            ShipCondition = 3;
          }
        } else {
          ShipCondition = 0;
          DangerTimer = 0;
        }
      } else {
        if (ShipCondition == 2) {
          if (0.01723 * readUltrasonicDistance(8, 9) < 100) {
            lcd_1.clear();
            lcd_1.print("CHARYBDIS");
            tone(11, 523, 1000); // play tone 60 (C5 = 523 Hz)
            DangerTimer += 1;
            if (DangerTimer >= 5) {
              ShipCondition = 3;
            }
          } else {
            ShipCondition = 0;
            DangerTimer = 0;
          }
        } else {
          lcd_1.clear();
          lcd_1.print("OPEN SEA");
          DangerTimer = 0;
          noTone(11);
          digitalWrite(13, LOW);
        }
      }
    }
  }
}