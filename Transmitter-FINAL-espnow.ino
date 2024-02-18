#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>
Servo myservo; 

#define X_AXIS_PIN 32
#define Y_AXIS_PIN 34
#define SWITCH_PIN 14

int xVal;
int yVal;
int butVal;
int Xpwm;
int Ypwm;
int Xcallib;
int Ycallib;
int switchVal;
//RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x3C,0xE9,0x0E,0x85,0x9A,0x7C};  //3C:E9:0E:85:9A:7C

typedef struct struct_message {
  
  byte xAxisValue;
  byte yAxisValue;
  byte switchPressed;
} struct_message;

//struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;




// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
    pinMode(X_AXIS_PIN,INPUT);
    pinMode(Y_AXIS_PIN,INPUT);
    pinMode(SWITCH_PIN,INPUT_PULLUP);
    myservo.attach(5);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
 xVal=analogRead(X_AXIS_PIN);
 yVal=analogRead(Y_AXIS_PIN);
 switchVal=digitalRead(SWITCH_PIN);

 if (xVal >= 1400)
    {
      Xcallib = map(xVal, 1100, 2640, 90, 180);
    }
    else if (xVal <= 1100)
    {
      Xcallib = map(xVal, 1100, 0, 90, 0);  
    }
    else
    {
      Xcallib = 90;
  }

   if (yVal >= 1400)
    {
      Ycallib = map(yVal, 1100, 2640, 90, 180);
    }
    else if (yVal <= 1100)
    {
      Ycallib = map(yVal, 1100, 0, 90, 0);  
    }
    else
    {
      Ycallib = 90;
  }

  myData.xAxisValue = Xcallib;
  myData.yAxisValue = Ycallib;  
  myData.switchPressed = switchVal; 
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
Serial.print(Xcallib);
Serial.print("    :    ");
Serial.println(Ycallib);
delay(10);  
}