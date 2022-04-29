#include <Wire.h>
#include "Mouse.h"
#include "SparkFun_Qwiic_Joystick_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_joystick
JOYSTICK joystick;  // Create instance of this object

// Charactaristic of a joystick
int xCenter = 517;
int yCenter = 522;

// parameters for reading the joystick:
int range = 32;         // output range of X or Y movement
int responseDelay = 10;  // response delay of the mouse, in ms
int center = range / 2;

const int rightButtonPin = 7;
const int leftButtonPin = 8;
int lastRightState = HIGH;
int lastLeftState = HIGH;
int lastMiddleState = HIGH;

void setup() {
  pinMode(rightButtonPin, INPUT);
  pinMode(leftButtonPin, INPUT);
  delay(1000);
  Serial.begin(9600);
  if (joystick.begin() == false)
  {
    Serial.println("Joystick does not appear to be connected. Please check wiring. Freezing...");
    while (1);
  }
  Mouse.begin();
}

void loop() {
  // Button
  int rightButtonState = digitalRead(rightButtonPin);
  int leftButtonState = digitalRead(leftButtonPin);
  int middleButtonState = joystick.getButton();

  if (rightButtonState == LOW && lastRightState == HIGH) {
    //    Serial.println("RIGHT CLICK");
    Mouse.press(MOUSE_RIGHT);
  }
  if (rightButtonState == HIGH && lastRightState == LOW) {
    //    Serial.println("RIGHT RELEASE");
    Mouse.release(MOUSE_RIGHT);
  }

  if (leftButtonState == LOW && lastLeftState == HIGH) {
    //    Serial.println("LEFT CLICK");
    Mouse.press(MOUSE_LEFT);
  }
  if (leftButtonState == HIGH && lastLeftState == LOW) {
    //    Serial.println("LEFT RELEASE");
    Mouse.release(MOUSE_LEFT);
  }

  if (middleButtonState == LOW && lastMiddleState == HIGH) {
    //    Serial.println("MIDDLE CLICK");
    Mouse.press(MOUSE_MIDDLE);
  }
  if (middleButtonState == HIGH && lastMiddleState == LOW) {
    //    Serial.println("MIDDLE RELEASE");
    Mouse.release(MOUSE_MIDDLE);
  }
  lastRightState = rightButtonState;
  lastLeftState = leftButtonState;
  lastMiddleState = middleButtonState;

  // X: high -> right
  //    low  -> left
  // Y: high -> down
  //    low  -> up
  int x = joystick.getHorizontal();
  int y = joystick.getVertical();

  bool xChanged = abs(x - xCenter) > 2;
  bool yChanged = abs(y - yCenter) > 2;
  if (xChanged || yChanged) {
    int xd = distance(x);
    if (xChanged && xd == 0) {
      if ((x - xCenter) > 0) {
        xd = 1;
      } else {
        xd = -1;
      }
    }
    int yd = distance(y);
    if (yChanged && yd == 0) {
      if ((y - yCenter) > 0) {
        yd = 1;
      } else {
        yd = -1;
      }
    }

    //    Serial.print("MOVE X: ");
    //    Serial.print(xd);
    //    Serial.print(", Y: ");
    //    Serial.println(yd);
    Mouse.move(xd, yd, 0);
  }
  delay(responseDelay);
}

int distance(int readVal) {

  // map the readVal from the analog input range to the output range:
  readVal = map(readVal, 0, 1023, 0, range);

  int d = readVal - center;

  // return the distafnce for this axis:
  return d;
}
