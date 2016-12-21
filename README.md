# dfrobot_rfid
A simple Arduino library for DFRobot's ID01 UHF RFID reader, which otherwise has no publicly available drivers.

## Introduction

The sample code provided on the [wiki page](https://www.dfrobot.com/wiki/index.php/UHF_RFID_MODULE-UART_(SKU:TEL0082)) for this product only covers two command sequences, and does not actually tell you how to read RFID tags. Furthermore, the [manual](https://github.com/Arduinolibrary/DFRobot_ID10_UHF_RFID_Reader/blob/master/ID01%20UHF%20RFID%20Reader%20user%20manual.pdf?raw=true) is complete indecipherable garbage. I spent a few days parsing it and wrote a driver based off of it, but when I went to test it I found that absolutely nothing worked the way the manual said it would. The only thing I could do successfully was read tags in the module's default mode, however nowhere was the format of the output documented.

This Arduino library provides only the bare minimum functionality to read tags with the ID01 module. Without needing to send any commands to the module, it will always start up in "continuous read" mode, spitting out tag IDs that it has read. This lightweight library simply parses these tags and verifies their checksums, providing an easy-to-use API for obtaining tag IDs.

## Usage

### This repository includes:

- The library, consisting of `RfidReader.cpp` and `RfidReader.h`
- A sample sketch, `dfrobot_rfid.ino`

### To use the library:

1. Copy the `.cpp` and `.h` files to your sketch directory, or install them somewhere the Arduino IDE can find them.

2. Add `#include RfidReader.h` to the preamble of your sketch.

3. Instantiate an `RfidReader` object on the `Stream` corresponding to the UART where the RFID module is connected. This can be either a `SoftwareSerial` or `HardwareSerial` device.

4. Call `begin` on the serial device if necessary; the default baud rate is 9600.

5. Set up a 12-byte buffer to store tag ID's in.

6. Call the `RfidReader` object's `readTagId` method, passing it your buffer as a parameter. The method returns `true` if a read a tag; the tag will have been placed into your buffer. If it returns `false`, no tag has been read.

The provided sample sketch gives an example of this usage.

### Additional notes:

- The sample sketch uses `Serial1` as the connection to the RFID module. This assumes an Arduino Due or Mega (or some other Arduino with multiple hardware UARTs).
- The `readTagId` function will consume all available input from the UART when it is called. As a result, it will return the *most recent* tag ID that has been seen since the last time it was called.
- The library verifies checksums and frame formats internally, and keeps a fixed-size circular buffer. As a result, it should be robust to any acute serial errors.

## Protocol

For the sake of completeness, here is a description of the tag ID format used by the ID01:

- A tag is sent in a 17-byte frame.
- The frame begins with start sequence `0x00 0x00`.
- The ID, 12 bytes, follows.
- After the ID, there is one more byte of `0x00`.
- Next, a 1-byte checksum. The sum of all 16 bytes read so far, modulu `0xff`, should be `0x00`.
- Finally, a trailer byte `0xff`.

## License

The content of this repository is released under the MIT License; see accompanying `LICENSE` file for details.