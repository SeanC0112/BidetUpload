#include "Arduino.h"

const int dirPin = 5;
const int stpPin = 6;

const int buttonRPin = 21;
const int buttonLPin = 20;

float angle = 0;
const int maxAngle = 90;
const int minAngle = -90;

bool left = false;
bool leftSide = false;
bool home = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonRPin, INPUT_PULLUP);
  pinMode(buttonLPin, INPUT_PULLUP);

  pinMode(dirPin, OUTPUT);
  pinMode(stpPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  updateSide();
  changeDir();
  move();
  delay(10);
}

bool rightButton() {
  return !digitalRead(buttonRPin);
}

bool leftButton() {
  return !digitalRead(buttonLPin);
}

bool anyButton() {
  if (leftButton() || rightButton()) Serial.println("button pressed");
  return leftButton() || rightButton();
}

void updateSide() {
  leftSide = (angle <= 0) ? true : false;
}

void changeDir() {
  if((rightButton() && left && !leftButton()) || (leftButton() && !left && !rightButton())) {
    left = !left;
    digitalWrite(dirPin, !left); // check to make sure dirPin and leftSide align

  }
}

void move() {
  if(home) {
    if((leftSide && !left && !leftButton()) || (!leftSide && left && !rightButton())) {
      step();
      Serial.println("homing");
    } else {
      home = !home;
    }
  } else {
    if(anyButton()) {
      if((!left && leftSide) || (left && !leftSide)) {
        home = !home;
        Serial.println("beginning to home");
      }
      if(home || (angle < 90 && angle > -90)) {
        step();
      }
    }
  }
}

void step() {
  Serial.println("step");
  digitalWrite(stpPin, HIGH);
  angle = left ? angle - 0.9 : angle + 0.9;
  delay(10);
  digitalWrite(stpPin, LOW);
}