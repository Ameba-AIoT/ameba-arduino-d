/*
  Louis Guan
  bat869cube672@gmail.com

  License
 **********************************************************************************
  This program is free software; you can redistribute it
  and/or modify it under the terms of the GNU General
  Public License as published by the Free Software
  Foundation; either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will
  be useful, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A
  PARTICULAR PURPOSE. See the GNU General Public
  License for more details.

  You should have received a copy of the GNU General
  Public License along with this program.
  If not, see <http://www.gnu.org/licenses/>.

  Licence can be viewed at
  http://www.gnu.org/licenses/gpl-3.0.txt
  Please maintain this license information along with authorship
  and copyright notices in any redistribution of this code
 **********************************************************************************
*/

// ensure this library description is only included once
#ifndef __GHMI_h__
#define __GHMI_h__

#if (ARDUINO >=100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define USE_SOFTWARE_SERIAL //Comment this line for use  HardwareSerial

#if defined(USE_SOFTWARE_SERIAL)
#include "SoftwareSerial.h"
#endif

class GHMI
{
  public:

    GHMI(){}; //Empty contructor
	
#if defined(USE_SOFTWARE_SERIAL)
    GHMI(SoftwareSerial &next, uint32_t baud); // Constructor
#else
    GHMI(HardwareSerial &next, uint32_t baud); // Constructor
#endif

    boolean ack(void);
    
    void SendSleep(boolean sleeps);
    void SendPage(String id);
    void SendPic(uint8_t x, uint8_t y, uint8_t id);
    void SendTxt(String id, String txt);
    void SendVal(String id, int val);
    void SendFree(String string);
    void writeString(String stringData);
	void SendCmd(String CMD);
    String Version(void);
	
	void SendCommand(const char* cmd);
	
    boolean init(const char* pageId = "0");
  private:

    void flushSerial();
	
#if defined(USE_SOFTWARE_SERIAL)
    SoftwareSerial *ghmi;
#else
    HardwareSerial *ghmi;
#endif

};
#endif
