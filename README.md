# satRod
A project aiming to create a custom Joystick using an Arduino Leonardo.

Follow me on [Busy](https://busy.org/@targodan) or [Steemit](https://steemit.com/@targodan) if you want to see any tutorials for this project as soon as they are uploaded.

## Cloning this Repo

If you want to clone this repository, especially if you want to just use the files and not contribute, you should consider cloning only the latest change using this.

```bash
$ git clone --depth 1 https://github.com/targodan/satRod.git
```

The reason for this being that the 3D files are rather big and this will greatly decrease the time it takes to clone this repository.

## Tutorials

Following me is in no way a requirement to see the tutorials. Here is a list of already released tutorials. More will follow.

- [Reading Buttons with a Microcontroller](https://d.tube/#!/v/targodan/lybimad3)
- [Overview of the included 3D Models](https://busy.org/@targodan/r9bloye6)

DTube is a fairly new platform with a few problems. Should the videos not be available try again at a later time. Should they still not be available then feel free to open an [issue](https://github.com/targodan/satRod/issues/new). I will do my best to make the videos available.

## Requirements

You'll need...

- an Arduino *Leonardo*. No other type of Arduino will work at this stage.
- Optionally you can use a PCF8574 based IO expander if you want lots of buttons.
- As many buttons/switches/analog sticks/rotary encoders/etc. that you need.

In your Arduino IDE you need to have the following libraries installed.

- [The Arduino Joystick Library by MHeironimus](https://github.com/MHeironimus/ArduinoJoystickLibrary)
- Encoder by Paul Stoffregen (Available through the Library Manager)
- IoAbstraction by davetcc (Available through the Library Manager)

## Supported Inputs

- Up to 32 Buttons or Switches in a down- or a change-mode (in change mode a short pulse will be sent whenever the state changes)
- Up to 3 rotary encoders (make sure at least one data line of each encoder is connected to an interuptable pin, i. e. pins 2, 3 or 7)
- Up to 6 analog axis (Currently limited by the amount of analog inputs of the Arduino. External ADCs are not yet supported.)

Hat switches are not yet supported.

## Available Joysticks

- [Elite Dangerous Joystick](https://github.com/targodan/satRod/blob/master/models/eliteJoystick/README.md)

## Contributing

If you want to contribute please open a pull-request onto the `develop` branch. I want the master branch to only contain released versions of this project.

Contributions may include other custom joysticks with 3D models and/or a configuration file. If you find any bugs feel free to open an issue and/or open a pull-request if you have fixed a bug.

I would love it if this repository became a big place for all sorts of different custom joysticks that are ready to print and assemble! :D

## License

The source code in the satRod project is published under the MIT License, see [LICENSE](https://github.com/targodan/satRod/blob/master/LICENSE) for details.

<a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />Any other files included here as well as all videos or written tutorials listed in this readme are released under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.
