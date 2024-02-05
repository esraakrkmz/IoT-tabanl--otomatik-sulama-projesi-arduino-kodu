#include <ESP8266WiFi.h>
#include<Firebase_ESP_Client.h>
#include"addons/TokenHelper.h"
#include"addons/RTDBHelper.h"
const char* WIFI_SSID = "E İPhone"; //çift tırnaklar arasıa wifi ağınızın adını giriniz
const char* WIFI_PASSWORD = "EdaEsra3632"; 

WiFiServer server(80);
#define API_KEY "VERİ TABANINIZIN API KEYİNİ GİRİNİZ."
#define DATABASE_URL "VERİ TABANINIZIN URLSİNİ GİRİNİZ"
#define role D4
#define nemsensor A0

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis= 0;
bool signupOK = false;
float nemdegeri ;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting to wi-fi");
  while(WiFi.status()!= WL_CONNECTED){
    Serial.print("."); delay(300);       
    }
 Serial.println();
 Serial.print("Connected with IP: ");
 Serial.println(WiFi.localIP());
 Serial.println();

 config.api_key = API_KEY;
 config.database_url = DATABASE_URL;
 if(Firebase.signUp(&config, &auth, "", "")){
    Serial.println("signUp OK");
    signupOK = true;
  }else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }
    config.token_status_callback = tokenStatusCallback;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
    if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  pinMode(role,OUTPUT);
  pinMode(nemsensor,INPUT);
  digitalWrite(role,HIGH);
  Serial.begin(9600);
}



void loop() {
  if(Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 
    || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
     // Your code here 
      int nem = analogRead(nemsensor);

      if(Firebase.RTDB.setInt(&fbdo, "Ölçülennem", nem)){
        Serial.println(); Serial.print(nem);
        Serial.print(" -successfully saved to : " + fbdo.dataPath());
        Serial.println(" (" +fbdo.dataType() + ")");
        }else{
            Serial.println("FAILED: " + fbdo.errorReason());
          }
        if(nem>=580){
         digitalWrite(role,LOW);
         delay(1000);
         digitalWrite(role,HIGH);
         DateTime now = rtc.now();

         // Saati ve tarihi oku
         int hour = now.hour();
         int minute = now.minute();
         int second = now.second();
         int day = now.day();
         int month = now.month();
         int year = now.year();

       // Firebase'e veriyi yaz
        Firebase.setInt("saat", hour);
        Firebase.setInt("dakika", minute);
        Firebase.setInt("saniye", second);
        Firebase.setInt("gun", day);
        Firebase.setInt("ay", month);
        Firebase.setInt("yil", year);

         delay(5000);
         }
       else{
         digitalWrite(role,HIGH);
       }
    }

}
