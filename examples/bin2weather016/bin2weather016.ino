//  bin2weather.ino
//
//  This converts the 24 bit meteotime telegram into a human readable forecast
//  In this example three meteotime weather forecasts are used to demonstrate the principles
//  You can play around with them to get different effects.
//  Bas Kasteel, 2014
//  The program uses a modified version of the library from Thijs Elenbaas, 2012
//  library expanded for weather information
//  You can contact me via www.arduino-projects4u.com
//  Date 25:7:2014
//  Chanelog:
//  0.1 innitial release

//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.

//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.

//  If you want a copy of the GNU Lesser General Public
//  License you can write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include <avr/pgmspace.h>
#include <Flash.h>    // http://arduiniana.org/libraries/flash/
#include <DCF77.h>    // https://github.com/thijse/Arduino-Libraries/downloads
#include <Time.h>     // http://www.arduino.cc/playground/Code/Time
#define DCF_PIN 2	                      // Connection pin to DCF77 device
#define DCF_INTERRUPT 0		              // Interrupt number associated with pin
time_t time;
DCF77 DCF = DCF77(DCF_PIN,DCF_INTERRUPT);
String meteodata;
String timedata;
int val;
char buffer[32];
int fourdayforecast=0;
int twodayforecast=0;
int temp=0;
int y = 0;
#define SerialPrint(x) SerialPrint_P(PSTR(x))
#define SerialPrintLn(x) SerialPrint_Q(PSTR(x))
int significantweather[60]={0};
String weathermemory[60];
void setup(){
Serial.begin(115200); 
//                   0110  Day critical weather
//                       0001  Night critical weather
//                           0001 Extremeweather
//                               011 Rainprobability
//                                  0  Anomaly
//                                   101101 Temperature
meteodata = String ("111001101100011010110110");//Snow, Heavy rain with thunderstorms, 90%, Heavy weather night, No, 23⁰c
DCF.DCFminute=8;DCF.DCFhour=2;// amsterdam day 
show();digitalClockDisplay();Serial.println();
meteodata = String ("111001100000011011000110");
DCF.DCFminute=8;DCF.DCFhour=5;// amsterdam night
show();digitalClockDisplay();Serial.println();
meteodata = String ("011000010000011011000110");
DCF.DCFminute=8;DCF.DCFhour=23;// amsterdam day 4 wind + 2 day forecast Lissabon
show();digitalClockDisplay();
delay(5000);
}

void loop(){
}

void unbinary(int a, int b){
        val=0;
        for (int i = 0; i < 16; i++) {
        if (! meteodata.substring(a,b) [i]) {
        break;
        }
        val <<= 1;
        val |= (meteodata.substring(a,b) [i] == '1') ? 1 : 0;
        }  
}
void weatherunbinary(int a, int b){
        val=0;
        for (int i = 0; i < 16; i++) {
        if (! weathermemory[fourdayforecast].substring(a,b) [i]) {
        break;
        }
        val <<= 1;
        val |= (weathermemory[fourdayforecast].substring(a,b) [i] == '1') ? 1 : 0;
        }  
}
static int Reverse(int x,int z)
{
y=0;
for (int i = 0; i < z; ++i)
{
y <<= 1;
y |= (x & 1);
x >>= 1;
}
return y;
}

void SerialPrint_P(PGM_P str) {
  for (uint8_t c; (c = pgm_read_byte(str)); str++) Serial.write(c);
}
void SerialPrint_Q(PGM_P str) {
  for (uint8_t c; (c = pgm_read_byte(str)); str++) Serial.write(c);
  Serial.println();
}
void digitalClockDisplay(){
  setTime(0,0,0,25,7,2014);
  SerialPrint("Time: ");
  Serial.print(DCF.DCFhour);
//  SerialPrint(":");
  printDigits(DCF.DCFminute);
  SerialPrint(":00");
  SerialPrint(" Date: ");
  Serial.print(day());
  SerialPrint(".");
  Serial.print(month());
  SerialPrint(".");
  Serial.println(year());
}
void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  SerialPrint(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void show(){

FLASH_STRING_ARRAY (town,
PSTR("F Bordeaux"), PSTR("F la Rochelle"),PSTR("F Paris"),PSTR("F Brest"),PSTR("F Clermont-Ferrand"),PSTR("F Beziers"),PSTR("B Bruxelles"),PSTR("F Dijon"),PSTR("F Marseille"),PSTR("F Lyon"),PSTR("F Grenoble"),PSTR("CH La Chaux de Fonds"),
PSTR("D Frankfurt am Main"),PSTR("D Trier"),PSTR("D Duisburg"),PSTR("GB Swansea"),PSTR("GB Manchester"),PSTR("F le Havre"),PSTR("GB London"),PSTR("D Bremerhaven"),PSTR("DK Herning"),PSTR("DK Arhus"),PSTR("D Hannover"),PSTR("DK Copenhagen"),PSTR("D Rostock"),
PSTR("D Ingolstadt"),PSTR("D Muenchen"),PSTR("I Bolzano"),PSTR("D Nuernberg"),PSTR("D Leipzig"),PSTR("D Erfurt"),PSTR("CH Lausanne"),PSTR("CH Zuerich"),PSTR("CH Adelboden"),PSTR("CH Sion"),PSTR("CH Glarus"),PSTR("CH Davos"),PSTR("D Kassel"),PSTR("CH Locarno"),PSTR("I Sestriere"),
PSTR("I Milano"),PSTR("I Roma"),PSTR("NL Amsterdam"),PSTR("I Genova"),PSTR("I Venezia"),PSTR("D Strasbourg"),PSTR("A Klagenfurt"),PSTR("A Innsbruck"),PSTR("A Salzburg"),PSTR("A/SK Wien/Bratislava"),PSTR("CZ Praha"),PSTR("CZ Decin"),PSTR("D Berlin"),PSTR("S Gothenburg"),
PSTR("S Stockholm"),PSTR("S Kalmar"),PSTR("S Joenkoeping"),PSTR("D Donaueschingen"),PSTR("N Oslo"),PSTR("D Stuttgart"),
PSTR("I Napoli"),PSTR("I Ancona"),PSTR("I Bari"),PSTR("H Budapest"),PSTR("E Madrid"),PSTR("E Bilbao"),PSTR("I Palermo"),PSTR("E Palma de Mallorca"),
PSTR("E Valencia"),PSTR("E Barcelona"),PSTR("AND Andorra"),PSTR("E Sevilla"),PSTR("P Lissabon"),PSTR("I Sassari"),PSTR("E Gijon"),PSTR("IRL Galway"),PSTR("IRL Dublin"),PSTR("GB Glasgow"),PSTR("N Stavanger"),PSTR("N Trondheim"),PSTR("S Sundsvall"),PSTR("PL Gdansk"),
PSTR("PL Warszawa"),PSTR("PL Krakow"),PSTR("S Umea"),PSTR("S Oestersund"),PSTR("CH Samedan"),PSTR("HR Zagreb"),PSTR("CH Zermatt"), PSTR("HR Split"));  


FLASH_STRING_ARRAY (weather,
PSTR("Reserved"),PSTR("Sunny"),PSTR("Partly clouded"),PSTR("Mostly clouded"),PSTR("Overcast"),PSTR("Heat storms"),PSTR("Heavy rain"),PSTR("Snow"),PSTR("Fog"),PSTR("Sleet"),PSTR("Rain showers"),PSTR("light rain"),
PSTR("Snow showers"),PSTR("Frontal storms"),PSTR("Stratus cloud"),PSTR("Sleet storms")); 

FLASH_STRING_ARRAY (heavyweather,
PSTR("None"),PSTR("Heavy Weather 24 hrs."),PSTR("Heavy weather Day"),PSTR("Heavy weather Night"),PSTR("Storm 24hrs."),PSTR("Storm Day"),PSTR("Storm Night"),
PSTR("Wind gusts Day"),PSTR("Wind gusts Night"),PSTR("Icy rain morning"),PSTR("Icy rain evening"),PSTR("Icy rain night"),PSTR("Fine dust"),PSTR("Ozon"),PSTR("Radiation"),PSTR("High water"));     

FLASH_STRING_ARRAY (probprecip,
PSTR("0 %"),PSTR("15 %"),PSTR("30 %"),PSTR("45 %"),PSTR("60 %"),PSTR("75 %"),PSTR("90 %"),PSTR("100 %"));

FLASH_STRING_ARRAY (winddirection,
PSTR("N"),PSTR("NO"),PSTR("O"),PSTR("SO"),PSTR("S"),PSTR("SW"),PSTR("W"),PSTR("NW"),
PSTR("Changeable"),PSTR("Foen"),PSTR("Biese N/O"),PSTR("Mistral N"),PSTR("Scirocco S"),PSTR("Tramont W"),PSTR("reserved"),PSTR("reserved"));

FLASH_STRING_ARRAY (windstrength,
PSTR("0"),PSTR("0-2"),PSTR("3-4"),PSTR("5-6"),PSTR("7"),PSTR("8"),PSTR("9"),PSTR(">=10"));

FLASH_STRING_ARRAY (anomaly1,
PSTR("Same Weather "),PSTR("Jump 1 "),PSTR("Jump 2 "),PSTR("Jump 3 "));

FLASH_STRING_ARRAY (anomaly2,
PSTR("0-2 hrs"),PSTR("2-4 hrs"),PSTR("5-6 hrs"),PSTR("7-8 hrs"));


        fourdayforecast = ((DCF.DCFhour)%3)*20; if (DCF.DCFminute>0) {fourdayforecast+=(DCF.DCFminute-1)/3;}
        if (DCF.DCFminute>0) {twodayforecast=((((DCF.DCFhour)*60)+(DCF.DCFminute-1))%90)/3;}else{twodayforecast=((DCF.DCFhour)*60);}
        twodayforecast+=60;     
        if (DCF.DCFhour<21) //Between 21:00-23:59 significant weather & temperature is for cities 60-89 wind and wind direction for cities 0-59.
        {
        if ((DCF.DCFhour)%6<3){weathermemory[fourdayforecast]=meteodata;weatherunbinary(8,12);Reverse(val,4);significantweather[fourdayforecast]=y;}// Extreme weather is valid from this hour but also +3 hour  
        Serial.print(fourdayforecast);SerialPrint(" ");town[fourdayforecast].print(Serial);Serial.println(); 
        if (((DCF.DCFhour)%6)>2) {SerialPrint("fourday f/c Night =      ");} else {SerialPrint("fourday f/c Day   =      ");}
        Serial.println((DCF.DCFhour/6)+1);//today is 1 tomorrow 2 etc       
        SerialPrint("Day               =   ");Serial.print(meteodata.substring(0,4));SerialPrint(" "); unbinary(0,4);Reverse (val,4); val=y;Serial.print(val,HEX);SerialPrint(" ");temp=val;
        if (temp==5 || temp==6 || temp==13 || temp==7) {
        if (significantweather[fourdayforecast]==1 || significantweather[fourdayforecast]==2) {if (temp !=6) {Serial.print ("Heavy ");};weather[temp].print(Serial);SerialPrintLn(" with thunderstorms");}
        else {weather[temp].print(Serial);Serial.println();}
        }else{weather[temp].print(Serial);Serial.println();}
        SerialPrint("Night             =   ");Serial.print(meteodata.substring(4,8));SerialPrint(" "); unbinary(4,8); Reverse (val,4);val=y;Serial.print(val,HEX);SerialPrint(" ");temp=val;
        if (temp==5 || temp==6 || temp==13 || temp==7) {
        if (significantweather[fourdayforecast]==1 || significantweather[fourdayforecast]==3) {if (temp !=6) {Serial.print ("Heavy ");};weather[temp].print(Serial);SerialPrintLn(" with thunderstorms");}
        else {weather[temp].print(Serial);Serial.println();}
        }else {if (temp==1) {SerialPrint("Clear");;Serial.println();} else {weather[temp].print(Serial);Serial.println();}
        }
        if ((DCF.DCFhour)%6<3){
        SerialPrint("Rainprobability   =    ");Serial.print(meteodata.substring(12,15));SerialPrint(" "); unbinary(12,15);Reverse (val,3);val=y;
        Serial.print(val,HEX);SerialPrint(" ");probprecip[val].print(Serial); Serial.println();
        SerialPrint("Extremeweather    =   "); unbinary(15,16);
        if (val==0) {// DI=0 and WA =0
        Serial.print(weathermemory[fourdayforecast].substring(8,12));SerialPrint(" ");
        Serial.print(significantweather[fourdayforecast],HEX);SerialPrint(" ");heavyweather[significantweather[fourdayforecast]].print(Serial); Serial.println();
        } else {
        Serial.print(meteodata.substring(8,10));SerialPrint(" ");  
        unbinary(8,10);Serial.print(val,HEX);SerialPrint(" ");anomaly1[val].print(Serial); Serial.println();
        SerialPrint("                  =     ");
        Serial.print(meteodata.substring(10,12));SerialPrint(" ");
        unbinary(10,12);Serial.print(val,HEX);SerialPrint(" ");anomaly2[val].print(Serial); Serial.println(); 
        }          
        }
        if ((DCF.DCFhour)%6>2){        
        SerialPrint("Winddirection     =   ");Serial.print(meteodata.substring(8,12));SerialPrint(" ");unbinary(8,12);Reverse (val,4);val=y;
        Serial.print(val,HEX);SerialPrint(" ");winddirection[val].print(Serial); Serial.println();
        SerialPrint("Windstrength      =    ");Serial.print(meteodata.substring(12,15));SerialPrint(" ");unbinary(12,15);Reverse (val,3);val=y;
        Serial.print(val,HEX);SerialPrint(" ");windstrength[val].print(Serial); SerialPrintLn(" Bft");
        SerialPrint("Extremeweather    =   ");
        Serial.print(weathermemory[fourdayforecast].substring(8,12));SerialPrint(" ");
        Serial.print(significantweather[fourdayforecast],HEX);SerialPrint(" ");heavyweather[significantweather[fourdayforecast]].print(Serial); Serial.println();       
        } 
        SerialPrint("Weather Anomality =      ");Serial.print(meteodata.substring(15,16));SerialPrint(" "); unbinary(15,16);Serial.print(val,HEX);
        if (val==1) {SerialPrintLn(" Yes");} else {SerialPrintLn(" No");}
        }
        else
        //*********************************************************************************************************************************
        {
        Serial.print(fourdayforecast);SerialPrint(" ");town[fourdayforecast].print(Serial);Serial.println(); 
        if (((DCF.DCFhour)%6)>2) {SerialPrint("fourday f/c Night =      ");} else {SerialPrint("fourday f/c Day   =      ");}
        Serial.println((DCF.DCFhour/6)+1);//today is 1 tomorrow 2 etc        
        SerialPrint("Winddirection     =   ");Serial.print(meteodata.substring(8,12));SerialPrint(" ");unbinary(8,12);Reverse (val,4);val=y;
        Serial.print(val,HEX);SerialPrint(" ");winddirection[val].print(Serial); Serial.println();
        SerialPrint("Windstrength      =    ");Serial.print(meteodata.substring(12,15));SerialPrint(" ");unbinary(12,15);Reverse (val,3);val=y;
        Serial.print(val,HEX);SerialPrint(" ");windstrength[val].print(Serial); SerialPrintLn(" Bft");
        Serial.print(twodayforecast);SerialPrint(" ");town[twodayforecast].print(Serial);Serial.println();  
        SerialPrint("twoday f/c day    =      ");
        if (((DCF.DCFhour-21)*60+DCF.DCFminute)>90) {SerialPrintLn("2");}else{SerialPrintLn("1");}//today is 1 tomorrow 2 etc                
        SerialPrint("Day               =   ");Serial.print(meteodata.substring(0,4));SerialPrint(" "); unbinary(0,4);Reverse (val,4); val=y;Serial.print(val,HEX);SerialPrint(" ");
        temp=val; unbinary(8,12); Reverse (val,4);val=y;
        weather[temp].print(Serial);Serial.println();
        SerialPrint("Night             =   ");Serial.print(meteodata.substring(4,8));SerialPrint(" "); unbinary(4,8); Reverse (val,4);val=y;Serial.print(val,HEX);SerialPrint(" ");
        if (val==1) {SerialPrint("Clear");;Serial.println();} else {weather[temp].print(Serial);Serial.println();}
        SerialPrint("Weather Anomality =      ");Serial.print(meteodata.substring(15,16));SerialPrint(" "); unbinary(15,16);Serial.print(val,HEX);
        if (val==1) {SerialPrintLn(" Yes");} else {SerialPrintLn(" No");}      
        }
        SerialPrint("Temperature       = ");Serial.print(meteodata.substring(16,22));SerialPrint(" "); unbinary(16,22);Reverse (val,6);val=y;    
        SerialPrint("0x");Serial.print(val,HEX);SerialPrint(" ");
        if (val==0) {SerialPrint("<-21 ");Serial.print(char(176));SerialPrint("C");}
        if (val==63) { Serial.print (">40 ");Serial.print(char(176));SerialPrint("C");}
        if ((val!=0) & (val!=63)) {Serial.print(val-22,DEC);Serial.print(char(176));SerialPrint("C");} 
        // Night temperature is minimum
        // Daytime temperature is daytime maximum       
        if (((DCF.DCFhour)%6)>2 && (DCF.DCFhour<21)) {SerialPrintLn(" minimum");} else {SerialPrintLn(" maximum");}        
        SerialPrint("Decoder status    =     "); 
        Serial.print(meteodata.substring(22,24));
        if(meteodata.substring(22,24) == "10") {Serial.println (" OK");} else {Serial.println (" NOT OK");} 
}


