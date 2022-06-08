# pio-arduinoteensy
PlatformIO framework-arduinoteensy package
Repo to support development in PlatformIO of a custom Teensy Arduino core.
Example platformio.ini:

[env:teensy41]

platform = teensy

framework = arduino

board = teensy41

platform_packages =
   framework-arduinoteensy @ https://github.com/spinorkit/pio-arduinoteensy.git
