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

IoAbstractionRef ioExpander = ioFrom8574(0x20);
IoAbstractionRef ioLocal = ioUsingArduino();

IoAbstractionRef deactivateIO = ioLocal;
byte deactivatePin = 12;

const int xAxisPin = 0;
const int yAxisPin = 1;

const byte encoderResetCountdown = 5;
const byte buttonResetCountdown = 20;

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

JoyButton joyButtons[] = {
  {
    ioExpander,
    0, // pin
    9, // buttonID
    PIN_MODE_PULL_DOWN,
    BUTTON_MODE_ON_PRESS,
    0,
    UP
  },
  {
    ioExpander,
    1, // pin
    10, // buttonID
    PIN_MODE_PULL_UP,
    BUTTON_MODE_ON_PRESS,
    0,
    UP
  },
  {
    ioExpander,
    2, // pin
    11, // buttonID
    PIN_MODE_PULL_UP,
    BUTTON_MODE_ON_CHANGE,
    0,
    UP
  },
  {
    ioExpander,
    3, // pin
    12, // buttonID
    PIN_MODE_PULL_UP,
    BUTTON_MODE_ON_CHANGE,
    0,
    UP
  },
  {
    ioExpander,
    4, // pin
    13, // buttonID
    PIN_MODE_PULL_UP,
    BUTTON_MODE_ON_CHANGE,
    0,
    UP
  },
  {
    ioExpander,
    5, // pin
    14, // buttonID
    PIN_MODE_PULL_UP,
    BUTTON_MODE_ON_CHANGE,
    0,
    UP
  },
  {
    ioExpander,
    6, // pin
    15, // buttonID
    PIN_MODE_PULL_UP,
    BUTTON_MODE_ON_CHANGE,
    0,
    UP
  },
  {
    ioExpander,
    7, // pin
    16, // buttonID
    PIN_MODE_PULL_UP,
    BUTTON_MODE_ON_CHANGE,
    0,
    UP
  },
  {
    ioLocal,
    6, // pin
    2, // buttonID
    PIN_MODE_PULL_UP,
    BUTTON_MODE_ON_CHANGE,
    0,
    UP
  },
  {
    ioLocal,
    10, // pin
    1, // buttonID
    PIN_MODE_PULL_UP,
    BUTTON_MODE_ON_PRESS,
    0,
    UP
  },
  {
    ioLocal,
    8, // pin
    0, // buttonID
    PIN_MODE_PULL_UP,
    BUTTON_MODE_ON_PRESS,
    0,
    UP
  }
};

const int numJoyButtons = sizeof(joyButtons)/sizeof(JoyButton);

JoyEncoder joyEncoders[] = {
  {
    0, // pinLeft
    4, // pinRight
    3, // buttonLeft
    4, // buttonRight
    0,
    {0, 4} // Encoder
  },
  {
    1, // pinLeft
    5, // pinRight
    5, // buttonLeft
    6, // buttonRight
    0,
    {1, 5} // Encoder
  },
  {
    7, // pinLeft
    9, // pinRight
    7, // buttonLeft
    8, // buttonRight
    0,
    {7, 9} // Encoder
  }
};

const int numJoyEncoders = sizeof(joyEncoders)/sizeof(JoyEncoder);


// Create the Joystick
Joystick_ Joystick(
    JOYSTICK_DEFAULT_REPORT_ID, // ID
    JOYSTICK_TYPE_JOYSTICK, // Type
    numJoyButtons+numJoyEncoders*2, // Button Count
    0, // HAT Switch Count
    true, // XAxis
    true, // YAxis
    false, // ZAxis
    false, // XAxis Rotation
    false, // YAxis Rotation
    false, // ZAxis Rotation
    false, // Rudder
    false, // Throttle
    false, // Accelerator
    false, // Break
    false // Steering
);

byte readJoyButton(const JoyButton& btn) {
  byte val = ioDeviceDigitalReadS(btn.io, btn.pin);
  if(btn.pinMode == PIN_MODE_PULL_UP) {
    val = !val;
  }
  return val == HIGH ? DOWN : UP;
}

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
  
  int xVal = analogRead(xAxisPin);
  int yVal = analogRead(yAxisPin);
  
  Joystick.setXAxis(xVal);
  Joystick.setYAxis(yVal);

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
