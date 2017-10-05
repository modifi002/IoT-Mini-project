//----------- set wifi--------------------------------
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
ESP8266WiFiMulti WiFiMulti;

//----------เรียกใช้ Library ของ ADS1015-----------------------------
#include <Wire.h>
#include <Adafruit_ADS1015.h>
// Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
Adafruit_ADS1015 ads;     /* Use thi for the 12-bit version */

#define Buzzer D6
#define led_AML D7
#define LINE_TOKEN "Line ToKen" //ที่ได้จาก https://notify-bot.line.me/my/

String message1 = "%E0%B8%82%E0%B8%93%E0%B8%B0%E0%B8%99%E0%B8%B5%E0%B9%89%E0%B8%9C%E0%B8%B9%E0%B9%89%E0%B8%AA%E0%B8%B9%E0%B8%87%E0%B8%AD%E0%B8%B2%E0%B8%A2%E0%B8%B8%E0%B9%80%E0%B8%81%E0%B8%B4%E0%B8%94%E0%B8%81%E0%B8%B2%E0%B8%A3%E0%B8%AB%E0%B8%81%E0%B8%A5%E0%B9%89%E0%B8%A1%20%E0%B8%81%E0%B8%A3%E0%B8%B8%E0%B8%93%E0%B8%B2%E0%B9%80%E0%B8%82%E0%B9%89%E0%B8%B2%E0%B9%84%E0%B8%9B%E0%B8%8A%E0%B9%88%E0%B8%A7%E0%B8%A2%E0%B9%80%E0%B8%AB%E0%B8%A5%E0%B8%B7%E0%B8%AD%E0%B8%AB%E0%B8%A3%E0%B8%B7%E0%B8%AD%E0%B9%82%E0%B8%97%E0%B8%A3%E0%B9%81%E0%B8%88%E0%B9%89%E0%B8%87%E0%B8%9C%E0%B8%B9%E0%B9%89%E0%B8%97%E0%B8%B5%E0%B9%88%E0%B8%AD%E0%B8%A2%E0%B8%B9%E0%B9%88%E0%B9%84%E0%B8%81%E0%B8%A5%E0%B9%89%E0%B9%80%E0%B8%84%E0%B8%B5%E0%B8%A2%E0%B8%87%E0%B9%80%E0%B8%9E%E0%B8%B7%E0%B9%88%E0%B8%AD%E0%B9%80%E0%B8%82%E0%B9%89%E0%B8%B2%E0%B8%8A%E0%B9%88%E0%B8%A7%E0%B8%A2%E0%B9%80%E0%B8%AB%E0%B8%A5%E0%B8%B7%E0%B8%AD" ;
//ขณะนี้ผู้สูงอายุเกิดการหกล้ม กรุณาเข้าไปช่วยเหลือหรือโทรแจ้งผู้ที่อยู่ไกล้เคียงเพื่อเข้าช่วยเหลือ
String message2 = "%E0%B9%84%E0%B8%94%E0%B9%89%E0%B8%A3%E0%B8%B1%E0%B8%9A%E0%B8%81%E0%B8%B2%E0%B8%A3%E0%B8%8A%E0%B9%88%E0%B8%A7%E0%B8%A2%E0%B9%80%E0%B8%AB%E0%B8%A5%E0%B8%B7%E0%B8%AD%E0%B9%81%E0%B8%A5%E0%B9%89%E0%B8%A7";
//ได้รับการช่วยเหลือแล้ว

byte n = 2;
byte n2 = 0;
int count = 0;
int count2 = 0;
int count3 = 0;
byte sw_st = 0;

void Wifi_manag(){
  WiFiManager wifiManager;
  wifiManager.setTimeout(300);
  digitalWrite(led_AML , HIGH);
  if(!wifiManager.autoConnect("ES")) {
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  } 
}
void Wifi_re(){
  if(WiFi.status() != WL_CONNECTED){
    WiFi.begin();
      while (WiFi.status() != WL_CONNECTED) {
        //offline mode
        Serial.println("Disconnect..");
        delay(50);
    }
  }
    Serial.println("connect..");
}
void setup(void) 
{
  Serial.begin(9600);
  
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  
  ads.begin(); //ให้ ADS1015 เริ่มทำงาน
  pinMode(D4,INPUT);
  pinMode(Buzzer ,OUTPUT);
  pinMode(led_AML,OUTPUT);
  digitalWrite(Buzzer , HIGH);
   Wifi_manag();
  digitalWrite(Buzzer , LOW);
  delay(50);
  digitalWrite(Buzzer , HIGH);
  delay(50);
  digitalWrite(Buzzer , LOW);
  delay(50);
  digitalWrite(Buzzer , HIGH);
  
  digitalWrite(led_AML , LOW);
}

void loop(void) 
{
  int16_t x, y, z; //กำหนดตัวแปร x y z ให้รับค่าเป็นจำนวนเต็ม 16 บิต
  x = ads.readADC_SingleEnded(0); //รับค่า แกน X จาก ADS1015 ที่ A0
  y = ads.readADC_SingleEnded(1); //รับค่า แกน Y จาก ADS1015 ที่ A1
  z = ads.readADC_SingleEnded(2); //รับค่า แกน Z จาก ADS1015 ที่ A2
  Serial.print("X : "); Serial.print(x); //แสดงค่าแกน x
  Serial.print(" Y : "); Serial.print(y); //แสดงค่าแกน y
  Serial.print(" Z : "); Serial.print(z); //แสดงค่าแกน z
  Serial.println(" ");
  delay(100);
  Wifi_re();
  digitalWrite(led_AML , HIGH);
  delay(100);
  digitalWrite(led_AML , LOW);
  /*Serial.print("count = ");
  Serial.println(count);
  Serial.print("n = ");
  Serial.println(n);
  Serial.print("n2 = ");
  Serial.println(n2);
  Serial.print("count3 = ");
  Serial.println(count3);*/
  
  if((x > 640 || x < 450) && n == 2 && n2 == 0 || (z > 645 || z < 450) && n == 2 && n2 == 0) {
    //เงื่อนไข คือ ถ้าค่าที่รับมาจากแกน X หรือ Z มีค่ามากกว่า 640 หรือน้อยกว่า 450 และ n = 2 และ n2 = 0
      digitalWrite(Buzzer , LOW);
      n = 0; //ให้ n = 0 เพื่อให้เข้าเงื่อนไขถัดไป
      count3 = count3 + 1000; // 1000 = 1 วินาที 
      sw_st = digitalRead(D4); //รับค่าจากปุ่ม 
      if(sw_st == LOW){//ถ้ามีการกดปุ่ม
        digitalWrite(Buzzer , HIGH); //ให้เสียงดับ
        digitalWrite(led_AML , LOW); //ให้ไฟดับ
        n = 3; //ให้ n=3 เพื่อไม่ให้เข้าเงื่อนไขใดๆ
        count3 = 0; //รีเซ็ตค่า count3 ให้เป็นค่าเริ่มต้น 
      }
    delay(1000);
    }
    ///------------ผ่านไป 10 วินาที------------//
    x = ads.readADC_SingleEnded(0);  // อ่านค่าจากขาอะนาล็อก A0
    z = ads.readADC_SingleEnded(2); // อ่านค่าจากขาอะนาล็อก A2
    if((x > 640 || x < 450)&& n == 0 && count3 == 10000 || (z > 645 || z < 450) && n == 0 && count3 == 10000 || count == 60000 ){
      //เงื่อนไข คือ ถ้าค่าที่รับมาจากแกน X หรือ Z มีค่ามากกว่า 640 หรือน้อยกว่า 450 และ n=0 และ count3 = 10000 หรือ count = 60000
      //count3 = 10000 คือ เมื่อเวลาผ่านไป 10 วินาที
      //count = 60000 คือ เมื่อเวลาผ่านไป 1 นาที
      digitalWrite(Buzzer , HIGH); //ให้เสียงดับ
      digitalWrite(led_AML , HIGH); //ให้ไฟคิด
      Serial.println("Sent to Line1");
      Line_Notify(message1); // ส่งข้อความที่1 ไปที่ Line
      n = 1; //ให้ n = 1 เพื่อไม่ให้เข้าเงื่อนไขนี้ และฟังก์ชันต่อไป
      //Serial.println(n);  
      delay(1000);
      count = 0;
      count3 = 0;
    }
    //-----------------หลังจากส่ง Line ครั้งแรก--------------//
    x = ads.readADC_SingleEnded(0);  // อ่านค่าจากขาอะนาล็อก A0
    z = ads.readADC_SingleEnded(2); // อ่านค่าจากขาอะนาล็อก A2
    if((x > 640 || x < 450)&& n == 1 || (z > 645 || z < 450) && n == 1){
      //เงื่อนไข คือ ถ้าค่าที่รับมาจากแกน X หรือ Z มีค่ามากกว่า 640 หรือน้อยกว่า 450 และ n=1
      digitalWrite(Buzzer , LOW);
      delay(400);
      digitalWrite(Buzzer , HIGH);
      digitalWrite(led_AML , LOW);
      delay(100);
      count = count + 500; // 500 = 0.5 วินาที
      sw_st = digitalRead(D4); //รับค่าจากปุ่ม 
      if(sw_st == LOW){ //ถ้ามีการกดปุ่ม
        digitalWrite(Buzzer , HIGH);
        digitalWrite(led_AML , LOW);
        n = 3;
      }
    }

    x = ads.readADC_SingleEnded(0);  // อ่านค่าจากขาอะนาล็อก A0
    z = ads.readADC_SingleEnded(2); // อ่านค่าจากขาอะนาล็อก A2
    if((x < 640 && x > 450 && z < 645 && z > 450)){
      digitalWrite(Buzzer , HIGH);
      digitalWrite(led_AML , LOW);
      if(n == 1) Line_Notify(message2);
      sw_st = digitalRead(D4);
      if(sw_st == LOW){ //รับค่าจากปุ่ม 
        count2 = count2 + 1;
        Serial.println(count2);
        if(count2 == 3){  //หยุดการประมวลผลค่าที่รับมา
          digitalWrite(Buzzer , LOW); delay(50);
          digitalWrite(Buzzer , HIGH); delay(50);
          digitalWrite(Buzzer , LOW); delay(50);
          digitalWrite(Buzzer , HIGH);
          n2 = 3;
          count2 = 3;
        }
        if(count2 == 6){ //กลับมาประมวลผลปกติ
          digitalWrite(Buzzer , LOW); delay(100);
          digitalWrite(Buzzer , HIGH); delay(50);
          digitalWrite(Buzzer , LOW); delay(50);
          digitalWrite(Buzzer , HIGH); delay(50);
          digitalWrite(Buzzer , LOW); delay(50);
          digitalWrite(Buzzer , HIGH);
          count2 = 0;
          n2 = 0;
        }
        delay(100);
      }
      delay(100);
      n = 2;
      count = 0;
      count3 = 0;
    }
  
}
//--------------Line_Notify----------------------------
void Line_Notify(String message) {
  WiFiClientSecure client;
  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }
 
  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  
  client.print(req);
    
  delay(20);
  
  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-------------");
}
