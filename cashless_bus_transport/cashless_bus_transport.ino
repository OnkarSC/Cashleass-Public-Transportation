#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#define WIFI_SSID "project"            // WIFI SSID here                                   
#define WIFI_PASSWORD "1234567890"        // WIFI password here

const char* host = "script.google.com";
const int httpsPort = 443;
//----------------------------------------

WiFiClientSecure client; //--> Create a WiFiClientSecure object.
String GAS_ID = "AKfycby-dirlJ12SoQtS5MesKEV07L7IWYpi6jbtSilEPIZC2B9PH1d21QSGMs2IMneUuzDC"; //--> spreadsheet script ID 

#include <Wire.h>  
#include <LiquidCrystal_I2C.h>  
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include<Servo.h>
Servo s;

#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN D4
#define RST_PIN D3
MFRC522 mfrc522(SS_PIN, RST_PIN);

int o = 0;
int o1 = 20;

String content= "";
  byte letter;

  int sen1 = 0;
  int sen2 = 0;
  int psg = 0;

void setup() {
 Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  s.attach(D0);
  s.write(90);
  lcd.begin();   // initializing the LCD
  lcd.backlight();
  WiFi.mode(WIFI_STA);           
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
//Serial.print("Connecting to ");
//Serial.print(WIFI_SSID);
while (WiFi.status() != WL_CONNECTED) 
{ //Serial.print(".");
  lcd.clear();
  lcd.print("Connecting ...");
    delay(500); }

    client.setInsecure();

    pinMode(D8,INPUT);
}

void loop() {
sen1 = digitalRead(D8);
sen2 = analogRead(A0);

if(sen1 == HIGH){psg++; delay(500);}
if(sen2 > 500)  {psg--; delay(500);}
if(psg == -1){psg = 0;}
if(psg < 10){

if (o == 0){
  lcd.clear();
  lcd.print("Scan Ur Card");
  lcd.setCursor(0,1);
  lcd.print("PSG CNT: ");
  lcd.print(psg);
  read_card();
  if (content.substring(1) == "9A 85 8E 19" || content.substring(1) == "81 74 39 08" || content.substring(1) == "B3 11 CE 04") //change here the UID of the card/cards that you want to give access
  {
    lcd.clear();
    lcd.print("SEL STATION");
    o = 1;
    content = "";
    delay(1000);
  }
}
  if(o == 1)
  {
    o1--;
     read_card();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("CBS STATION");
     lcd.setCursor(0,1);
    lcd.print("Rem Time:");
    lcd.print(o1);
    delay(300);
    if(o1 == 0)
    {
      o = 2;o1 = 20;
    }
    if (content.substring(1) == "9A 85 8E 19" || content.substring(1) == "81 74 39 08" || content.substring(1) == "B3 11 CE 04") //change here the UID of the card/cards that you want to give access
  {
    lcd.clear();
    lcd.print("CBS Selected.");
    lcd.setCursor(0,1);
    lcd.print("Fair 20 Rs");
    o = 0;
    o1 = 20;
    delay(1000);
    s.write(0);
    delay(3000);
    s.write(90);
    delay(1000);
    String card = content.substring(1);
    content = "";
    card.replace(' ','-');
   sendData(card, "CBS","20RS");
   card="";
   
  }
  }
    if(o == 2)
  {
    o1--;
    read_card();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("KRANTI CHOWK");
     lcd.setCursor(0,1);
    lcd.print("Rem Time:");
    lcd.print(o1);
    delay(300);
    if(o1 == 0)
    {
      o = 3;o1 = 20;
    }
   if (content.substring(1) == "9A 85 8E 19" || content.substring(1) == "81 74 39 08" || content.substring(1) == "B3 11 CE 04") //change here the UID of the card/cards that you want to give access
  {
    lcd.clear();
    lcd.print("K.C. Selected.");
    lcd.setCursor(0,1);
    lcd.print("Fair 40 Rs");
    o = 0;
    o1 = 20;
    delay(1000);
    s.write(0);
    delay(3000);
    s.write(90);
    delay(1000);
    String card1 = content.substring(1);
    content = "";
    card1.replace(' ','-');
    sendData(card1, "KRANTI_CHOWK","40RS");
    card1="";
  }
  }
   if(o == 3)
  {
    o1--;
    read_card();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("BABA PETROL P.");
     lcd.setCursor(0,1);
    lcd.print("Rem Time:");
    lcd.print(o1);
    delay(300);
    if(o1 == 0)
    {
      o = 0;o1 = 20;
    }
    if (content.substring(1) == "9A 85 8E 19" || content.substring(1) == "81 74 39 08" || content.substring(1) == "B3 11 CE 04") //change here the UID of the card/cards that you want to give access
  {
    lcd.clear();
    lcd.print("BPP Selected.");
    lcd.setCursor(0,1);
    lcd.print("Fair 60 Rs");
    o = 0;
    o1 = 20;
    delay(1000);
    s.write(0);
    delay(3000);
    s.write(90);
    delay(1000);
    String card2 = content.substring(1);
    content = "";
    card2.replace(' ','-');
    sendData(card2, "BABA_PETROL_PUMP","60RS");
    card2="";
  }
  } 
delay(100);
}
else
{
  lcd.clear();
  lcd.print("bus if full");
  delay(200);
}
}

void read_card()
{
   if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
}

void sendData(String card, String station, String fair) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  
  //----------------------------------------Connect to Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  else
  {
    Serial.println("Connected !!!");
  }

  String url = "/macros/s/" + GAS_ID + "/exec?value1=" + card + "&value2=" + station+ "&value3=" + fair;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  //----------------------------------------

  //----------------------------------------Checking whether the data was sent successfully or not
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  //----------------------------------------
}
