// C:\Users\Guan\AppData\Local\Arduino15\packages\realtek\hardware\AmebaD\3.0.7\variants\rtl8721d\variant.cpp
// https://makermore.wordpress.com/2016/01/04/arduino-ide-1-5-%E7%AC%AC%E4%B8%89%E6%96%B9%E5%87%BD%E5%BC%8F%E5%BA%AB%E8%A6%8F%E6%A0%BC%E6%9B%B8/
// https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
// https://blog.csdn.net/liurunjiang/article/details/111194762  C之宏定义
// https://www.ncnynl.com/archives/201606/42.html
// https://gcc.gnu.org/onlinedocs/cpp/Index-of-Directives.html#Index-of-Directives  Index of Directives
// https://techclass.rohm.com.cn/knowledge/deviceplus/how-tos/arduino/arduino-preprocessor-directives-tutorial  Arduino预处理器指令教程
// https://www.arduino.cc/en/Reference/APIStyleGuide  Arduino Style Guide for Writing Libraries
// https://github.com/bblanchon/ArduinoJson/issues/1487  ArudinoJson.h for 8722 fix (mocro problem)
// https://www.youtube.com/watch?v=l8TYLFbo9ws&t=60s  How to upgrade from ArduinoJson 5 to 6
//
// https://www.facebook.com/groups/accomdemy/permalink/1641936199329512 // 辦辦學線上影片

#include "SoftwareSerial.h"
#define RXD2 11
#define TXD2 12
SoftwareSerial Serial4(RXD2, TXD2); // RX, TX only can use (0,1),(11,12) not (7,8) or others
#include "src/GHMI.h"
GHMI GHMI01(Serial4, 9600); //create a GHMI object named GHMI01 using the HMI serial port @ 9600bps
#define HMIdataCount 100
//
//
#define LedStatePin 13
//
//
#include "WiFi.h"
char *ssid = "WIFISSID"; // your wifi ssid Tenda_387CD0
char *password = "WIFIPSWD"; // your wifi password
// char* = char[] in array
WiFiClient AmebadClient;
//
//
#include "PubSubClient.h"
char* mqttServer = "MQTTServerIp"; // mqtt server address
int mqttPort = 1883; // mqtt port
char* mqttUser = "MQTTSSID"; // your mqtt user
char* mqttPassword = "MQTTPSWD"; // your mqtt password
//
String clientId = "ESP32Client-"; // Create a random client ID
PubSubClient client(AmebadClient);
//
#define MQTT_SUB_PZEM004T "esp/pzem004t"
#define MQTT_PUB_HandShake "amebad/init"
#define MQTT_SUB_ACK "amebad/ack"
#define MQTT_SUB_Relay "amebad/output"
String MQTT_SUB_Any1 = "amebad/AAA";
String MQTT_SUB_Any2 = "amebad/BBB";
String MQTT_PUB_Any = "amebad/Console";
//
#define MQTT_SUB_Side "amebad/CCC"
//
//
#include <ArduinoJson.h> // ArduinoJson library v6.17.3 (must)
//
//
int MqttSubAction = 0;
int ConsoleMode = 0;




void callback(char* topic, byte* message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");

  String json = "";
  for (int i = 0; i < length; i++)
  {
    // Serial.print((char)message[i]);
    json += (char)message[i];
  }
  Serial.println(json);


  switch (MqttSubAction)
  {
    case 0:
      // do nothing
      break;

    case 1:
      if ((json.length() > 10) && (json != "OK")) // to prevent some bug(null to string.replace) and get stuck~
      {
        StaticJsonDocument<500> doc; // in Ver6

        DeserializationError error = deserializeJson(doc, json); // Deserialize the JSON document
        if (error) // Test if parsing succeeds.
        {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.c_str()); // string 轉換為 const char*
          GHMI01.SendCmd("Monitor1.t16.bco=63488");
          return;
        }

        if (String(topic) == MQTT_SUB_PZEM004T)
        {
          String dn = doc["device"];
          String st = doc["sensortype"];
          String v = doc["voltage"];
          String c = doc["current"];
          String p = doc["power"];
          String e = doc["energy"];
          String f = doc["frequency"];
          String pf = doc["powerfactor"];
          //    long time = doc["time"];
          //    double latitude = doc["data"][0];
          //    double longitude = doc["data"][1];
          //
          if (dn == "ESP32") // You can change the name for deference device
          {
            if (st == "PZEM004T")
            {
              GHMI01.SendCmd("t16.bco=2016");
              GHMI01.SendTxt("t8", v); // voltage
              GHMI01.SendTxt("t9", c); // current
              GHMI01.SendTxt("t10", p); // power
              GHMI01.SendTxt("t11", e); // energy
              GHMI01.SendTxt("t12", f); // frequency
              GHMI01.SendTxt("t13", pf); // power factor
              GHMI01.SendTxt("t14", dn); // Device Name
              GHMI01.SendTxt("t15", st); // Sensor Type
            }
            else
            {
              // do nothing
            }
          }
          else
          {
            // do nothing
          }
        }
        // else if()
        // {
        //
        // }
        else
        {
          // do nothing
        }
      }
      else if ((json.length() > 0) && (json == "OK")) // MQTT success and ACK back~
      {
        Serial.print("MQTT Back ACK -> ");
        Serial.println(json);
      }
      else if ((json.length() > 0) && (json == "on")) // MQTT back control
      {
        Serial.print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        Serial.println(json);
      }
      else if ((json.length() > 0) && (json == "off")) // MQTT back control
      {
        Serial.print("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
        Serial.println(json);
      }
      else
      {
        // do nothing
      }
      break;

    case 2:
      if (String(topic) == MQTT_SUB_Any1)
      {
        /* work hard to parse json by myself */
        //      json.replace("{", ""); // remove the json resudue~
        //      json.replace("}", "");
        json.replace("\"", "");
        //
        //      int str_len = json.length() + 1; // Length (with one extra character for the null terminator)
        //      char char_array[str_len]; // Prepare the character array (the buffer)
        //      json.toCharArray(char_array, str_len); // Copy it over
        //      //
        //      char *JTmp[30]; // the tmp char array to recive the json parsed char array(string array)
        //      int i = 0;
        //      //
        //      const char parses[] = ","; // parse the string by ","
        //      char *token;
        //      token = strtok(char_array, parses);  /* get the first token */
        //      while ( token != NULL )  /* walk through other tokens */
        //      {
        //        JTmp[i] = token;
        //        // Serial.println(JTmp[i]);
        //        i++;
        //
        //        // Serial.println(token);
        //        token = strtok(NULL, parses);
        //      }
        /* work hard to parse json by myself */
        GHMI01.SendTxt("slt0", json); // voltage
      }
      else
      {

      }
      break;

    case 3:
      if (String(topic) == MQTT_SUB_Any2)
      {
        /* work hard to parse json by myself */
        //      json.replace("{", ""); // remove the json resudue~
        //      json.replace("}", "");
        json.replace("\"", "");
        //
        //      int str_len = json.length() + 1; // Length (with one extra character for the null terminator)
        //      char char_array[str_len]; // Prepare the character array (the buffer)
        //      json.toCharArray(char_array, str_len); // Copy it over
        //      //
        //      char *JTmp[30]; // the tmp char array to recive the json parsed char array(string array)
        //      int i = 0;
        //      //
        //      const char parses[] = ","; // parse the string by ","
        //      char *token;
        //      token = strtok(char_array, parses);  /* get the first token */
        //      while ( token != NULL )  /* walk through other tokens */
        //      {
        //        JTmp[i] = token;
        //        // Serial.println(JTmp[i]);
        //        i++;
        //
        //        // Serial.println(token);
        //        token = strtok(NULL, parses);
        //      }
        /* work hard to parse json by myself */
        GHMI01.SendTxt("slt1", json); // voltage
      }
      else
      {

      }
      break;
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(LedStatePin, OUTPUT);
  //
  WIFIreconnect();
  //
  // 以下順序很重要~錯了少了~都會沒作用歐~
  //  client.setBufferSize(512); // option or with need
  client.setServer(mqttServer, mqttPort); // need
  client.setCallback(callback); // need
  MQTTreconnect(); // need
  //
  client.subscribe(MQTT_SUB_PZEM004T); // option or need with topic
  client.subscribe(MQTT_SUB_Relay); // option or need with topic
  client.subscribe(MQTT_SUB_Any1.c_str()); // option or need with topic
  client.subscribe(MQTT_SUB_Any2.c_str()); // option or need with topic
  client.subscribe(MQTT_SUB_Side); // option or need with topic
  //
  GHMI01.SendSleep(0); // HMI wake up
  GHMI01.SendCmd("rest");  // HMI rest & go to the first page
  delay(2000);
  GHMI01.SendCmd("wav0.en=1");  // HMI rest & go to the first page
  GHMI01.SendCmd("tsw t2,1");  // HMI rest & go to the first page
  GHMI01.Version(); // HMI Lib Ver.
}

void loop()
{
  if (!client.connected())
  {
    MQTTreconnect();
  }
  client.loop();
  //
  //
  if (Serial4.available() > 0)
  {
    int i;
    byte HMIdata[HMIdataCount];
    Serial.print("0x ");

    while (Serial4.available() > 0)
    {
      delay(3); // must
      i++; // 接收一個字節加1
      HMIdata[i] = Serial4.read();
      //usart HMI屏發送最後三個數據將發送FF FF FF作為結束標誌
      //最後三個數同時為255（0xFF）時跳出while循環，跳出循環後i會重新計數
      if (HMIdata[i] == 255 && HMIdata[i - 1] == 255 && HMIdata[i - 2] == 255)
      {
        break;
      }
      //      Serial.print(i);
      //      Serial.print("= 0x");
      Serial.print(HMIdata[i], HEX);
      Serial.print(" ");
    }
    Serial.println("FF");

    // 串口HMI設備返回數據格式
    switch (HMIdata[1])
    {
      // 表格一：串口指令執行成功或失敗的通知格式
      case 0x00:
        Serial.print("無效指令 ");
        break;

      case 0x01:
        Serial.print("指令成功執行 ");
        break;

      case 0x02:
        Serial.print("控制項ID無效 ");
        break;

      case 0x03:
        Serial.print("頁面ID無效 ");
        break;

      case 0x04:
        Serial.print("圖片ID無效 ");
        break;

      case 0x05:
        Serial.print("字形檔ID無效 ");
        break;

      case 0x11:
        Serial.print("串列傳輸速率設置無效 ");
        break;

      case 0x12:
        Serial.print("曲線控制項ID號或通道號無效 ");
        break;

      case 0x1A:
        Serial.print("變數名稱無效 ");
        break;

      case 0x1B:
        Serial.print("變數運算無效 ");
        break;

      case 0x1C:
        Serial.print("賦值操作失敗 ");
        break;

      case 0x1D:
        Serial.print("EEPROM操作失敗 ");
        break;

      case 0x1E:
        Serial.print("參數數量無效 ");
        break;

      case 0x1F:
        Serial.print("IO操作失敗 ");
        break;

      case 0x20:
        Serial.print("轉義字元使用錯誤 ");
        break;

      case 0x23:
        Serial.print("變數名稱太長 ");
        break;

      case 0x24:
        Serial.print("串口緩衝區溢位 ");
        break;

      // 表格二：其他資料返回格式
      case 0x65:
        Serial.print("觸摸熱區事件返回 ");
        switch (HMIdata[2])
        {
          case 0x01:
            Serial.print("頁面一 ");
            break;

          case 0x02:
            Serial.print("頁面二 ");
            break;

          case 0x03:
            Serial.print("頁面三 ");
            break;

          case 0x04:
            Serial.print("頁面四 ");
            break;

          case 0x05:
            Serial.print("頁面五 ");
            break;

          case 0x06:
            Serial.print("頁面六 ");
            break;

          case 0x07:
            Serial.print("頁面七 ");
            switch (HMIdata[3])
            {
              case 0x01:
                Serial.print("Console開始訂閱MQTT-1 ");
                ConsoleMode = 1; // start print console and get hmi MQTT subject string
                MqttSubAction = 2;
                break;

              case 0x02:
                Serial.print("Console發布一則主題MQTT ");
                ConsoleMode = 3; // start print console and get hmi MQTT subject string
                break;

              case 0x05:
                Serial.print("Console停止訂閱MQTT ");
                MqttSubAction = 0;
                ConsoleMode = 0;
                break;

              case 0x0A:
                Serial.print("Console開始訂閱MQTT-2 ");
                ConsoleMode = 2; // start print console and get hmi MQTT subject string
                MqttSubAction = 3;
                break;

              case 0x03:


                break;

              case 0x07:

                break;
            }
            break;

          case 0x08:
            Serial.print("頁面八 ");
            switch (HMIdata[3])
            {
              case 0x00:
                Serial.print("返回Menu主停止訂閱MQTT ");
                MqttSubAction = 0;
                break;

              case 0x14:
                Serial.print("開始訂閱MQTT ");
                MqttSubAction = 1;
                break;

              case 0x15:
                Serial.print("停止訂閱MQTT ");
                MqttSubAction = 0;
                break;
            }
            break;

          case 0x09:
            Serial.print("頁面九 ");
            break;

          case 0x0A:
            Serial.print("頁面十 ");
            break;

          case 0x0B:
            Serial.print("頁面十一 ");
            break;

          case 0x0C:
            Serial.print("頁面十二 ");
            Serial.print("Hello Temperature");
            break;
        }
        break;

      case 0x66:
        Serial.println("當前頁面的ID號返回 ");
        break;

      case 0x67:
        Serial.println("觸摸座標資料返回 ");
        break;

      case 0x68:
        Serial.println("睡眠模式觸摸事件 ");
        break;

      case 0x70:
        Serial.print("字串變數資料返回 ");
        switch (ConsoleMode)
        {
          case 0: // 無所事事
            // do nothing
            break;

          case 1: // 訂閱1
            {
              String Tmp = "";
              for (int j = 2; j < (i - 2); j++)
              {
                Tmp += (char)HMIdata[j];
              }
              MQTT_SUB_Any1 = Tmp;
              Serial.println(MQTT_SUB_Any1);
            }
            break;

          case 2: // 訂閱2
            {
              String Tmp = "";
              for (int j = 2; j < (i - 2); j++)
              {
                Tmp += (char)HMIdata[j];
              }
              MQTT_SUB_Any2 = Tmp;
              Serial.println(MQTT_SUB_Any2);
            }
            break;

          case 3: // 發布
            {
              String Tmp = "";
              for (int j = 2; j < (i - 2); j++)
              {
                Tmp += (char)HMIdata[j];
              }
              //
              /* work hard to parse json by myself */
              Tmp.replace("{", ""); // remove the json resudue~
              Tmp.replace("}", "");
              Tmp.replace("\"", "");

              int str_len = Tmp.length() + 1; // Length (with one extra character for the null terminator)
              char char_array[str_len]; // Prepare the character array (the buffer)
              Tmp.toCharArray(char_array, str_len); // Copy it over
              //
              char *hmiTmp[50]; // the tmp char array to recive the json parsed char array(string array)
              int k = 0;
              //
              const char parses[] = " "; // parse the string by ","
              char *token;
              token = strtok(char_array, parses);  /* get the first token */
              while ( token != NULL )  /* walk through other tokens */
              {
                hmiTmp[k] = token;
                // Serial.println(hmiTmp[k]);
                k++;

                // Serial.println(token);
                token = strtok(NULL, parses);
              }
              /* work hard to parse json by myself */

              MQTT_PUB_Any = hmiTmp[0];
              Serial.println(MQTT_PUB_Any);
              client.publish(toCharArray(MQTT_PUB_Any), hmiTmp[1]);
            }
            break;
        }
        break;

      case 0x71:
        Serial.print("數值變數資料返回 ");
        break;

      case 0x86:
        Serial.println("設備自動進入睡眠模式 ");
        break;

      case 0x87:
        Serial.println("設備自動喚醒 ");
        break;

      case 0x88:
        Serial.println("系統啟動成功 ");
        break;

      case 0x89:
        Serial.println("開始SD卡升級 ");
        break;

      case 0xFD:
        Serial.println("透傳資料完成 ");
        break;

      case 0xFE:
        Serial.println("數據透傳就緒 ");
        break;

      case 0xFF:
        Serial.println("HMI進入休眠 ");
        break;

      default:
        Serial.println("未知的指令[1] ");
        break;
    }
    Serial.println("");
  }
}

bool SmartWifi(void)
{
  int c = 0;

  //  Serial.print("connecting to "); Serial.println(ssid);

  while ( c < 30 ) //wait for 30s
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      //      for (uint16_t i = 0; i < Num; i++)
      //      {
      //        strip.setPixelColor(i, 0, 255, 0);
      //      }
      //      strip.show();
      return true;  //return一執行就結束testwifi這個函數, 有連上就return true, 沒連上就return false
    }
    //
    //    for (uint16_t i = 0; i < Num; i++)
    //    {
    //      strip.setPixelColor(i, 0, 0, 0);
    //    }
    //    strip.show();
    //
    Serial.print(".");
    Serial.println(WiFi.status());
    c++;
    delay(500);
  }
  return false;
}

void WIFIreconnect()
{
  delay(10);
  // We start by connecting to a WiFi network
  //  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    for (int i = 0; i < 5; i++)
    {
      digitalWrite(LedStatePin, HIGH);
      delay(50);
      digitalWrite(LedStatePin, LOW);
      delay(50);
    }
    //
    Serial.println("Connecting to WiFi..");
    digitalWrite(LedStatePin, HIGH);
  }
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println(" connected");
  //
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //
  //  LEDAllState(2, 1000);
  //  LEDAllState(0, 1000);
}

void MQTTreconnect()
{
  while (!client.connected())
  {
    Serial.println("Connecting to MQTT...");
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(MQTT_PUB_HandShake, "I'm In~");
      // ... and resubscribe
      Serial.println(client.subscribe(MQTT_SUB_ACK));
      //      LEDAllState(2, 1000);
      //      LEDAllState(0, 1000);
    }
    else
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      //      LEDAllState(1, 1250);
      //      LEDAllState(0, 1250);
      // delay(5000); // Wait 5 seconds before retrying
      /*
        client.state() Returns
        -4 : MQTT_CONNECTION_TIMEOUT - the server didn't respond within the keepalive time
        -3 : MQTT_CONNECTION_LOST - the network connection was broken
        -2 : MQTT_CONNECT_FAILED - the network connection failed
        -1 : MQTT_DISCONNECTED - the client is disconnected cleanly
        0 : MQTT_CONNECTED - the client is connected
        1 : MQTT_CONNECT_BAD_PROTOCOL - the server doesn't support the requested version of MQTT
        2 : MQTT_CONNECT_BAD_CLIENT_ID - the server rejected the client identifier
        3 : MQTT_CONNECT_UNAVAILABLE - the server was unable to accept the connection
        4 : MQTT_CONNECT_BAD_CREDENTIALS - the username/password were rejected
        5 : MQTT_CONNECT_UNAUTHORIZED - the client was not authorized to connect
      */
    }
  }
}

char* toCharArray(String str)
{
  return &str[0];
}
