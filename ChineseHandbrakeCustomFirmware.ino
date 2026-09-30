// Chinese Sim Racing Handbrake Custom Firmware
//
// Based on Daniel Korgel's original firmware:
// https://github.com/Dak0r/Chinese-SimRacing-14Bit-Handbrake-Custom-Firmware-Arduino-Sketch
//
// For compatible Arduino Pro Micro / ATmega32U4 handbrakes with an analog
// Hall sensor connected to A2. The firmware reports the lever as a USB
// game-controller Z axis and supports two optional buttons on pins 2 and 3.
//
// Set HALL_MIN_VALUE and HALL_MAX_VALUE below to calibrate the lever's
// released and fully pulled positions. The analog input is 10-bit by default;
// the joystick axis is scaled to values from 0 to 4095.
//
// Requires the Joystick library by Matthew Heironimus:
// https://github.com/MHeironimus/ArduinoJoystickLibrary
//
// Uncomment DEBUG below to print raw sensor and mapped axis values
// to the Serial Monitor at 115200 baud.
//--------------------------------------------------------------------

//#define DEBUG // uncomment for debug output

#define AXIS_RESOLUTION   4095 // 12-bit axis maximum
#define PIN_HALL_SENSOR   A2
#define PIN_BUTTON_0      2
#define PIN_BUTTON_1      3
#define HALL_MIN_VALUE    270 // example raw value with the handbrake released
#define HALL_MAX_VALUE    720 // example raw value with the handbrake fully pulled

#if HALL_MAX_VALUE <= HALL_MIN_VALUE
#error HALL_MAX_VALUE must be greater than HALL_MIN_VALUE
#endif

#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  2,                      // Button Count
  0,                      // Hat Switch Count
  false, false, true,     // No X or Y axis; Z axis enabled
  false, false, false,    // No Rx, Ry, or Rz
  false, false,            // No rudder or throttle
  false, false, false);   // No accelerator, brake, or steering

void setup() {

#ifdef DEBUG
  Serial.begin(115200);
#endif

  // Initialize Hall sensor input
  pinMode(PIN_HALL_SENSOR, INPUT);

  // Initialize button pins
  pinMode(PIN_BUTTON_0, INPUT_PULLUP);
  pinMode(PIN_BUTTON_1, INPUT_PULLUP);

  // analogRead() is 10-bit on the ATmega32U4; the joystick axis is scaled below.

  // Initialize Joystick Library
  Joystick.begin(false);
  Joystick.setZAxisRange(0, AXIS_RESOLUTION);
}


void loop() {

  // Buttons use INPUT_PULLUP, so LOW means pressed.
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
