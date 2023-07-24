# Xiao Sense BT 

  This repo connects uses a seeed studio sense to provide streaming measurements of accelerometer and gyro data from the on board IMU.  The data can be received by any BLE compatible host.


To make this repo work you must build and flash the firmware and then capture the data over BLE.

* Firmware - minimal written C++ with the arduino toolchain. The firmware does not perform any power management, so as long as there is power from either USB-C or a battery it will attempt to connect over BLE and stream the data.
	- stream  accel / gyro over BLE 

* Web Client - using the BLE support in the chrome browser the client graphs and shows current instantaeoous values of the IMU in near real-time.
	- uses direct connection from the board via BLE in the browser
	- live charts of the accel and gyro data (raw)
	- export all the session data as a JSON


## Compiling the Firmware
Install the Arduino IDE.  

Then get the right board support package (BSP).  The core board used here is the Seeed Studio Xiao BLE Sense.

Open the File > Preferences and add the following URL and fill in the Additional Board Manager URLs"
[Seeed Xiao BLE BSP](https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json)

Then go to Tools > Board > Board Manager and search for "seeed nrf52".  Add the Seeed Studio Xiao NRF52840 Sense.

Now go to Tools > Board > Seeed nRF Boards > Seeed Xiao BLE Sense nRF52840.

In order to communicate with Bluetooth 5 devices, we will use the Ardiuno BLE library.  Go to Library Manager and pic ArduinoBLE from the list and click install.

At this point you should be able to compile code for the bands which don't have the display.


## Viewing the browser
The webpage does not depend on a server side per se.  However to allow BLE to be permitted in chrome one must run this on either localhost or the page must be run from an https:// enabled web site.

run with python 3.6_
```sh
cd app
python3 -m http.server 8000 
```

or run with nodejs
```sh
cd app
npx serve
```

open localhost:8080/index.html in the chrome browser (chrome supports BLE extensions uses)



## Electrical Design
The hardware is based on the Nordic NRF52840 BLE system on chip with support for BLE v5.  Accelerometer and Gyro information is provided by ST Semiconductor LSM6DS3TR.   A bqBQ25101 charge management chip manages the 150 mAH LiIon battery.

All programming and charging take place over a USB-C connection.

## Revision history
June 2022 - M A Chatterjee - initial fw, webpage, hw design
Aug  2022 - M A Chatterjee - clean up checking to git



