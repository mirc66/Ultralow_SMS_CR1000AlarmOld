//update 2/23/17 9am
//temp comment out use my phone for alarm reset 
//changed Chip phone for mine for old crappy test of alarm texting.

//last update 2/22/2017 12:08 changed from 5 min to 15 min
// 7/20/2018 currently running program offset the calls
/***************************************************
 * 

  This is an example for our Adafruit FONA Cellular Module

  Designed specifically to work with the Adafruit FONA
  ----> http://www.adafruit.com/products/1946
  ----> http://www.adafruit.com/products/1963
  ----> http://www.adafruit.com/products/2468
  ----> http://www.adafruit.com/products/2542

  These cellular modules use TTL Serial to communicate, 2 pins are
  required to interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution

  This program measures type T Thermocouples using a CR1000 and
  sends a 1V to an SDM-A04 module if no alarm is triggered
  and 4V if it is triggered.
  5/26/16 reconfigurd pin numbers
  
  
  AO4_1_1 connected to digital pin 5 anna
  AO4_1_2 connected to digital pin 11 chip original pin 6 bad on this board
  AO4_1_3 connected to digital pin 7 old crappy blue
  AO4_1_4 connected to digital pin 8 not 13 B/C LED wired there OldGrey zang
  AO4_2_1 connected to digital pin 9 shawn  
  AO4_2_2 connected to digital pin 10chembug
  the combination of FONA-Test and Digital Read made by
  Maritza Romero

  this program sends 1x alarm txt to the person responsible and a back up txt to me
 ****************************************************/



//-----------------(Import Libraries used)------------------------------------------
#include "Adafruit_FONA.h"
#include <SoftwareSerial.h>

#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4
//-----------------(Declare Constants & Pin #'s)-----------------------------------------------
const int threshold = 1;// alarm state to send txt
int loopcounter;
unsigned long Time;

int AnnaAlarmPin = 5; //AnnaAlarmPin is the AO4 output Alarm for AO4-1#1 
int ChipAlarmPin = 11;//AO4-1#2 (Slave CR1000)
int OldBlueAlarmPin = 7; //oldcrappy me  call me for now AO4-1#3
int OldGreyAlarmPin = 8; // AO4-1#4

int ShawnAlarmPin = 9;// Ao4-2#1
int CharlieAlarmPin = 10;//AO4-2#2 (master CR1000)
//--------------------(Declare Variables)-----------------------------------------------------------
// conunt[] is an array of 6 elements that starts with inex 0
int count[6] = {0, 0, 0, 0, 0, 0}; //used to only send 1 txt when the alarm pin goes to 4V)removed extra comma at end of last array 0
int i; // reply counter 0 to 5
// this program disabled call back feature and changed message
//7_18_2018   Will send one text only
char callerIDbuffer[32];

char replybufferAnna[] = "Block Lab Ultralow overheating";
char replybufferChip[] = "Hunter 01 Lab Ultralow overheating";
char replybufferBlue[] = " OldCrappy Ultralow overheating";
char replybufferGrey[] = "Old Zang Ultralow overheating";

char replybufferShawn[] = "Shawn Lab Ultralow overheating";
char replybufferCharlie[] = "Chembug Lab Ultralow overheating";



//commenting phone number out so it cant send text while testing  replace # below with phone numbers with area code
//char phone_numberMaritza[] = "+##########"; // put reset system number here
char phone_numberAnna[] = "+1##########"; // put sending to number here
char phone_numberShawn[] = "+1##########"; // put sending to number here
char phone_numberChip[] = "+1##########"; // put sending to number here
char phone_numberCharlie[] = "+##########"; // put sending to number here

//--------------------(Declare Objects)-------------------------------------------------------------
// We default to using software serial.
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

// Use this for FONA 800 and 808s
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

void setup() {
  while (!Serial);

  Serial.begin(115200); // for FONA may not need separete serial.begin for alarm
  Serial.println(F("FONA basic test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) { //open if 1
    Serial.println(F("Couldn't find FONA"));
    while (1);
  } //close if 1
  Serial.println(F("FONA is OK"));

  //make AlarmPins and input
  pinMode(AnnaAlarmPin, INPUT);
  pinMode(ChipAlarmPin, INPUT);
  pinMode(OldBlueAlarmPin, INPUT);
  pinMode(OldGreyAlarmPin, INPUT);
  
  pinMode(ShawnAlarmPin, INPUT);
  pinMode(CharlieAlarmPin, INPUT);
  
  delay(3000);// 3 seconds delay is in milisec there are 1000 ms=1sec

}
char fonaInBuffer[64];
void loop() {
  //read the "Person" AlarmPin and save into variable state

  int AnnaAlarmState = digitalRead(AnnaAlarmPin);
  int ChipAlarmState = digitalRead(ChipAlarmPin);
  int OldBlueAlarmState = digitalRead(OldBlueAlarmPin);
  int OldGreyAlarmState = digitalRead(OldGreyAlarmPin);
  
  int ShawnAlarmState = digitalRead(ShawnAlarmPin);
  int CharlieAlarmState = digitalRead(CharlieAlarmPin);
  

  Serial.print(F("AnnaAlarmState=")); Serial.println(AnnaAlarmState);
  Serial.print(F("count[0]=")); Serial.println(count[0]);
  delay (100);
  Serial.print(F("ChipAlarmState=")); Serial.println(ChipAlarmState);
  Serial.print(F("count[1]=")); Serial.println(count[1]);
  delay (100);
  Serial.print(F("OldBlueAlarmState=")); Serial.println(OldBlueAlarmState);
  Serial.print(F("count[2]=")); Serial.println(count[2]);
  delay (100);
  Serial.print(F("OldGreyAlarmState=")); Serial.println(OldGreyAlarmState);
  Serial.print(F("count[3]=")); Serial.println(count[3]);
  delay (100);
  
  Serial.print(F("ShawnAlarmState=")); Serial.println(ShawnAlarmState);
  Serial.print(F("count[4]=")); Serial.println(count[4]);
  delay (100);
  Serial.print(F("CharlieAlarmState=")); Serial.println(CharlieAlarmState);
  Serial.print(F("count[5]=")); Serial.println(count[5]);
  delay (100);

  Serial.print(F("                   "));
  delay (500);

  // end AlarmPin
// 7/1/2018 ----------------------commented out since for this version not allowing call back.
//  //------------------------5/16/16 added from FONA-SMS Response reads the incoming-----------------------------
//
//  char* bufPtr = fonaInBuffer;    //handy buffer pointer
//
//  if (fona.available())      //any data available from the FONA?
//  { //if loop 2
//    int slot = 0;            //this will be the slot number of the SMS
//    int charCount = 0;
//    //Read the notification into fonaInBuffer
//    do  {// do loop 1
//      *bufPtr = fona.read();
//      Serial.write(*bufPtr);
//      delay(1);
//    }// close do loop 1
//    while ((*bufPtr++ != '\n') && (fona.available()) && (++charCount < (sizeof(fonaInBuffer) - 1)));
//
//    //Add a terminal NULL to the notification string
//    *bufPtr = 0;
//
//    //Scan the notification string for an SMS received notification.
//    //  If it's an SMS message, we'll get the slot number in 'slot'
//    if (1 == sscanf(fonaInBuffer, "+CMTI: \"SM\",%d", &slot)) {// if loop 3
//      Serial.print("slot: "); Serial.println(slot);
//
//      char callerIDbuffer[32];  //we'll store the SMS sender number in here
//
//      // Retrieve SMS sender address/phone number.
//      if (! fona.getSMSSender(slot, callerIDbuffer, 31)) {// if loop 4
//        Serial.println("Didn't find SMS message in slot!");
//      }// close if loop 4
//      Serial.print(F("callerIDBuffer: ")); Serial.println(callerIDbuffer);// Caller IDBuffer is the phone number of texter
//       //---------------------------compare callerIDbuffer to my number and reset system --------
//  // strcmp works as string comparison for now all cases have my phone when its finished it will have individual lab phone#
//  // this section compares the txt acknowleging the alarm and turns alarm function off i will need to send my text to the system to 
//  //re instate alarm.



//7/18/2018 commented out for testing  
////---------------------------commented out 2/23/17 for testing timing----------------------------------------------------------------------
////  if (strcmp(callerIDbuffer, phone_numberMaritza) == 0) { // open if ; strcmp==0 both strings are the same
////   
////    for (i = 0; i < 6; i = i + 1) { // open for
////     
////      count[i]=0; //reset all the freezers so they can recieve alarm conditions
////    } //close for
////  }
////-----------------------------------------------------------------------------------------------------------------------------------------
//  if (strcmp(callerIDbuffer, phone_numberAnna) == 0) { // open if ; strcmp==0 both strings are the same
//      count[0]=2;  //reset Annas Alarm to tex no calls
//   }
//    if (strcmp(callerIDbuffer, phone_numberCharlie) == 0) { // open if ; strcmp==0 both strings are the same
//      count[5]=2;  //reset charlie Alarm to tex no calls
//   }
//    if (strcmp(callerIDbuffer, phone_numberChip) == 0) { // open if ; strcmp==0 both strings are the same 2/23/17 needs to be numberChip but for testing using Maritza
//      count[1]=2; 
//      count[2]=2;//reset chips Alarm to tex no calls
//   }
//    if (strcmp(callerIDbuffer, phone_numberShawn) == 0) { // open if ; strcmp==0 both strings are the same
//      count[3]=2; 
//      count[4]=2;//reset chips Alarm to tex no calls
//   }
//
//      // delete the original msg after it is processed
//      //   otherwise, we will fill up all the slots
//      //   and then we won't be able to receive SMS anymore
//      if (fona.deleteSMS(slot)) {// if loop 5
//        Serial.println(F("OK!"));
//      } // close if loop 5
//      else {// else loop 1
//        Serial.println(F("Couldn't delete"));
//      }// close else loop 1
//    }// close if loop 3
//  }// close if loop 2
 
  //--------------------------------------- send an SMS! based on alarm condition---------------------------------------
  // delay(1000)= 1 sec because the scan rate of the campbell program is 1 min this program needs to have a delay(60,000) between calls here to account for the other scan
  if (AnnaAlarmState == threshold && count[0] == 0) { // test for when it is equal to 1 only send txt if count[]=0
  //  flushSerial();// added 7/19/2018
    Serial.print(F("threshold="));  Serial.println(threshold);
    Serial.print(F("count[0]="));  Serial.println(count[0]);
    Serial.println(F("sending sms to:"));
    Serial.println(phone_numberAnna);
    Serial.println(replybufferAnna);
    if (!fona.sendSMS(phone_numberAnna, replybufferAnna)) {// after trouble shootint put phone_numberShawn
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("Sent!"));
    }// close else
    Serial.println(F("              "));
  //  if (!fona.sendSMS(phone_numberMaritza, replybufferAnna)) {// back up email to me for reset follow up
  //    Serial.println(F("Failed"));
  //  } else {
 //     Serial.println(F("Sent!"));
 //   }// close else
   // delay ( 500); // dekay(1000) 1 sec delay
    count[0] = 1; //only send one text resets so loop count returns this to 0 at 15 min only sends txt if count[0]==0
    
    Serial.print(F("count[0]=")); Serial.println(count[0]);
    delay (60000); // added 7/19/2018  1 min pause because of scan rate in campbell sys also allowes time to send txt 
  }// close if

 //--------------------------------------------------------------------------------------------------------------------------------- 
  if (ChipAlarmState == threshold && count[1] == 0) {
   // flushSerial();
    Serial.print(F("threshold="));  Serial.println(threshold);
    Serial.print(F("count[1]="));  Serial.println(count[1]);
    Serial.println(F("sending sms to:"));
    Serial.println(phone_numberChip);
    Serial.println(replybufferChip);

    if (!fona.sendSMS(phone_numberChip, replybufferChip)) {// after trouble shootint put phone_numberShawn
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("Sent!"));
    }
    Serial.println(F("              "));
//    if (!fona.sendSMS(phone_numberMaritza, replybufferChip)) {// back up email to me for reset follow up
//      Serial.println(F("Failed"));
//    } else {
//      Serial.println(F("Sent!"));
//    }// close else
   // delay(500);
    count[1] = 1;

    Serial.print(F("count[1]=")); Serial.println(count[1]);
    delay(60000);
  }
  //--------------------------------------------------------------------------------------------------------

  if (OldBlueAlarmState == threshold && count[2] == 0) {
   // flushSerial();
    Serial.print(F("threshold="));  Serial.println(threshold);
    Serial.print(F("count[2]="));  Serial.println(count[2]);
    Serial.println(F("sending sms to:"));
    Serial.println(phone_numberChip); //2/23/17 change back to chip after test
    Serial.println(replybufferBlue);
    Serial.print(F("count[2]=")); Serial.println(count[2]);
    if (!fona.sendSMS(phone_numberChip, replybufferBlue)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("Sent!"));
    }
    Serial.println(F("              "));
//    if (!fona.sendSMS(phone_numberMaritza, replybufferBlue)) {// back up email to me for reset follow up
//      Serial.println(F("Failed"));
//    } else {
//      Serial.println(F("Sent!"));
//    }// close else
    //delay(500);
    count[2] = 1;

    Serial.print(F("count[2]=")); Serial.println(count[2]);
    delay(60000);
  }

  ////--------------------------------------------------------------------------------------------------------


  if (OldGreyAlarmState == threshold && count[3] == 0) {
  //  flushSerial();
    Serial.print(F("threshold="));  Serial.println(threshold);
    Serial.print(F("count[3]="));  Serial.println(count[3]);
    Serial.println(F("sending sms to:"));
    Serial.println(phone_numberShawn);
    Serial.println(replybufferGrey);
    if (!fona.sendSMS(phone_numberShawn, replybufferGrey)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("Sent!"));
    }
    Serial.println(F("              "));
//    if (!fona.sendSMS(phone_numberMaritza, replybufferGrey)) {// back up email to me for reset follow up
//      Serial.println(F("Failed"));
//    } else {
//      Serial.println(F("Sent!"));
//    }// close else
   // delay(500);
    count[3] = 1;

    Serial.print(F("count[3]=")); Serial.println(count[3]);
    delay(60000);
    
  }//closed if oldgrey loop

//-------------------------------------------------------------------------------------------------------------------------
  
  if (ShawnAlarmState == threshold && count[4] == 0) { // test for when it is equal to 1
  //  flushSerial();
    Serial.print(F("threshold="));  Serial.println(threshold);
    Serial.print(F("count[4]="));  Serial.println(count[4]);
    Serial.println(F("sending sms to:"));
    Serial.println(phone_numberShawn);
    Serial.println(replybufferShawn);
    if (!fona.sendSMS(phone_numberShawn, replybufferShawn)) {// after trouble shootint put phone_numberShawn
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("Sent!"));
    }// close else

    Serial.println(F("              "));
//    if (!fona.sendSMS(phone_numberMaritza, replybufferShawn)) {// back up email to me for reset follow up
//      Serial.println(F("Failed"));
//    } else {
//      Serial.println(F("Sent!"));
//    }// close else
    //delay(500);
    count[4] = 1;

    Serial.print(F("count[4]=")); Serial.println(count[4]);
    delay(60000);

  }   // end if statement

   //////--------------------------------------------------------------------------------------------------------
  if (CharlieAlarmState == threshold && count[5] == 0) {
   // flushSerial();
    Serial.print(F("threshold="));  Serial.println(threshold);
    Serial.print(F("count[5]="));  Serial.println(count[5]);
    Serial.println(F("sending sms to:"));
    Serial.println(phone_numberCharlie);
    Serial.println(replybufferCharlie);

    if (!fona.sendSMS(phone_numberCharlie, replybufferCharlie)) { //replace to charlie after testing
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("Sent!"));
    }
    Serial.println(F("              "));
//    if (!fona.sendSMS(phone_numberMaritza, replybufferCharlie)) {// back up email to me for reset follow up
//      Serial.println(F("Failed"));
//    } else {
//      Serial.println(F("Sent!"));
//    }// close else
   //delay(500);
    count[5] = 1;

    Serial.print(F("count[5]=")); Serial.println(count[5]);
    delay(60000);
  }

  ////--------------------------------------------------------------------------------------------------------


  
   Serial.print(F("FROM: ")); Serial.println(callerIDbuffer);
  loopcounter++;
  Serial.print(F("loopcounter")); Serial.println(loopcounter);
  //  Serial.print(F("Time:"));
  //   Time=millis();
  //prints time since program started
  //   Serial.println(Time);

  if (loopcounter == 300) {// oritinally used 500 about every 5 min resets system to send another txt now 3000 for every 30min
    loopcounter = 0;//reset loop counter 3000 close to 30min see pg69 4/12/16 labbook
//---------------commented out to turn off call back capability --------------------------------------------------   
//    for (i = 0; i < 5; i = i + 1) { // open for
//      if (count[i]!=2) {// if not = to 2 caller shut down alarm can reset
//      count[i]=0;// reset count[i] so another  txt can be sent
//      }
//      Serial.print(F("count[i]= ")); Serial.println(count[i]);
//    } //close for
//  }//closed the loopcounter if
//------------------------------------------------------------------------------------------------------------------
for (i = 0; i < 5; i = i + 1) { // open for

     count[i]=0;// reset count[i] so another  txt can be sent

     Serial.print(F("count[i]= ")); Serial.println(count[i]);

  }//closed the loopcounter for
  }//close if loopcounter
}// closes void loop
