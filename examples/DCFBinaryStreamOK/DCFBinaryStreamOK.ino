/*
 * DCFBinaryStream.ino
 * example code illustrating time synced from a DCF77 receiver
 * Thijs Elenbaas, 2012
 * This example code is in the public domain.
 
  This example shows the binary stream generated by the 
  pulse train coming from the DCF decoder and the resulting
  time
  In order for this example to give output from the DCF library,
  make sure that logging is turned on in the DCF library. You can 
  do this by adding the   #define VERBOSE_DEBUG 1   in Utils.cpp. 
 */


#include <DCF77.h>       //https://github.com/thijse/Arduino-Libraries/downloads
#include <TimeLib.h>        //http://www.arduino.cc/playground/Code/Time

#define DCF_PIN 2	         // Connection pin to DCF 77 device
#define DCF_INTERRUPT 0		 // Interrupt number associated with pin

time_t time;
DCF77 DCF = DCF77(DCF_PIN,DCF_INTERRUPT);

void setup() {
 Serial.begin(115200); 
 Serial.println("1   - binary 1 corresponding to long pulse");
 Serial.println("0   - binary 0 corresponding to short pulse");
 Serial.println("BF  - Buffer is full at end of time-sequence. This is good");
 Serial.println("EoB - Buffer is full before at end of time-sequence");
 Serial.println("EoM - Buffer is not yet full at end of time-sequence");
 DCF.Start();
}

void loop() {
	delay(1000);
	time_t DCFtime = DCF.getTime();
	if (DCFtime!=0) {
		digitalClockDisplay(DCFtime);
	}	
}

void digitalClockDisplay(time_t _time){
  tmElements_t tm;   
  breakTime(_time, tm);
  Serial.print("Time: ");
  Serial.print(tm.Hour);
  Serial.print(":");
  printDigits(tm.Minute);
  Serial.print(":");
  printDigits(tm.Second);
  Serial.print(" Date: ");
  Serial.print(tm.Day);
  Serial.print(".");
  Serial.print(tm.Month);
  Serial.print(".");
  Serial.println(tm.Year+1970);
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
