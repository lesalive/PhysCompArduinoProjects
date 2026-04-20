/*
 Lisa Lai
 DIGF-2002
 OCAD University
 Based on:
 YouveBeenNotified Examples
 01 RTC Minute Tracker using LED Matrix
 03 RTCservo Two Servos Example
 https://github.com/DigitalFuturesOCADU/YouveBeenNotified 
 */

// Include libraries
 #include "RTC.h"
 #include <Servo.h>
 #include "YouveBeenNotified.h"
 #include "ArduinoGraphics.h"
 #include "Arduino_LED_Matrix.h"
 ArduinoLEDMatrix matrix;

// Variables and things
 Servo servo1; // Create first servos and notifier
 ServoNotifier notifier1(servo1);
 Servo servo2; // Create second servo and notifier
 ServoNotifier notifier2(servo2);

 float playSpeed = 1.0; //scale animation speed (changed from 2.0 to 1.0)

 // Animation names
 String anim1Name = "Fast";
 String anim2Name = "slowingWave";
 String hitName = "lightswitch";

// Variables for time keeping
 int currentMinute;         
 int currentSecond;
 int lastMinute = -1;
 int lastSecond = -1;

// Variables for LED matrix; 5 secs per LED, 8 minutes to fill 96 matrix, 7.5 cycles per hour
 int ledX = -1;              // X position (0-11)
 int ledY = 0;              // Y position (0-7)
 int updateInterval = 5;    // LED interval
 int full = 0;              // track number of completed cycles

//----- Start of void Setup
void setup() 
{
  // Initialize serial and RTC
  Serial.begin(9600);
  Serial.println("Starting setup...");
  RTC.begin();  
  RTCTime initialTime(04, Month::APRIL, 2025, 10, 0, 0, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(initialTime);

  // Initialize LED matrix
    if (!matrix.begin()) 
    {
      Serial.println("Failed to initialize LED matrix!");
      while (1);
    }
  // LED Matrix
   matrix.beginDraw();
   matrix.clear();
   matrix.endDraw();
   updateLED();
   
  // Connect servos to pins
  servo1.attach(9);
  servo2.attach(10);
  
  setupServo1Animations(); // Function to setup servo 1 animation
  setupServo2Animation(); // Function to setup servo 2 animation
  
  // Set the playSpeed
  notifier1.setGlobalSpeed(playSpeed);
  notifier2.setGlobalSpeed(playSpeed);
  
  // Start the animation on Servo 2
  notifier2.playAnimation(hitName, LOOP);

} // End of void Setup

// Function to setup animations for servo 1
void setupServo1Animations() 
{
  Serial.println("Setting up animations for Servo 1...");
  
  KeyframeAnimation anim1(anim1Name); // fast wave
  anim1.addKeyFrame(0, 0);       
  anim1.addKeyFrame(180, 1500);   
  anim1.addKeyFrame(0, 3000);    
  
  KeyframeAnimation anim2(anim2Name); // slowing wave
  anim2.addKeyFrame(0, 0);      
  anim2.addKeyFrame(180, 1500 + full*500);   
  anim2.addKeyFrame(0, 3000 + full*500);    
  
  // Add animations to notifier
  notifier1.addAnimation(anim1);
  notifier1.addAnimation(anim2);
}

// Function to setup animation for servo 2
void setupServo2Animation() 
{
  Serial.println("Setting up animation for Servo 2...");
  
  KeyframeAnimation hitlights(hitName);
  hitlights.addKeyFrame(60, 0);      
  hitlights.addKeyFrame(30, 5000);  
  hitlights.addKeyFrame(0, 1000); 
  hitlights.addKeyFrame(60, 30000); 
  hitlights.addKeyFrame(60, 60000);
  notifier2.addAnimation(hitlights);  // Add animation to notifier
}

void loop() // Start of void Loop
{
  // Update both animations
  notifier1.update();
  notifier2.update();
  
  // Get current time
  RTCTime currentTime;
  RTC.getTime(currentTime);
  currentMinute = currentTime.getMinutes(); 
  currentSecond = currentTime.getSeconds();

  // Check if minute for servo 1
  if (currentMinute != lastMinute) 
  {
    lastMinute = currentMinute;
    if (currentMinute % 5 == 0) {
      // Matching minute
      notifier1.playAnimation(anim1Name, LOOP);
      Serial.print("Minute ");
      Serial.print(currentMinute);
    } else 
    {
      // Other minutes
      notifier1.playAnimation(anim2Name, LOOP);
      Serial.print("Minute ");
      Serial.print(currentMinute);
    }
  }
  
  // Update servo 1 if position has changed
  if (notifier1.hasChanged()) 
   {
      int value1 = notifier1.getValue();
      servo1.write(value1); //Serial.print("Servo 1 position: "); Serial.println(value1);
    }
  
  // Update servo 2 if position has changed
  if (notifier2.hasChanged()) 
    {
      int value2 = notifier2.getValue();
      servo2.write(value2); //Serial.print("Servo 2 position: "); Serial.println(value2);
    }

// Check if the second has changed to update the LED matrix
 if (currentSecond != lastSecond) 
 {
   lastSecond = currentSecond;
   if (currentSecond % updateInterval == 0) // update matrix at the interval
   { 
     ledX++;
     if (ledX >= matrix.width()) 
     {
       ledX = 0; ledY++;
       if (ledY >= matrix.height()) // reset and clear matrix when full
       {
         ledX = 0;
         ledY = 0;
         matrix.beginDraw();
         matrix.clear();
         matrix.endDraw();
         full++;   
         Serial.println(full);
       }
     }  
     updateLED(); // function to update LED matrix
    }
  }

} // End of void Loop

// Function to update LED matrix to current LED position
void updateLED() 
{
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF); // Set color to "on"
  matrix.point(ledX, ledY);
  matrix.endDraw();
  // Print update message
  Serial.println("---------------------");
  Serial.print("LED at ");
  Serial.print(currentMinute);
  Serial.print(":");
  Serial.println(currentSecond);

  Serial.println(notifier1.getCurrentAnimationName());
}