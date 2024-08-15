# Serial protocol

## c# serial port Setup

```c++:
this.ardPort = new WinSerialPortStream(portName);
this.ardPort.BaudRate = 115200;
this.ardPort.DataBits = 8;
this.ardPort.ReceivedBytesThreshold = 2;
this.ardPort.DtrEnable = true;
this.ardPort.DataReceived += ArdPort_DataReceived;
this.ardPort.ReadTimeout = 100;
this.ardPort.WriteTimeout = 200;
```

## Global workflow

- Send Ident message, check the arduino response container "EDControlPanel".

- Messages sent to arduino start with a char, optionally followed by data.
Arduino may answer with a string, starting with a char, followed by \t, followed by data, ending with a "\n"

## Messages specs

### Ident

Purpose

: Get Arduino slave program name & version

Header char

: `I`

Returns

: `I\t` followed by ident string, containing "EDControlPanel"

### Silent

Purpose

: Put arduino in silent mode

Header char

: `S`

Returns

: `S`

### Hello

Purpose

: Used to send a ping to arduino, and know if it is in it's startup phase

Header char

: `H`

Returns

: `h` if arduino is in startup phase, `H` otherwise

### Verbose

Purpose

: Put the arduino in verbose mode

Header char

: `V`

Returns

: `V` followed by ident string

### Location

Purpose

: Send star system location infos

Header char

: `V`

Data

: `\t` separator
  System name, max 20 chars, `\t` terminated
  Station name, max 20 chars, `\t` terminated
  Local allegiance, max 20 chars, `\t` terminated
  System security level, max 20 chars, `\0` terminated

Returns

: N/A

### Game infos

Purpose

: Send game general infos

Header char

: `G`

Data

: `\t` separator
  Commander name, max 20 chars, `\t` terminated
  Ship name, max 20 chars, `\0` terminated

Returns

: N/A

### Game status infos

Purpose

: Send game status flags and infos

Header char

: `F`

Data

: Status flags 1, uint32_t (see EDGamesVariables.h, STATUS_FLAGS1)
  Status flags 2, uint32_t (see EDGamesVariables.h, STATUS_FLAGS2)
  GUI Focus flag, byte (see EDGamesVariables.h, GUIFOCUS_MODE)
  Current fire group, byte
  Legal status, max 20 chars, `\0` terminated

Returns

: N/A

### Loadout

Purpose

: Send ship module loadout flags. Each equipment uses 4 bits.
  0b0001 0x1 : Module is On
  0b0010 0x2 : Ammo in clip > 0
  0b0100 0x4 : Ammo in Hopper > 0
  0b1000 0x8 : Health > 0

Header char

: `M`

Data

: Loadout flags 1, uint32_t (see EDGamesVariables.h, LOADOUT_FLAGS1)
  Loadout flags 2, uint32_t (see EDGamesVariables.h, LOADOUT_FLAGS2)
  unused string, future usage, max 20 chars, `\0` terminated

Returns

: N/A

### Nav route

Purpose

: Send FTL navigation route

Header char

: `N`

Data

: `\t` separator
  Nav route 1, max 20 chars, `\t` terminated
  Nav route 2, max 20 chars, `\t` terminated
  Nav route 3, max 20 chars, `\0` terminated

Returns

: N/A

### Alert message

Purpose

: Send Alert message. The message will be displayed for *duration* seconds.
: Title is displayed on first line, blinking.
: Sending a duration of zero clear the alert.

Header char

: `A`

Data

: Alert duration in seconds, uint32_t
  Alert title, max 20 chars, `\t` terminated
  Alert line 1, max 20 chars, `\t` terminated
  Alert line 2, max 20 chars, `\t` terminated
  Alert line 3, max 20 chars, `\0` terminated

Returns

: N/A
