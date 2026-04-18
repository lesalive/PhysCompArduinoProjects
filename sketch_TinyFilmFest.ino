/*
 Tiny Film Festival
 Lisa Lai
 DIGF-2002
 OCAD University
  Based on 
 * TinyFilmFestival - Pressure-Based Animation Switch Example
 */

#include "TinyFilmFestival.h"

#include "Panda_Happy.h"
#include "Panda_OW.h"
#include "Panda_Idle.h"

// Create instance of TinyFilmFestival
TinyFilmFestival film;

// Create Animation objects
Animation idleAnim = Panda_Idle;
Animation petAnim = Panda_Happy;
Animation madAnim = Panda_OW;

// Pins and variables
const int mushPin = A0;             // Analog input pin for pressure sensor
const int mushLimit1 = 40;      
const int mushLimit2 = 400;     
int mushValue = 0;                  // Variable to store pressure reading

void setup() 
{
    Serial.begin(9600);
    film.begin();
    
    // Setup pressure sensor pin
    pinMode(mushPin, INPUT);
    
    // Start with idle animation
    film.startAnimation(idleAnim, LOOP);
    film.setSpeed(750);

}

void loop() 
{  
mushValue = analogRead(mushPin); 

    if (mushValue > mushLimit1 && mushValue < mushLimit2 ) 
    {
        film.startAnimation(petAnim, LOOP);
        Serial.println("Happy");
        Serial.println(mushValue);
    } 
  
      else 
     {
         if (mushValue > mushLimit2) 
        {
         film.startAnimation(madAnim, LOOP);
         Serial.println("Sad");
         Serial.println(mushValue);
        } 
          else 
          {
            film.startAnimation(idleAnim, LOOP);
            Serial.println("Playing Idle");
            Serial.println(mushValue);
          }
      }
    
    film.update();
  }
