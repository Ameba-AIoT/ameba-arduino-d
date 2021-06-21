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

#include "GHMI.h"

#if defined(USE_SOFTWARE_SERIAL)
GHMI::GHMI(SoftwareSerial &next, uint32_t baud): ghmi(&next)
{
  ghmi->begin(baud);
  flushSerial();
}
#else
GHMI::GHMI(HardwareSerial &next, uint32_t baud): ghmi(&next)
{
  ghmi->begin(baud);
  flushSerial();
}
#endif

boolean GHMI::ack(void)
{
  /* CODE+END*/
  uint8_t bytes[4] = {0};
  ghmi->setTimeout(20);
  if (sizeof(bytes) != ghmi->readBytes((char *)bytes, sizeof(bytes)))
  {
    return 0;
  } 
  
  if((bytes[1]==0xFF)&&(bytes[2]==0xFF)&&(bytes[3]==0xFF))
  {
    switch (bytes[0]) 
	{
	case 0x00:
	  return false; // 無效指令
	break; 
  
	case 0x01:
	  return true; // 指令成功執行
	break; 
    
	//...... more case you can see in tables
	
	default: 
	  return false;
    }  
  } 
}  

void GHMI::SendSleep(boolean sleeps)
{
  switch (sleeps)
  {
    case 0:
      ghmi->write("sleep=0\xff\xff\xff"); // wake up
      ghmi->write("sleep=0\xff\xff\xff"); // wake up
      Serial.println("Wake up~");
      break;

    case 1:
      ghmi->write("sleep=1\xff\xff\xff"); // sleep
	  ghmi->write("sleep=1\xff\xff\xff"); // sleep
      Serial.println("Sleep~");
      break;
  }
}

void GHMI::writeString(String stringData)
{
  for (int i = 0; i < stringData.length(); i++)
  {
    ghmi->write(stringData[i]); // Push each char 1 by 1 on each loop pass
  }
}

void GHMI::SendPage(String PageName)
{
 SendSleep(0); // wake up
 ghmi->write("page ");
 for (int i = 0; i < PageName.length(); i++)
 {
   ghmi->write(PageName[i]);   // Push each char 1 by 1 on each loop pass
 }
 ghmi->write(0xFF);
 ghmi->write(0xFF);
 ghmi->write(0xFF);
}

void GHMI::SendPic(uint8_t x, uint8_t y, uint8_t id)
{
  char bf[20];
  sprintf(bf, "pic % d, % d, % d\xff\xff\xff", x, y, id);
  ghmi->write(bf);
}

void GHMI::SendTxt(String id, String txt)
{
  String string = "";
//  string += id + ".txt=" + char(34) + txt + char(34) + "\xff\xff\xff";
  string += id + ".txt=\""+ txt +"\"\xff\xff\xff";
  writeString(string);
}

void GHMI::SendVal(String id, int val)
{
  String string = "";
  string += id + ".val=" + String(val) + "\xff\xff\xff";
  writeString(string);
}

void GHMI::SendCmd(String CMD)
{
 for (int i = 0; i < CMD.length(); i++)
 {
   ghmi->write(CMD[i]);   // Push each char 1 by 1 on each loop pass
 }
 ghmi->write(0xFF);
 ghmi->write(0xFF);
 ghmi->write(0xFF);
}

String GHMI::Version(void)
{
  return "20210328 HMI W/ MCU Controller XYG V2";
}

void GHMI::SendCommand(const char* cmd)
{
  while (ghmi->available())
  {
	ghmi->read();
  }
  
  ghmi->write(cmd);
  ghmi->write(0xFF);
  ghmi->write(0xFF);
  ghmi->write(0xFF);
}

boolean GHMI::init(const char* pageId)
{
  SendSleep(0);
  
  // String page = "page " + String(pageId); // Page
  // SendCommand("");
  // ack();
  // sendCommand(page.c_str());
  // delay(100);
  // return ack();
}

void GHMI::flushSerial(){
  Serial.flush();
  ghmi->flush();
}