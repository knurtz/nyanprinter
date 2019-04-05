# NyanPrinter
This printer runs on STM32F103 and it prints AND plays the nyan cat!

## Idea

Thermal printers make a lot of noise while printing -- everyone, who has bought something from a shop with one of those old cash registers knows that! At the heart of such a printer lies of course a stepper motor. Making music with stepper motors is an old thing in the Arduino community. But as far as I know, nobody has ever tried to make music AND print something at the same time -- until now!

Getting picture and sound to line up is not particulary easy, because there are certain limitations to the printing speed of a thermal printer -- and thus also for the maximum pitch we can get out of the machine. 

The datasheet recommends an approximate exposure of 1 ms per line, which responds to a 1 kHz pitch. This is, unless more than 64 pixels out of the total 384 pixels per line are printed simultanously. Here we need to do devided printing, which means multiple exposure bursts for one single line. This is where things get tricky. I need to let the printer run at a fixed speed and cannot stop at a line just because I want to print more than 64 pixels. Otherwise I would get a sudden break in my music.

This is the solution I came up with: If the printer encounters a line with more than 64 pixels, it prints the first 64 (or less) pixels, then advances the motor one step, going back one step immediately in order to print the next 64 pixels and so on. I have not yet tested, if the sudden change in direction is audible. If so, I might have to change it up a little bit and print multiple lines first, then go back over them a second time.

## Hardware

- STM32F103 Bluepill Development Board: [stm32duino.com](https://wiki.stm32duino.com/index.php?title=Blue_Pill)
- Thermal printer from [pollin.de](https://www.pollin.de/p/thermo-druckelement-matsushita-epl1801s2e-721066)
- an awesome huge blue push button from [Amazon](https://www.amazon.de/gp/product/B07BSFMZJ2) (it lights up, too!)
- A4988 stepper motor driver board like this one from [pololu.com](https://www.pololu.com/product/1182)
- RECOM R-78E5.0-0.5 DC/DC converter from [conrad.de](https://www.conrad.de/de/recom-r-78e50-05-dcdc-wandler-print-24-vdc-5-vdc-500-ma-anzahl-ausgaenge-1-x-157954.html?)
- MAX6966 led driver from [farnell.com](https://de.farnell.com/maxim-integrated-products/max6966aee/led-treiber-mit-i-o-erweiterung/dp/2516036) for fancy led stuff
- custom milled PCB with some additional components like resistors and capacitors (Eagle files following shortly)
- custom enclosure printed with the amazing Vertigo Starlight filament from [filamentworld.de](https://www.filamentworld.de/shop/pla-filament-3d-drucker/pla-filament-1-75-mm-vertigo-starlight-075-kg/) (STL files following shortly as well)

## Software

I wanted to learn more about hardware peripherals of the STM32 family in this project. This is why I use a total of 3 timers, 2 DMA channels, one SPI and one UART peripheral. Things could have probably be done less complicated, but I would not have learned anything the easy way.

TIM2 is responsible for the motor step signal. It runs at a frequency that is determined by the current pitch of the music. The motor driver makes a step at every rising edge of the signal. Also, the printer needs to advance two steps for every line. Because of that, at every update interrupt of TIM2 (which corresponds to every step of the motor) I alternate between sending new data to the printer (triggering TIM1) and starting the exposure (triggering TIM3).

Data transfer is timed by TIM1, which triggers 64 DMA requests (64 transfers * 6 "pixels" per transfer = 384 pixels per line). The DMA sends the data to 6 parallel data lines that go into 6 shift registers inside the printer. Data alignment looks weird, this is why I wrote a small python script to convert images to a special byte array.

Finally TIM3 creates a pulse that corresponds to the needed exposure time of about 750 - 1000 us (depending on the supply voltage).

UART is needed for debugging and SPI for the led driver. 
