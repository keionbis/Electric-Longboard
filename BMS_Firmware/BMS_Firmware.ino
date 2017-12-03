#include <esp_now.h>
#include <WiFi.h>
#include "esp32-hal-ledc.h"
#define CHANNEL 1
#define COUNT_LOW 0
#define COUNT_HIGH 1023
#define TIMER_WIDTH 10

#define PowerCheck 15
#define Drain1 25
#define Drain2 26
#define Drain3 27
#define Drain4 14
#define Drain5 12
#define Drain6 13
#define Batt6 36 
#define Batt5 39
#define Batt4 34
#define Batt3 35
#define Batt2 32
#define Batt1 33
#define currentSense 4

int State = 0;  //0 = driving State  1 = Charging State   2 =   

// Init ESP Now with fallback
void InitESPNow() {
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}

// config AP SSID
void configDeviceAP() {
  char* SSID = "Slave_1";
  bool result = WiFi.softAP(SSID, "Slave_1_Password", CHANNEL, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  }
}

void setup() {
  Serial.begin(115200);
  ledcAttachPin(32, 1);   // GPIO 22 assigned to channel 1
  ledcAttachPin(35, 2);   // GPIO 22 assigned to channel 1
  ledcSetup(7, 50, TIMER_WIDTH); // channel 1, 50 Hz, 16-bit width
  ledcSetup(8, 50, TIMER_WIDTH); // channel 1, 50 Hz, 16-bit width
  Serial.println("ESPNow/Basic/Slave Example");
  //Set device in AP mode to begin with
  WiFi.mode(WIFI_AP);
  // configure device AP mode
  configDeviceAP();
  // This is the mac address of the Slave in AP Mode
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
  esp_now_register_recv_cb(OnDataRecv);
}

// callback when data is recv from Master
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  //if(State == 0){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  Serial.print("Last Packet Recv Data: "); Serial.println(*data);
  Serial.println("");
  ledcWrite(1,map(*data,0,255,COUNT_LOW,COUNT_HIGH));
  delay(10);
  ledcWrite(2,map(*data,0,255,COUNT_LOW,COUNT_HIGH));
  delay(10);
  //}
//  else
//  {
//    //do nothing
//  }
  
}

void loop() {
//  if (analogRead(PowerCheck) >2000){
//    State = 1;
//  }
//  else
//  {
//    State = 0;
//  }
//  switch(State)
//  {
//    case(0):
//    
//      break;
//    case(1):
//      break;
//      
//  }
}
