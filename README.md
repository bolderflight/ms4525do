[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

![Bolder Flight Systems Logo](img/logo-words_75.png) &nbsp; &nbsp; ![Arduino Logo](img/arduino_logo_75.png)

# Ms4525do
This library communicates with [MS4525DO](https://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Data+Sheet%7FMS4525DO%7FB2%7Fpdf%7FEnglish%7FENG_DS_MS4525DO_B2.pdf%7FCAT-BLPS0002) pressure transducers and is compatible with Arduino and CMake build systems.
   * [License](LICENSE.md)
   * [Changelog](CHANGELOG.md)
   * [Contributing guide](CONTRIBUTING.md)

# Description
The MS4525DO pressure transducers are fully signal conditioned, amplified, and temperature compensated over a temperature range of -10 to +85 C. Digital measurements are sampled with a 14 bit resolution. The MS4525DO sensors are available in a wide variety of pressure ranges and configurations.

# Usage
This library communicates with the AMS 5915 sensors using an I2C interface. Pressure and temperature data can be provided at rates of up to 2 kHz.

# Installation

## Arduino

Simply clone or download and extract the zipped library into your Arduino/libraries folder. The library is added as:

```C++
#include "ms4525do.h"
```

An example Arduino executable is located in: *examples/arduino/ms4525do_example/ms4525do_example.ino*. Teensy 3.x, 4.x, and LC devices are used for testing under Arduino and this library should be compatible with other Arduino devices.

## CMake
CMake is used to build this library, which is exported as a library target called *ms4525do*. The header is added as:

```C++
#include "ms4525do.h"
```

The library can be also be compiled stand-alone using the CMake idiom of creating a *build* directory and then, from within that directory issuing:

```
cmake .. -DMCU=MK66FX1M0
make
```

This will build the library and an example executable called *ms4525do_example*. The example executable source file is located at *examples/cmake/ms4525do_example.cc*. Notice that the *cmake* command includes a define specifying the microcontroller the code is being compiled for. This is required to correctly configure the code, CPU frequency, and compile/linker options. The available MCUs are:
   * MK20DX128
   * MK20DX256
   * MK64FX512
   * MK66FX1M0
   * MKL26Z64
   * IMXRT1062_T40
   * IMXRT1062_T41
   * IMXRT1062_MMOD

These are known to work with the same packages used in Teensy products. Also switching packages is known to work well, as long as it's only a package change.

Each target also has a *_hex*, for creating the hex file to upload to the microcontroller, and an *_upload* for using the [Teensy CLI Uploader](https://www.pjrc.com/teensy/loader_cli.html) to flash the Teensy. Please note that the CMake build tooling is expected to be run under Linux or WSL, instructions for setting up your build environment can be found in our [build-tools repo](https://github.com/bolderflight/build-tools).

# Namespace
This library is within the namespace *bfs*.

# Ms4525do

**Ms4525do()** Default construction, requires calling the *Config* method to setup the I2C bus, I2C address, and pressure range.

**Ms4525do(TwoWire &ast;bus, const uint8_t addr, const float p_max, const float p_min, const OutputType type = OUTPUT_TYPE_A)** Creates a Ms4525 object given a pointer to the I2C bus object, I2C address, maximum and minimum pressure range (PSI), and sensor output type. The available output types are:

| Enum | Description |
| --- | --- |
| OUTPUT_TYPE_A | Output type A |
| OUTPUT_TYPE_B | Output type B |

The output type is specified in the product number and defaults to OUTPUT_TYPE_A.

For example, the following code declares an Ms4525 object called *pres* with an +/-1 PSI transducer located on I2C bus 0 with an I2C address of 0x28:

```C++
bfs::Ms4525do pres(&Wire, 0x28, 1.0f, -1.0f);
```

**void Config(TwoWire &ast;bus, const uint8_t addr, const float p_max, const float p_min, const OutputType type = OUTPUT_TYPE_A)** This is required when using the default constructor and sets up the I2C bus, I2C address, pressure range, and output type.

**bool Begin()** Initializes communication with the sensor. True is returned if communication is able to be established, otherwise false is returned. The communication bus is not initialized within this library and must be initialized seperately; this enhances compatibility with other sensors that may on the same bus.

```C++
Wire.begin();
Wire.setClock(400000);
if (!pres.Begin()) {
  // ERROR
}
```

**bool Read()** Reads data from the sensor and stores the data in the Ms4525do object. Returns true if data is successfully read, otherwise, returns false.

```C++
/* Read the sensor data */
if (pres.Read()) {
}
```

**float pres_pa()** Returns the pressure data from the object in units of Pa.

```C++
float pressure = pres.pres_pa();
```

**float die_temp_c()** Returns the die temperature of the sensor from the object in units of degrees C.

```C++
float temperature = pres.die_temp_c();
```

**Status status()** Returns the latest status from the sensor. The status options are:

| Enum | Description |
| --- | --- |
| STATUS_GOOD | Normal Operation. Good Data Packet |
| STATUS_STALE_DATA | Stale Data. Data has been fetched since last measurement cycle. |
| STATUS_FAULT | Fault Detected |

# Example List
* **ms4525do_example**: demonstrates declaring an object, initializing the sensor, and collecting data. 
