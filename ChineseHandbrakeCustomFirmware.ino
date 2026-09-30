// Alternative Firmaware for Chinese "14 Bit" Sim Racing Handbrake by Daniel Korgel
//
// The 12 and 14 Bit Version of the chineses SimRacing Handbrake use an
// Arduino Pro Micro and a hall sensor.
//
// This sketch is desigend as a replacement firmware to allow modifcations
// like connecting more buttons to the
//
// Arduino Joystick Library by Matthew Heironimus
// Download and place in Arduino/library directory:
// Repository: https://github.com/MHeironimus/ArduinoJoystickLibrary
// Director Download: https://github.com/MHeironimus/ArduinoJoystickLibrary/archive/master.zip
//--------------------------------------------------------------------

//#define DEBUG // uncomment for debug output

#define AXIS_RESOLUTION   4095 //12 Bits axis maximum
#define PIN_HALL_SENSOR   A2
#define PIN_BUTTON_0      2
#define PIN_BUTTON_1      3
#define HALL_MIN_VALUE    270 // raw value with the handbrake released
#define HALL_MAX_VALUE    720 // raw value with the handbrake fully pulled

#if HALL_MAX_VALUE <= HALL_MIN_VALUE
#error HALL_MAX_VALUE must be greater than HALL_MIN_VALUE
#endif

#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  2,                      // Button Count
  0,                      // Hat Switch Count
  false, false, true,     // X and Y, but no Z Axis
  false, false, false,    // No Rx, Ry, or Rz
  false, false,            // No rudder or throttle
  false, false, false);   // No accelerator, brake, or steering

void setup() {

#ifdef DEBUG
  Serial.begin(115200);
#endif

  // Initialize Button Pins
  pinMode(PIN_HALL_SENSOR, INPUT);

  pinMode(PIN_BUTTON_0, INPUT_PULLUP);
  pinMode(PIN_BUTTON_1, INPUT_PULLUP);

  //analogReadResolution(14); // not supported on Arduino Micro, so 14 Bit readout is not possible.

  // Initialize Joystick Library
  Joystick.begin(false);
  Joystick.setZAxisRange(0, AXIS_RESOLUTION);
}


void loop() {

  // Read pin values
  Joystick.setButton(0, digitalRead(PIN_BUTTON_0)==LOW);
  Joystick.setButton(1, digitalRead(PIN_BUTTON_1)==LOW);

  int hallSensorValue = analogRead(PIN_HALL_SENSOR);

  int zAxisValue = map(constrain(hallSensorValue, HALL_MIN_VALUE, HALL_MAX_VALUE),
                       HALL_MIN_VALUE, HALL_MAX_VALUE, 0, AXIS_RESOLUTION);

#ifdef DEBUG
    Serial.print("Hall: ");
    Serial.print(hallSensorValue);
    Serial.print(" Z: ");
    Serial.println(zAxisValue);
#endif
  Joystick.setZAxis(zAxisValue);
  Joystick.sendState();

  delay(10);
}
