This fork is intended to be used on a ```Logitech G413``` keyboard which features a ```stm32l100r8``` MCU with the ```Arduino``` framework.
### Wiring 
| Test point | Description | Notes                       |
|------------|-------------|-----------------------------|
| TP36       | NRST        |                             |
| TP53       | DIO         | has 'DIO' on the silkscreen |
| TP54       | CLK         | has 'CLK' on the silkscreen |
| TP34       | VCC         | 3.3V                        |
| TP37       | GND         |                             |
| TP2        | SCL         | Software I2C                |
| TP3        | SDA         | Software I2C                |
![](/images/G413.png)
### Execution
Hook up a STLINK on TP36,TP53,TP54 pins then dump the firmware off the keyboard by using the STM32CubeProg tool.
Then load the arduino sketch and flash!
Do not continue if the keyboard has Level-2 security implemented (this will be shown in STM32CubeProg tool).
NOTE: This will erase the keyboard! so be very cautious when doing this.
Hook up SH1106 LCD to TP2,TP3,TP34,TP37
### Misc 
A ROM dump of the keyboard can found under the ROM folder which contains the code running on a stock G413 that can be useful to flash the keyboard without any external tools! psst QMK community
### Resources
resource which helped me port to this keyboard: https://geekhack.org/index.php?topic=113813.0
## doom-nano
A 3d raycast engine for Arduino

![](/images/screen-1.jpg?raw=true)

See it in action:
- https://youtu.be/wJG04DSo7Xw (Progress on Feb, 09 2019)
- https://youtu.be/FX7PQtUAkHk (Progress on Jan, 18 2019)

Just experimenting with an Arduino Nano I bought some time ago. This chip is very limited in process and memory: 16Mhz, only 32kb for program memory and 2kb of RAM, of which 1kb is entirely used for the screen.
Most of problems I´m facing currently are about memory. CPU clock might be a problem too, but can be more or less avoided having more memory for calculation shortcuts.
Despite of all those limitations it´s capable to run at up to 14 FPS. Probably can be optimized and structured much better. Will try to keep learning and improve it.

To be clear. **This is not an actual Doom game**, just picked some sprites from it (and simplified a lot). The rendering engine is more like a Wolfeistein 3D. The Doom idea came because I started this building the fancy melt-screen effect (included in an early version, but not anymore).

Hardware I used:
- A protoboard
- An Arduino Uno/Nano (ATmega328P)
- An OLED Display (i2c 128x64)
- 4 buttons
- ~~4 10k ohms resistors~~. Not needed anymore, check out the wiring diagram from below.
- Buzzer (Optional)

Resources:
- Sprites from https://www.spriters-resource.com
- Much thanks to https://lodev.org/cgtutor for so wonderful resource about raycasting engines

Current status:
- The map rendering is working nicely. Even I was able to add a depth effect by using different dithering patterns, depending on the distance to the view.
- Sprites are working too, though has some issues hiding them behind walls because memory limitations (the z-buffer precision has been limited a lot to make it smaller).
- You can move through the map, collide with walls, collect items and interact with enemies. I could also add the jogging effect, like Doom´s one.
- The enemies AI, despite is very simple, I think works very well and it´s enough for the purpose of the game. Looks very similar to  Imp enemy from original Doom.
- For the HUD, I realized that the native `print` from Adafruit's library uses too much memory. So I've implemented my custom text rendering methods and a custom font which includes only needed characters and some icons.
- ~~Currently I´m using 99% of program memory, which doesn´t let me adds more code. I need to research how to optimize it to make it smaller.~~
- FPS and memory consumption has been improved using a slim and customized version of the SSD1306 lib from Adafruit. Thanks  [@miracoly](https://github.com/miracoli)
- Now it has a basic sound support through Pin 9. Thanks again [@miracoly](https://github.com/miracoli)!

(I'd like) To do:
- ~~Make possible kill enemies.~~
- Doors and locked doors.
- A game over screen.
- Add more sprites, decorative elements, etc.
- Textures? Very performance expensive. I don't think so.
- Make code looks nicer! Move all to pure c++.
- ~~Sound/Music? Hmmm I wish so, but...~~

Simplified version (using an Arduino UNO, built-in pull-up resistors for buttons and a buzzer):
![](/images/input-pull-up-version.jpg?raw=true)

Wiring:
![](/images/wiring.png?raw=true)

More screens (outdated):
![](/images/screen-4.jpg?raw=true)
![](/images/screen-5.jpg?raw=true)
![](/images/screen-6.jpg?raw=true)
![](/images/screen-7.jpg?raw=true)
