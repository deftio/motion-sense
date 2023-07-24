/*****************
  Sense BT
  
  M A Chatterjee

  supports streaming accel & gyro information for gesture reco via BLE 5.

  See README.md in this repo for more details.

  This fw for Xiao BLE Sense
  =======
  revisions
  2022-06-20 started..

 ****************/
 
#include "Wire.h"

#include "LSM6DS3.h"
#include <ArduinoBLE.h>

#define DBG_STREAM_SERIAL (false)
#define FWVERSION (0x00001001)

#define SENSE_BT(val)         ("19701201-" val "-ab15-cd17-1f3e5d7e2020")
BLEService                     service            (SENSE_BT("0000"));
BLEUnsignedIntCharacteristic   versCharacteristic (SENSE_BT("1001"), BLERead);
BLECharacteristic              acclCharacteristic (SENSE_BT("4001"), BLENotify, 3 * sizeof(float)); // 3 floats in units of G 
BLECharacteristic              gyroCharacteristic (SENSE_BT("5001"), BLENotify, 3 * sizeof(float)); // 3 floats in units of deg / s
BLECharacteristic              magsCharacteristic (SENSE_BT("6001"), BLENotify, 3 * sizeof(float)); // 3 floats in units of gauss (not avail all boards)
  
const int statusLED = LED_BUILTIN; // pin to use for the LED

int showDebugSerial = DBG_STREAM_SERIAL;

//Create a instance of class LSM6DS3
LSM6DS3 lms6DS_IMU(I2C_MODE, 0x6A);    //I2C device address 0x6A
 

// note 
void setup() {
  // LED pin to output mode
  pinMode(statusLED,OUTPUT);  
  
  digitalWrite(statusLED, LOW);
 
  Serial.begin(115200);
  delay(333);
  
  // begin initialization
  if (!BLE.begin()) {
    if (Serial)
      Serial.println("starting BLE service failed!");
    while (1);
  }
 
 

  String address = BLE.address();
  address.toUpperCase();
  String  name = "xp_sense_ble";
  
  if (Serial) {
    Serial.print("address = ");
    Serial.println(address);
    Serial.print("device_name = ");
    Serial.println(name);
  }

  
  BLE.setLocalName(name.c_str());
  //BLE.setDeviceName(name.c_str());


  // now set up the BLE services (characteristics)

  versCharacteristic.setValue(FWVERSION);
  
  service.addCharacteristic(versCharacteristic);
  service.addCharacteristic(acclCharacteristic);
  service.addCharacteristic(gyroCharacteristic);
  

  //xcmdCharacteristic.writeValue(xmcdBytes, sizeof(xmcdBytes));

  BLE.addService(service);
  // start advertising
  BLE.advertise();

  if (Serial) {
    Serial.println("Setting up IMU");
  }
  if (lms6DS_IMU.begin()!= 0) {
    if (Serial)
      Serial.println("IMU setup Error");
  }
  else {
    if (Serial)
      Serial.println("IMU ready"); 
  }
}

int x = HIGH;

void loop() {
  digitalWrite(statusLED, LOW); // turn off until connected
   
  BLEDevice central = BLE.central();
 
  if (central) {
    if (Serial) {
      Serial.print("Connected to central: ");
      // print the central's MAC address:
      Serial.println(central.address());
    }
 
   while (central.connected()) {
      digitalWrite(statusLED, x); // set LED to blink when transmitting data.  comment out to save power
      x = x== HIGH ? LOW : HIGH;
  
      if (showDebugSerial) {
         showDebugToSerial();
      }
      if (acclCharacteristic.subscribed() ) {
  
        float accl[3] = { lms6DS_IMU.readFloatAccelX(), lms6DS_IMU.readFloatAccelY(), lms6DS_IMU.readFloatAccelZ() };
  
        acclCharacteristic.writeValue(accl, sizeof(accl));
      }
      delay(1);
      if (gyroCharacteristic.subscribed() ) {
  
        float gyro[3] = { lms6DS_IMU.readFloatGyroX(), lms6DS_IMU.readFloatGyroY(), lms6DS_IMU.readFloatGyroZ() };
  
        gyroCharacteristic.writeValue(gyro, sizeof(gyro));
      } 
      delay(1);  
    }
  } 
  if (Serial) {
    Serial.println("not connected");
  }
  BLE.advertise();
  
}

void showDebugToSerial() {
    if (Serial) {
        //Accelerometer
      Serial.print("\nAccl :\n");
      Serial.print(" x:");
      Serial.println(lms6DS_IMU.readFloatAccelX(), 4);
      Serial.print(" y: ");
      Serial.println(lms6DS_IMU.readFloatAccelY(), 4);
      Serial.print(" z: ");
      Serial.println(lms6DS_IMU.readFloatAccelZ(), 4);

      //Gyroscope
      Serial.print("\nGyroscope:\n");
      Serial.print(" x:");
      Serial.println(lms6DS_IMU.readFloatGyroX(), 4);
      Serial.print(" y: ");
      Serial.println(lms6DS_IMU.readFloatGyroY(), 4);
      Serial.print(" z: ");
      Serial.println(lms6DS_IMU.readFloatGyroZ(), 4);

      //Thermometer
      Serial.print("\nTemp:\n");
      Serial.print(" Degrees C1 = ");
      Serial.println(lms6DS_IMU.readTempC(), 4);
      Serial.print(" Degrees F1 = ");
      Serial.println(lms6DS_IMU.readTempF(), 4);
      delay(2);
    }
}
