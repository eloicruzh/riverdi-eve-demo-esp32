OVERVIEW
--------

This repository contains example of [*riverdi-eve-lib*](https://github.com/riverdi/riverdi-eve-lib) library integration for [*ESP32 MCUs*](https://www.espressif.com/en/products/socs/esp32) (series of low-cost, low-power system on a chip microcontrollers with integrated Wi-Fi and dual-mode Bluetooth). Please check [*riverdi-eve-lib*](https://github.com/riverdi/riverdi-eve-lib) project repository for more info about library itself.

PREREQUISITES
-------------

To get started, please make sure, that all libraries and tools for C/C++ development on *ESP32* are properly installed and configured on your machine - visit [*ESP32 Get Started*](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/) for more info.

CONNECTIONS
-----------

For different chip the connections may be different - please check *main/platform.h* file for specific pinouts.

|  RiBUS connector    | ESP32 |
| ---- | ----- |
| VDD  | 3.3V  |
| GND  | GND   |
| SCLK | 19    |
| MISO | 25    |
| MOSI | 23    |
| CS   | 21    |
| INT  | 5     |
| PD   | 18    |

COMPILATION
-----------

[1] Set up the environment variables:

`. /path/to/esp-idf/export.sh`

[2] Clone *'riverdi-eve-demo-esp32'* repository (with *'riverdi-eve-lib'* submodule):

`git clone --recursive git@github.com:riverdi/riverdi-eve-demo-esp32.git`

[3] Navigate to your *riverdi-eve-demo-esp32* directory and set the target:

`cd riverdi-eve-demo-esp32`
`idf.py set-target esp32|esp32s2|esp32c3|esp32s3|esp32c2|esp32c6|esp32h2`

[4] Build the project by running:

`idf.py build`

[5] Flash deivce:

`idf.py -p PORT flash`

GETTING HELP
------------

Please contact Riverdi support - [*<contact@riverdi.com>*](contact@riverdi.com)

LICENSE
-------

See *LICENSE.txt* file for details.

