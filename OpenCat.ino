/*
   The driver for OpenCat, runs on ATmega328P-based NyBoard (as Arduino Uno).
   Compatible with Petoi Nybble, Bittle, and many other 8 or 12 DoF quadruped robots.
   Drives up to 16 PWM servos.

   Rongzhong Li
   Mar 15rd, 2022
   Copyright (c) 2022 Petoi LLC.

   This sketch may also include others' codes under MIT or other open-source licenses.
   Check those licenses in the corresponding module test folders.
   Feel free to contact us if you find any missing references.

  The MIT License

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

/*
  To configure the board:
  1. Select your robot and board version.
  2. Comment out #define MAIN_SKETCH and upload. Upload and follow the serial prompts to proceed.
  3. Uncomment #define MAIN_SKETCH to make it active. Then upload the program for main functions.
*/

//#define MAIN_SKETCH //the Petoi App only works when this mode is on
//#define AUTO_INIT //automatically select 'Y' for the reset joint and IMU prompts
//#define DEVELOPER //to print out some verbose debugging data

//#define BITTLE    //Petoi 9 DOF robot dog: 1x on head + 8x on leg
#define NYBBLE  //Petoi 11 DOF robot cat: 2x on head + 1x on tail + 8x on leg

//#define NyBoard_V0_1
//#define NyBoard_V0_2
//#define NyBoard_V1_0
//#define NyBoard_V1_1
//#define NyBoard_V1_2
#define M5AtomBoard

//you can also activate the following modes (they will disable the gyro to save programming space)
//allowed combinations: RANDOM_MIND + ULTRASONIC, RANDOM_MIND, ULTRASONIC, VOICE, CAMERA
//#define RANDOM_MIND     //advanced random behaviors. use token 'z' to activate/deactivate
//#define ULTRASONIC      //for Nybble's ultrasonic sensor
//#define VOICE           //for LD3320 module
//#define CAMERA          //for BallTracking using Mu Vision camera
//You need to install https://github.com/mu-opensource/MuVisionSensor3 as a zip library in Arduino IDE.
//Set the four dial switches on the camera as **v ^ v v** (the second switch dialed up to I2C) and connect the camera module to the I2C grove on NyBoard.
//The battery should be turned on to drive the servos.
//
//You can use these 3D printed structures to attach the camera module.
//https://github.com/PetoiCamp/NonCodeFiles/blob/master/stl/MuIntelligentCamera_mount.stl
//https://github.com/PetoiCamp/NonCodeFiles/blob/master/stl/bone.stl
//After uploading the code, you may need to press the reset buttons on the module and then the NyBoard.
//The tracking demo works the best with a yellow tennis ball or some other round objects. Demo: https://www.youtube.com/watch?v=CxGI-MzCGWM


#include "src/OpenCat.h"

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.setTimeout(2);
#ifdef M5AtomBoard
  M5.begin(true, true, true); //Init Atom-Matrix(Initialize serial port, LED matrix).  
#else  
  // join I2C bus (I2Cdev library doesn't do this automatically)
  //#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  //  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
  Wire.setClock(500000L);
  //#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  //  Fastwire::setup(400, true);
  //#endif
#endif
  //----------------------------------
#ifdef MAIN_SKETCH  // **
  PTL('k');
#ifdef GYRO_PIN
  imuSetup();
#endif */ GYRO_PIN */
#ifdef IR_PIN
  irrecv.enableIRIn(); // Start the receiver
  gait.reserve(4);
#endif /* IR_PIN */

#ifdef LED_PIN
  pinMode(LED_PIN, OUTPUT);
#endif
#ifdef NEOPIXEL_PIN
  //  pixel.begin();           // INITIALIZE NeoPixel pixel object (REQUIRED)
  //  pixel.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
#endif

  servoSetup();
  skill.assignSkillAddressToOnboardEeprom();
#ifdef RANDOM_MIND
  for (byte i = 0; i < randomMindListLength; i++) {
    randomBase += choiceWeight[i];
  }
#endif

#ifdef M5AtomBoard
  randomSeed(esp_random());
#else
  randomSeed(analogRead(A2));//use the fluctuation of voltage caused by servos as entropy pool
#endif

#ifdef VOICE
  voiceSetup();
#endif
#ifdef CAMERA
  cameraSetup();
#endif

  playMelody(MELODY_NORMAL);

  delay(2000);//change the delay if the app doesn't recognize the Petoi device.
#ifdef GYRO_PIN
  for (byte r = 0; r < 3; r++)
    read_IMU();  //ypr is slow when starting up. leave enough time between IMU initialization and this reading
  token = (fabs(ypr[1]) > 30 || fabs(ypr[2]) > 30) ? T_CALIBRATE : T_REST; //put the robot's side on the table to enter calibration posture for attaching legs
  newCmdIdx = 2;
#endif
  PTLF("\n* Start *");
#ifdef BITTLE
  PTLF("Bittle");
#elif defined NYBBLE
  PTLF("Nybble");
#elif defined CUB
  PTLF("Cub");
#endif
  //----------------------------------
#else               // ** save parameters to device's static memory
  configureEEPROM();
  servoSetup(); //servo needs to be after configureEEPROM and before imuSetup
#ifdef GYRO_PIN
  imuSetup();
#endif
#endif              // **
  PTLF("Ready!");
}

void loop() {
#ifdef MAIN_SKETCH
#ifdef VOLTAGE_DETECTION_PIN
  lowBattery(); //  block the loop if battery is low
  //  can be disabled to save programming space and reduce low voltage interruptions
#endif
#ifdef GYRO_PIN
  readEnvironment();
  dealWithExceptions(); //fall over, lifted, etc.
#endif
  readSignal();
  reaction();
#endif
}
