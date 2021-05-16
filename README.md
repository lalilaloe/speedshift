# SpeedShift

An universal aproach to electronic shifting with off the shelf components.

## Current feature set

- Support for 1x12 speed controlling rear deraileur (Other speeds when customizing script)
- Single digit display indicating current gear (auto turn off delay)
- Configurating gear servo position precisely using controls
- Controlling servo position by gear selection up/down
- Fast shifting with short and long presses

## Future Goals

- Documenting Instructions/variations in mounting and testing/troubleshooting
- A Configurator setting up gears, front and rear deraileur and flashing firmware
- Support for front deraileur
- Update Schematics
- More targets, smaller size Arduino, Node, ESP targets

## Requirements (currently)

1. A target device (Currently available targets):
   - Arduino UNO
1. A servo (waterproof recomended)
1. A set of buttons (waterproof recomended), Wires and (10k) resistors
1. (Optional) a 7 segment display
1. A Li-ion battery providing 5V (with a regulator)
1. Mounting for servo, arduino and buttons (ductape will do 👌)
1. Arduino IDE to flash SpeedShift

## Tested/Recommended components

### Servo

- [Metal Gear High Torque Waterproof Digital Servo (banggood)](https://www.banggood.com/DSSERVO-DS3225-25KG-180-Degree-Metal-Gear-High-Torque-Waterproof-Digital-Servo-For-RC-Airplane-Robot-Car-p-1353688.html?rmmds=myorder&cur_warehouse=CZ&ID=6301958)

### Buttons

- [Tactile waterproof push buttons (banggood) ](https://www.banggood.com/20Pcs-Tact-Switch-DIP-SMD-Momentary-Tact-Tactile-Push-Button-Switch-Waterproof-p-1276757.html?rmmds=myorder&cur_warehouse=CN)

## Usage

\*\* This will probably be configuratable and change in the future

### Switching gears

- Press up or down button for switching 1 gear at a time
- Short press up or down button for a fast switch of 3 gear at a time
- Long press up or down button for a drastic switch of 5 gear at a time'

### Configurating gears

- Press up and down button simultaniously to start configurating the currently selected gear. A dot will apear on the seven segment display indicating you are in configurating mode
- Press up or down button to change the position of the deraileur
- Press up and down button simultaniously again to save the current position of the selected gear into memory

## Contributing

Yes! My aim for this project is to be a interface for multiple variations in setup, a universal firmware for gear selection and configuration. Mounting is up to you, so please add any variation in documention with photos. Maybe if there is enough interest it will gradually turn into a universal product. Or if you are into programming you can add a target, optimize or add features. Please open an issue first when working on a feature/target so others know.

## Other projects (inspiration)

- Automatic gear switching based on speed
  https://www.youtube.com/watch?v=P6gbg_-JhW0 (Direct servo mount)
- https://github.com/Diyshift/Diyshift (2012) (Direct servo mount)
- https://www.youtube.com/watch?v=qWQT1213F5o (Direct cable shifting test)
- Not di2 https://www.youtube.com/watch?v=ErkBtg0VFMs (Rear) https://www.youtube.com/watch?v=SQCnD1VPIFQ (Front)
- https://www.youtube.com/watch?v=v8n-0Rz7nOc (direct servo mount)
