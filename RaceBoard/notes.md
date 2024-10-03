# Notes

---

## Configuration

- File olr-settings.h line 48
PIN_LED = 22 (led strip)
PIN_AUDIO = 23 (loudspeaker)
line 71...
  - \#define DIG_CTRL_1_PIN 12 // switch player 1 to PIN and GND
  - \#define DIG_CTRL_2_PIN 14 // switch player 2 to PIN and GND
  - \#define DIG_CTRL_3_PIN 27 // switch player 3 to PIN and GND
  - \#define DIG_CTRL_4_PIN 33 // switch player 4 to PIN and GND

- File olr-param.h, line 18
MAXLED = number of led
NUMLAP = 5

## piste

dans le fichier olr-param.c (line 18...)

---

## Serial command

### Protocol

- welcome message
DEFAULT->EEPROM-v):9
- \# -> Handshake

### Informations

- $ -> Get Board UID
\$ADIMAKER
- ? -> Get Software Id
?A4P0
- % -> Get Software Version
%0.9.9
- Q -> Get current configuration Info
QTK:64,64,0,-1,10,0,0.006,0.015,0
QRP:80,90,100,6,0
QBT:3,60,10,0
QRC:1,5,1,1,0,0,0,0
- : -> **Set** board Unique Id and save

### Settings

- @ -> Enter "Configuration Mode" status
- \* -> Exit "Configure Mode"
- P -> Parse Player 3/4 configuration -> P[2|3|4]
- M -> Parse DEMO mode configuration
- G -> Parse Autostart configuration -> Gautostart
- E -> Parse Battery configuration -> Edelta,min,boost,active
- A -> Parse Ramp configuration -> Astart,center,end,high,perm
- B -> Parse BoxLenght Configuration -> Blen,perm
- T -> Parse Track configuration -> Track length
- C -> Parse race configuration -> C1,2,3,0
- K -> Parse Physic simulation parameters
- W -> Write configuration to EEPROM
- D -> Load Default Parameters and store them in from EEPROM
- R -> Set Race Phase
  - IDLE = 0,
  - 1/ CONFIG,
  - 2/ CONFIG_OK,
  - 3/ READY,
  - 4/ COUNTDOWN,
  - 5/ RACING,
  - 6/ PAUSE,
  - 7/ RESUME,
  - 8/ COMPLETE,
  - 9/ RACE_PHASES

---

## Telemetry

p**abc**,**d**,**ef**

- a -> number of car (int from 1 to 4)
- b -> track ID (char)
- c -> lap number (byte from 1 to NUMLAP)
- d -> position (int)
- e -> battery level (int)
- f -> \\n (EOL)

p1M1,0,100
p2M1,0,100
