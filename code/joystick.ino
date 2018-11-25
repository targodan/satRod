/*
MIT License

Copyright (c) 2018 Luca Corbatto

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <Encoder.h>

#include <IoAbstraction.h>
#include <IoAbstractionWire.h>

#include <Joystick.h>

#define PIN_MODE_PULL_UP 1
#define PIN_MODE_PULL_DOWN 2

#define BUTTON_MODE_ON_PRESS 1
#define BUTTON_MODE_ON_CHANGE 2

#define DOWN 1
#define UP 0

IoAbstractionRef ioLocal = ioUsingArduino();

const byte encoderResetCountdown = 5;
const byte buttonResetCountdown = 20;

typedef enum {
  X,
  Y,
  Z,
  RX,
  RY,
  RZ,
  RUDDER,
  THROTTLE,
  ACCELERATOR,
  BRAKE,
  STEERING
} AxisType;

typedef struct {
  byte pin;
  AxisType type;
} Axis;

typedef struct {
  IoAbstractionRef io;
  byte pin;
  byte joystickButton;
  byte pinMode;
  byte buttonMode;
  byte resetCountdown;
  byte lastState;
} JoyButton;

typedef struct {
  byte pinLeft;
  byte pinRight;
  byte joystickButtonLeft;
  byte joystickButtonRight;
  byte resetCountdown;
  Encoder encoder;
} JoyEncoder;

#include "config.h"

const int numAxis = sizeof(axis)/sizeof(Axis);
const int numJoyButtons = sizeof(joyButtons)/sizeof(JoyButton);
const int numJoyEncoders = sizeof(joyEncoders)/sizeof(JoyEncoder);

byte readJoyButton(const JoyButton& btn) {
  byte val = ioDeviceDigitalReadS(btn.io, btn.pin);
  if(btn.pinMode == PIN_MODE_PULL_UP) {
    val = !val;
  }
  return val == HIGH ? DOWN : UP;
}

bool isAxisEnabled(AxisType ax) {
  for(int i = 0; i < numAxis; ++i) {
    if(axis[i].type == ax) {
      return true;
    }
  }
  return false;
}

// Create the Joystick
Joystick_ Joystick(
    JOYSTICK_DEFAULT_REPORT_ID, // ID
    JOYSTICK_TYPE_JOYSTICK, // Type
    numJoyButtons+numJoyEncoders*2, // Button Count
    0, // HAT Switch Count
    isAxisEnabled(AxisType::X), // XAxis
    isAxisEnabled(AxisType::Y), // YAxis
    isAxisEnabled(AxisType::Z), // ZAxis
    isAxisEnabled(AxisType::RX), // XAxis Rotation
    isAxisEnabled(AxisType::RY), // YAxis Rotation
    isAxisEnabled(AxisType::RZ), // ZAxis Rotation
    isAxisEnabled(AxisType::RUDDER), // Rudder
    isAxisEnabled(AxisType::THROTTLE), // Throttle
    isAxisEnabled(AxisType::ACCELERATOR), // Accelerator
    isAxisEnabled(AxisType::BRAKE), // Brake
    isAxisEnabled(AxisType::STEERING) // Steering
);

void setup() {
  Wire.begin(false);

  ioDevicePinMode(deactivateIO, deactivatePin, INPUT_PULLUP);

  for(int i = 0; i < numJoyButtons; ++i) {
    byte mode;
    if(joyButtons[i].pinMode == PIN_MODE_PULL_UP) {
      mode = INPUT_PULLUP;
    } else {
      mode = INPUT;
    }
    ioDevicePinMode(joyButtons[i].io, joyButtons[i].pin, mode);
  }
  for(int i = 0; i < numJoyEncoders; ++i) {
    ioDevicePinMode(ioLocal, joyEncoders[i].pinLeft, INPUT_PULLUP);
    ioDevicePinMode(ioLocal, joyEncoders[i].pinRight, INPUT_PULLUP);
  }

  // Initialize Joystick Library
  Joystick.begin();
}

void loop() {
  bool ignoreInput = !ioDeviceDigitalReadS(deactivateIO, deactivatePin);

  for(int i = 0; i < numAxis; ++i) {
    int val = analogRead(axis[i].pin);

    switch(axis[i].type) {
      case X:
        Joystick.setXAxis(val);
        break;
      case Y:
        Joystick.setYAxis(val);
        break;
      case Z:
        Joystick.setZAxis(val);
        break;
      case RX:
        Joystick.setRxAxis(val);
        break;
      case RY:
        Joystick.setRyAxis(val);
        break;
      case RZ:
        Joystick.setRzAxis(val);
        break;
      case RUDDER:
        Joystick.setRudder(val);
        break;
      case THROTTLE:
        Joystick.setThrottle(val);
        break;
      case ACCELERATOR:
        Joystick.setAccelerator(val);
        break;
      case BRAKE:
        Joystick.setBrake(val);
        break;
      case STEERING:
        Joystick.setSteering(val);
        break;
    }
  }

  for(int i = 0; i < numJoyButtons; ++i) {
    JoyButton& btn = joyButtons[i];

    // Reset if necessary
    if(btn.resetCountdown > 0) {
      --btn.resetCountdown;
      if(btn.resetCountdown == 0) {
        Joystick.releaseButton(btn.joystickButton);
      }
    }

    // Read and set
    byte val = readJoyButton(btn);
    if(val != btn.lastState) {
      if(btn.buttonMode == BUTTON_MODE_ON_PRESS) {
        Joystick.setButton(btn.joystickButton, val);
      } else if(!ignoreInput) {
        // ON_CHANGE
        Joystick.pressButton(btn.joystickButton);
        btn.resetCountdown = buttonResetCountdown;
      }

      btn.lastState = val;
    }
  }

  for(int i = 0; i < numJoyEncoders; ++i) {
    JoyEncoder& enc = joyEncoders[i];

    // Reset if necessary
    if(enc.resetCountdown > 0) {
      --enc.resetCountdown;
      if(enc.resetCountdown == 0) {
        Joystick.releaseButton(enc.joystickButtonLeft);
        Joystick.releaseButton(enc.joystickButtonRight);
      }
    }

    // Read and set
    int state = enc.encoder.read();
    if(state == 0) {
      continue;
    }

    byte buttonID = state < 0 ? enc.joystickButtonLeft : enc.joystickButtonRight;
    Joystick.pressButton(buttonID);
    enc.resetCountdown = encoderResetCountdown;

    enc.encoder.write(0);
  }

  noInterrupts();
  Joystick.sendState();
  interrupts();
}
