# Ultralow_SMS_CR1000AlarmOld
Create SMS alarm system with leftover K type thermocouples,Campbell Scientific CR1000 , SDMAO-4, MD-485 Loggernet network, arduino and FONA shield

Ultralow monitor part:
In this program some leftover Campbell Sci Equipment was used to create an economic ultralow Alarms sytem.  At the time of original coding 2017
arduino did not have sensor that could read accurately below -20 degC and the ultra lows are -80 deg C.  Having some surplus Campbell Scientific CR1000's, MD485's , SDM-AO4
and Type T thermocouples created an ultralow monitoring system.  The  system used one CR1000 each on the East of the Blg and another on the West.  East CR1000 is the master and has 2 type T thermocouples connected.  Each was fed to corresponding ultralow's upper chamber.  The West CR1000, the slave, had 4 Type T thermocouples connected  to corresponding ultralows.   Campbell Scientific Loggernet software was used to create an MD-485 network that traversed the building  with sheilded twisted pair cable. One 
MD-485 with each CR1000 and in the engineer's office  a base MD-485 connected to a Windows desktop.  This allowed remote monitoring in real time of the ultralows. The CR1000's automatically download the  average temperature every 30 min and provide a once a day dowload of CSV file with all the ultralows temperatures for archiving.  It is also used to monitor compressor performance and schedule defrosting schedule.

Alarm and SMS arduino:
The master CR1000 program monitors the temp and sets an alarm high if temp >-68 degC.  This is communicated to the arduino by using the SDMAO-4 which provides a 1V(Low) or 4V(high) to a pin of the arduino.  The arduino has a adfruit FONA shield a Ting SIM card in order to send  texts to the scientist that their ultralow is overheating and needs attention.

In the arduino code used ########## to represent a phone number with area code.



