# Building
Familiarise yourself with [Raspberry Pi's guide on the C SDK](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html#quick-start-your-own-project). You will need to install the required packages, and download the Pico SDK to pass to cmake.

Then, run the following commands, replacing `path/to/pico-sdk` with the actual path:
```bash
mkdir build && cd build
PICO_SDK_PATH=path/to/pico-sdk cmake -DPICO_BOARD=pico_w ..
make rios
```

Then, upload the rios.uf2 firmware file to your Pico!