#ifndef CONFIG_H
#define CONFIG_H

IoAbstractionRef ioExpander = ioFrom8574(0x20);

IoAbstractionRef deactivateIO = ioLocal;
byte deactivatePin = 12;

Axis axis[] = {
  {
    0, // pin
    AxisType::X
  },
  {
    1, // pin
    AxisType::Y
  }
};

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

#endif // CONFIG_H
