/*
  Sonar2Analog

 Overview:
 the program can be used experimentaly on attaching a HC-SR04
 sonar sensor to Ardupilot 2.x.
 The sensor is getting about 15~20 samples/second and an analog 
 voltage is generated out of the R/C filter that can feed
 the Ardupilot sonar sensor pin.
 
 This code reads the sonar sensor HC-SR04 and generates a PWM
 signal on pin 5 at 32.5KHz PWM frequency. That in turn can be
 converted to analog voltage through a R/C filter.
 The program uses a slightly modified TinyNewPing library
 http://www.adafruit.com/blog/2013/09/24/from-the-forums-serial-sonar-sensor-attiny85-hc-sr04-and-arduino-softwareserial-arduino/
 
 
 Prerequisites
 The arduino-tiny https://code.google.com/p/arduino-tiny/
 core files library installed
 Also modified according to
 http://forum.arduino.cc/index.php/topic,142948.msg1073712.html#msg1073712
 
 
 This code is designed to run on the ATTiny 45
 
 The connections to the ATTiny are as follows:
 
 ATTiny      Info
 =======    =====================================================
 Pin  1      RESET connected to Vcc
 Pin  2      Sonar trigger
 Pin  3      Sonar echo
 Pin  4      GND
 Pin  5      PWM output to be fed on a RC filter (R=10K, C=1uF)
 Pin  6      Free - Not connected
 Pin  7      Free - Not connected
 Pin  8      +Vcc
 
 
 
 R/C filter connection:
 Pin 5 ---- Res 10K ---+------> Analog Output to Ardupilot sonar sensor input
                       |
                     Cap 1uF
                       |
                      GND
 
 This example code is in the public domain.


 Author:  Christos Nikolaou  sv1eia@gmail.com
 
 
 Disclaimer:
 This software is in public domain and given as is without any support.
 It highly experimental firmware and configuration setup, please use at your own
 responsibility.
 
 */

#define F_CPU 8000000
#define PWM_TABLE_ITEMS 18
#define PWM_CORR_FACTOR 72

#include <stdlib.h>
#include <avr/io.h>        
#include <util/delay.h>    

#include <UserTimer.h>
#include "TinyNewPing.h"

#define TRIGGER_PIN    3 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN       4 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 450 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


#define INTERNAL2V56NC (6)

const int pwm_out = 0;

int pwm_table[PWM_TABLE_ITEMS];
int pwm_table_counter = 0;


void setup()  { 

  UserTimer_SetToPowerup();
  UserTimer_SetWaveformGenerationMode( UserTimer_(Fast_PWM_FF) );
  UserTimer_ClockSelect( UserTimer_(Prescale_Value_1) );
  pinMode(pwm_out, OUTPUT);
  analogReference(INTERNAL2V56NC);
  delay(100);
  
} 

void loop()  { 
  
  int pwm_sum = 0, pwm_value = 0;
  pwm_table[pwm_table_counter] = (int)(sonar.ping() / PWM_CORR_FACTOR);
  for (int i =0; i<PWM_TABLE_ITEMS; i++) pwm_sum += pwm_table[i];
  pwm_value = pwm_sum / PWM_TABLE_ITEMS;
  pwm_table_counter++;
  if (pwm_table_counter > (PWM_TABLE_ITEMS - 1))    pwm_table_counter = 0;
    
  analogWrite(pwm_out, pwm_value);
  _delay_ms(45);  // make max 20 samples per second

}

