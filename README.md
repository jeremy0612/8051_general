# 8051 General
## Dictionary Management Application
Author: J3r3my
### Objective

This repo was created in purpose of storing case studies in my learning road about 8051. In practise, 8051 would be "old but gold" for anyone who first get in the verse of Embedded any microcontrollers. I am not sure if I would be an Embedded Engineer in the future but my way could have some mixing of theses as a favor, but i will still keep on filling and make this experiment variety from now and then. For constructing programs and building system simulation I uses vscode as text editor, sdcc compiler and proteus for schematic. 

Thanks for lecturers, friends and people for the inspirations !

### Features

* Schematic design and simulation for each project 📗
* Could have PCB design for some project.
* Firmware programs for 8051 microcontroller familly. 🔍
* Some other assists app 💾

### Constructing

**Recompiling**
To compile the project, simply run the following command :

You could compile the modules files at first, the output is .rel files
```bash
sdcc -c module1.c
sdcc -c module2.c 
```
then you compile the main with the .rel module files, in order to flash the MCU we need .hex file
```bash
sdcc main.c module1.rel module2.rel -o main.hex 
```

**Hex file generating**
Ifyou already have the .ihx file, run the following command to generate .hex file 
```bash
packihx main.ihx > main.hex 
```

## Installation Requirements

The following toolchains is required to run the projects as:

* Proteus 8.13 or later
* SDCC 

To install SDCC, you can download the installer from the [source forge](https://dotnet.microsoft.com/download](https://sourceforge.net/projects/sdcc/files/sdcc-win64/)).
to find more infomation and user manual, you can look at [sdcc document](https://sdcc.sourceforge.net/)

## Future Plans

* Add support for other operating systems, such as macOS and Linux.
* Add more projects.



