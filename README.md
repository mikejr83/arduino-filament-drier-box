# Arduino Powered Filament Drier Box 

**With RepRapDiscount Full Graphic Smart Controller**

This project is for an Arduino powered filament drier box. The idea is to repurpose components which can come from 3D printers into a useful tool to remove moisture from 3D printer filament.

The idea for the project came after deciding that a MakerTech 3D AXIS 3D printer was not worth investing any more time in troubleshooting. The printer had some functional parts that could be reused for other projects. While working on a dry box for filament storage the idea came into being to use parts from the AXIS to take the dry box from passive moisture control to actively attempting to remove it.

## Hardware Requirements

This project uses the following:

1. Arduino Uno Microcontroller
2. RepRapDiscount Full Graphic Smart Controller
3. 3D printer heated bed
4. External mosfet control board for supplying power to heated bed *(commonly found in 3D printers for supplying the high current demands directly from the power supply to the bed without traversing the mainboard controlling the printer)*
5. 24v power supply
6. 24v to 5v buck converter or similar voltage step down *(for powering the Arduino and LCD from the power supply)*
7. 100k thermister (attached to the heated bed in most cases)
8. Pull-up/down resister
9. Various assorted wires, tools, etc.

Other than the Arduino, pull-up/down resister, and tools, all parts were pulled from the MakerTech3D AXIS printer. If you have a printer that is no longer worth your troubles you may already have most of these components. The rest can be found extremely cheaply on Amazon or SparkFun.

## Project Code

The codebase makes use of the PlatformIO integration with VisualStudio Code. You should be able to clone this repository, open the code in VS Code with the PlatformIO extension installed, and then build/deploy.

The project relies on some external libraries which should be managed by the PlatformIO extension.

1. LiquidCrystal
2. AutoPID
