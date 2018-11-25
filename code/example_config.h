#ifndef CONFIG_H
#define CONFIG_H

IoAbstractionRef ioExpander = ioFrom8574(0x20);

IoAbstractionRef deactivateIO = ioLocal;
byte deactivatePin = 12;

Axis axis[] = {
  {
    0, // pin
    AxisType::X
  }
};

JoyButton joyButtons[] = {
  {
    ioExpander,
    0, // pin
    0, // buttonID
    PIN_MODE_PULL_DOWN,
    BUTTON_MODE_ON_PRESS,
    0,
    UP
},
  {
    ioLocal,
    8, // pin
    1, // buttonID
    PIN_MODE_PULL_UP,
    BUTTON_MODE_ON_CHANGE,
    0,
    UP
  }
};

JoyEncoder joyEncoders[] = {
  {
    0, // pinLeft
    4, // pinRight
    3, // buttonLeft
    4, // buttonRight
    0,
    {0, 4} // Encoder
  }
};

#endif // CONFIG_H
