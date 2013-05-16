#include <SoftwareSerial.h>
#include <Keypad.h>
#include <Time.h> 
#include <LcdDisplay.h>
#include <ClockAide.h>

// Declare LcdDisplay Object to display text on LCD Display
LcdDisplay myLCD = LcdDisplay();

/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////   Keypad Stuff  /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

char keyStroke;
char option;
char temp;
String timeNow = "";

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'7','8','9'},
  {'4','5','6'},
  {'1','2','3'},
  {'#','0','*'}
};
byte rowPins[ROWS] = {9, 10, 11, 12}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////   Serial Comm Stuff  ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
int mode;
int response;
String Name = "";


// Variables to recieve time from Raspberry Pi

int pi_hour;
int pi_mins;
int pi_secs;
int pi_day;
int pi_month;
int pi_year;

/////////////////////////////////////////////////////////////////////////////////////////////////
int led = 13;

void setup()
{
    /*
        Initialize serial communication with R. Pi
        Default mode = Normal       
    */
    pinMode(led, OUTPUT);
    Serial.begin(9600);
    delay(500);
    myLCD.clearLCD();
    myLCD.writeToScreen("ClockAide",0,0);
    myLCD.writeToScreen("Loading...",1,0);
    digitalWrite(led, HIGH);
    while(Serial.available() == 0);
    digitalWrite(led, LOW);
    // Get Time from Raspberry Pi
    while (Serial.available() > 0){
        pi_hour  = Serial.parseInt();
        pi_mins  = Serial.parseInt();
        pi_secs  = Serial.parseInt();
        pi_day   = Serial.parseInt();
        pi_month = Serial.parseInt();
        pi_year  = Serial.parseInt();
        while (Serial.read() >= 0);        
    }
    setTime(pi_hour, pi_mins, pi_secs, pi_day, pi_month, pi_year);
      
    
    // setTime(16, 3, 0, 10, 3, 2013);

}



void loop()
{
    
    switch(mode){
       case NORMAL:            // Normal Mode
             //myLCD.clearLCD();
             {
             
             if(hourFormat12() < 10){
                 if(minute()<10){
                     myLCD.writeToScreen("ClockAide  0" + String(hourFormat12()) + ":0" + String(minute()), 0,0);
                     myLCD.writeToScreen("Press Any Button", 1,0);
                 }
                 else{
                     myLCD.writeToScreen("ClockAide  0" + String(hourFormat12()) + ":" + String(minute()), 0,0);
                     myLCD.writeToScreen("Press Any Button", 1,0);
                 }
             }
             else{
                 if(minute()<10){
                     myLCD.writeToScreen("ClockAide  " + String(hourFormat12()) + ":0" + String(minute()), 0,0);
                     myLCD.writeToScreen("Press Any Button", 1,0);
                 }
                 else{
                     myLCD.writeToScreen("ClockAide  " + String(hourFormat12()) + ":" + String(minute()), 0,0);
                     myLCD.writeToScreen("Press Any Button", 1,0);
                 }
             }

             // Wait for button to be pressed
             char p = '\0';
             p = keypad.getKey();
             while (p != '\0'){
                 if(p == '*' || p == '#'){  
                     // Send "Wake-up" Message to Pi
                     // sendToPi(p);
                     sendToPi(WAKE_UP);
                     //sendToPi('a');                 
                     // Get Mode from Pi
                     p = '\0';
                     mode = getFromPi();
                 }
                 
                 else{
                    sendToPi(SPEAK_TIME);
                    p = '\0';
                    mode = getFromPi(); 
                 }
             }
             }
       break; 
             
       case CHECK_ID:            // Check ID 
             myLCD.clearLCD();
             myLCD.writeToScreen("Enter ID#:", 0,0);
             myLCD.selectLineTwo();
             // Send ID to Pi
             Serial.print(getID());
             response = getFromPi();
             if (response == GOOD){
                 myLCD.clearLCD();
                 myLCD.writeToScreen("Welcome", 0,0);
                 Name = getNameFromPi();
                 myLCD.writeToScreen(Name, 1,0);
                 delay(3000);
                 myLCD.clearLCD();
                 myLCD.writeToScreen("Choose Mode:", 0,0);
                 myLCD.writeToScreen("(1)Read (2)Set", 1,0);
                 
                 char m = keypad.waitForKey();
                 
                 if (m=='1'){
                     sendToPi(READ);
                     mode = getFromPi();
                 }
                 else if (m == '2'){
                     sendToPi(SET);
                     mode = getFromPi();
                 }
                 else{
                      myLCD.clearLCD();
                      myLCD.writeToScreen("Invalid Mode", 0,0);
                      myLCD.writeToScreen("Start Again", 1,0);
                      mode = 0;
                      delay(5000);
                 }
                 
             }
             else{
                 myLCD.clearLCD();
                 myLCD.writeToScreen("Invalid ID", 0,0);
                 myLCD.writeToScreen("Start Again", 1,0);
                 mode = 0;
                 delay(5000);               
             }
             
       break;
       
       case READ:            // Read Mode
            myLCD.clearLCD();
            myLCD.writeToScreen("Enter time HH:MM", 0,0);
            myLCD.selectLineTwo();
            Serial.print(getTime());
            
            // Recieve message from PI
            response = getFromPi();
            if (response == GOOD){            // Recieve name from PI
                 myLCD.clearLCD();
                 myLCD.writeToScreen("Good Job", 0,0);
                 //Name = getNameFromPi();
                 myLCD.writeToScreen(Name, 1,0);            
            }
            else{
                 myLCD.clearLCD();
                 myLCD.writeToScreen("That's Wrong", 0,0);
                 myLCD.writeToScreen("Try Again", 1,0);
            }
            mode = getFromPi();
            
       break;
       
       case SET: 
             // Set Mode
             {
             char p = '\0';
             myLCD.clearLCD();
             myLCD.writeToScreen("Set Mode", 0,0);
             // Recieve time from Pi
             while(Serial.available() == 0);
             while(Serial.available()>0){
                  pi_hour = Serial.parseInt();
                  pi_mins = Serial.parseInt();
                  while (Serial.read() >= 0); // Empty serial buffer
             }
             myLCD.clearLCD();
             
             if (pi_hour > 12){
                 myLCD.writeToScreen("Turn knobs:" + String(pi_hour%12) + ":" + String(pi_mins), 0,0);
                 myLCD.writeToScreen( "Press Enter", 1,0);
             }
             else{
                 myLCD.writeToScreen("Turn knobs:" + String(pi_hour) + ":" + String(pi_mins) , 0,0);
                 myLCD.writeToScreen("Press Enter", 1,0);
             }
             // Wait for button to be pressed
             p = keypad.waitForKey();
             if(p){
                 sendToPi(GET_TIME);
                 myLCD.clearLCD();
                 myLCD.writeToScreen("Processing...", 0,0);
                 
                 response = getFromPi();
                 if (response == GOOD){            // Recieve name from PI
                     myLCD.clearLCD();
                     myLCD.writeToScreen("Good Job", 0,0);
                     //Name = getNameFromPi();
                     myLCD.writeToScreen(Name, 1,0);            
                 }
                 else{
                 myLCD.clearLCD();
                 myLCD.writeToScreen("That's Wrong", 0,0);
                 myLCD.writeToScreen("Try Again", 1,0);
            }
             }
            else{
                 myLCD.clearLCD();
                 myLCD.writeToScreen("That's Wrong", 0,0);
                 myLCD.writeToScreen("Try Again", 1,0);
            }
           
             mode = getFromPi();            
             }
       break;
             
       case TEACHER: 
             // TEACHER Mode
             myLCD.clearLCD();
             myLCD.writeToScreen("TEACHER MODE", 0,0);
       break;
       
       default:
             mode = NORMAL;// Set mode to Normal mode
       break;
    }
}

String getID(){
   String k = "";
   int i = -1;
   while(i<0){
       keyStroke = keypad.getKey();  
          switch(keyStroke){
            
            case '#':                                  //Clear button
                myLCD.clearLCD();
                myLCD.writeToScreen("Enter ID #:", 0,0);
                myLCD.selectLineTwo();
                k = "";
            break;  
           
            case '*':                                  //Enter Button
                i = 1;                
            break;
                        
            default:                                    //Every other Button
                k = k + (String) keyStroke; 
                myLCD.writeText((String)keyStroke);   
            break;
          }
   }
     
   return k;        
}

String getTime(){
  char a = '_';
  char b = '_';
  char c = '_';
  char d = '_';
  char e;
  int pos = 0;
  //String t = 
  myLCD.writeToScreen(String(a) + String(b) +":"+ String(c)+String(d), 1,0);
  while (pos < 5){
      switch(pos){
         case 0:
              a = keypad.waitForKey();
              if(a == '#' || b == '*'){
                a = '_';
                b = '_';
                c = '_';
                d = '_';
                
                myLCD.writeToScreen(String(a) + String(b) +":"+ String(c)+String(d), 1,0);
                pos = 0;
              }
              else{
                myLCD.writeToScreen(String(a) + String(b) +":"+ String(c)+String(d), 1,0);          
                pos++;
              }
         break;
         
         case 1:
              b = keypad.waitForKey();
              if(b == '#' || b == '*'){
                a = '_';
                b = '_';
                c = '_';
                d = '_';
                
                myLCD.writeToScreen(String(a) + String(b) +":"+ String(c)+String(d), 1,0);
                pos = 0;
              }
              else{
                myLCD.writeToScreen(String(a) + String(b) +":"+ String(c)+String(d), 1,0);          
                pos++;
              }
         break;
         
         case 2:
              c = keypad.waitForKey();
              if(c == '#' || c == '*'){
                a = '_';
                b = '_';
                c = '_';
                d = '_';
                
                myLCD.writeToScreen(String(a) + String(b) +":"+ String(c)+String(d), 1,0);
                pos = 0;
              }
              else{
                myLCD.writeToScreen(String(a) + String(b) +":"+ String(c)+String(d), 1,0);          
                pos++;
              }     
         break;
         
         case 3:
              d = keypad.waitForKey();
              if(d == '#' || d == '*'){
                a = '_';
                b = '_';
                c = '_';
                d = '_';
                
                myLCD.writeToScreen(String(a) + String(b) +":"+ String(c)+String(d), 1,0);
                pos = 0;
              }
              else{
                myLCD.writeToScreen(String(a) + String(b) +":"+ String(c)+String(d), 1,0);          
                pos++;
              }     
         break;
         
         case 4:
              e = keypad.waitForKey();
              if(e == '#'){
                a = '_';
                b = '_';
                c = '_';
                d = '_';
                
                myLCD.writeToScreen(String(a) + String(b) +":"+ String(c)+String(d), 1,0);
                pos = 0;
              }
              else if (e == '*'){
                myLCD.writeToScreen(String(a) + String(b) +":"+ String(c)+String(d), 1,0);          
                pos++;
              }     
         break;
         
         default:
               pos = 0;     
         break;
  }
    
  }
     
   
 return (String(a) + String(b) +","+ String(c) + String(d)); // return time string 
  
}

void sendToPi(int c){
      Serial.print(c); 
}

int getFromPi(){
      int temp;
      while(Serial.available() == 0);
      while(Serial.available()>0){
           temp = Serial.parseInt();
      }
      
      return temp;  
}

String getNameFromPi(){
       String name = "";
       while(Serial.available() == 0);
    
       while (Serial.available()){
         char t_display = Serial.read();
         name += (String) t_display;
       }
       return name;
}
