#include <EasyStepper.h>
#include <Time.h>  
#include <ClockAide.h>
 
////////////////////////////////////////////////////////////////////////
////////////////  Encoder  /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <PinChangeInt.h> // necessary otherwise we get undefined reference errors.
#include <AdaEncoder.h>

////////////////////////////////////////////////////////////////////////
////////////////  Encoder  /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#define a_PINA 2
#define a_PINB 3
#define b_PINA A2
#define b_PINB A3

int8_t clicks=0;
char id=0;

int h_counter = 0;
int m_counter = 0;

int h_count = 0;
int m_count = 0;

////////////////////////////////////////////////////////////////////////
////////////////  Stepper Motor  ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////

const int stepsPerRevolution = 200;
int opticalSensorPinHour = A4;
int opticalSensorPinMinute = A5;

EasyStepper hours(stepsPerRevolution, 7,6,5,4);   
EasyStepper minutes(stepsPerRevolution, 11,10,9,8);   


////////////////////////////////////////////////////////////////////////
////////////////  Serial Comm Link  ////////////////////////////////////
////////////////////////////////////////////////////////////////////////

int mode;
int temp = 0;

// Variables to recieve time from Raspberry Pi

int pi_hour;
int pi_mins;
int pi_secs;
int pi_day;
int pi_month;
int pi_year;

void setup() {
  //Initialize serial communication
  Serial.begin(9600);
  
  //intialize optical sensor pins as input
  pinMode(opticalSensorPinHour, INPUT);
  pinMode(opticalSensorPinMinute, INPUT);
  
  // set the speed at 10 rpm:
  hours.setSpeed(10);
  minutes.setSpeed(10);
  
  //initialize motors and find Zero position
  minutes.findZero(opticalSensorPinMinute);
  hours.findZero(opticalSensorPinHour);  
   
  // Initialize the Encoders
  AdaEncoder::addEncoder('a', a_PINA, a_PINB);
  AdaEncoder::addEncoder('b', b_PINA, b_PINB);  

// Wait for time from Raspbery Pi
  while(Serial.available() == 0);

  // Get Time from Raspberry Pi
  while(Serial.available() > 0){
    pi_hour  = Serial.parseInt();
    pi_mins  = Serial.parseInt();
    pi_secs  = Serial.parseInt();
    pi_day   = Serial.parseInt();
    pi_month = Serial.parseInt();
    pi_year  = Serial.parseInt();
  }
   
  setTime(pi_hour, pi_mins, pi_secs, pi_day, pi_month, pi_year);

  // Empty Serial Buffer 
  while (Serial.read() >= 0);
 
  //setTime(14, 35, 9, 12, 12, 2013);       
}

void loop()
{
    switch(mode){
       case NORMAL:            // Normal Mode
             minutes.normalMinute(minute());
             hours.normalHour(hourFormat12(),minute());
          
             if(Serial.available() > 0){
               mode = Serial.parseInt();
               
             }
       break; 
             
       case CHECK_ID:            // Check ID
             minutes.normalMinute(minute());
             hours.normalHour(hourFormat12(),minute());
          
             if(Serial.available() > 0){
               mode = Serial.parseInt();
             }             
       break;
       
       case READ:            // Read Mode
             minutes.findZero(opticalSensorPinMinute);
             hours.findZero(opticalSensorPinHour); 
                          
             // Wait for time from Raspbery Pi
             while(Serial.available() == 0);                
             // Get Time from Raspberry Pi
             while(Serial.available() > 0){
                 pi_hour  = Serial.parseInt();
                 pi_mins  = Serial.parseInt();
                 // Empty Serial Buffer 
                 while (Serial.read() >= 0);
             }
             //Set Hands
             minutes.normalMinute(pi_mins);
             hours.normalHour(pi_hour, pi_mins);
             
             while(Serial.available() == 0);
             
             if(Serial.available() > 0){
               mode = Serial.parseInt();               
             }
             minutes.findZero(opticalSensorPinMinute);
             hours.findZero(opticalSensorPinHour); 
       break;
       
       case SET:             // Set Mode
             minutes.findZero(opticalSensorPinMinute);
             hours.findZero(opticalSensorPinHour); 
             
             h_counter;
             m_counter;
             
             while(mode == SET){                    
                   encoder *thisEncoder;
                   thisEncoder=AdaEncoder::genie(&clicks, &id);
                   if (thisEncoder != NULL) {
                      thisEncoder=AdaEncoder::getFirstEncoder();
          
                      if (clicks > 0) {
                        if (id == 'a')
                        {
                            h_counter++;
                            hours.updateMotorPosition(-1);
                         }
                        else
                        {
                            m_counter++;
                            minutes.updateMotorPosition(-1);
                        }
                      }
                      if (clicks < 0) {
                        if (id == 'a')
                        {
                            h_counter--;
                            hours.updateMotorPosition(1);
                        }
                        else
                        {
                            m_counter--;
                            minutes.updateMotorPosition(1);                  
                        }
                       }
                    }
                    
                    if(h_counter<0 && h_counter>(-200)){
                      h_count = h_counter + 200;
                    }
                    else {
                      h_count = h_counter;
                    }
                    
                    if(m_counter<0 && m_counter>(-200)){
                      m_count = m_counter + 200;
                    }
                    
                    else {
                      m_count = m_counter;
                    }
                      
                    //Serial.println(h_count);
                    //Serial.println(hours.calculateHourPosition(h_count%200));
                    
                    //Serial.println(m_count);
                    //Serial.println(minutes.calculateMinutePosition(m_count%200));
                  
                    // Empty Serial Buffer 
                   //while (Serial.read() >= 0);
                   
                   while(Serial.available() > 0){
                     mode = Serial.parseInt();
                    
                      minutes.findZero(opticalSensorPinMinute);
                      hours.findZero(opticalSensorPinHour);
                   }
                  
                   
             }  
       break;
       
       default:
             mode = NORMAL;// Set mode to Normal mode
       break;
    }
}

