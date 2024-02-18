#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>
Servo myservo;  // create servo object to control a servo
#define SIGNAL_TIMEOUT 1000  // This is signal timeout in milli seconds. We will reset the data if no signal
unsigned long lastRecvTime = 0;

typedef struct struct_message {
  
  byte xAxisValue;
  byte yAxisValue;
  byte switchPressed;
} struct_message;

// Create a struct_message called myData
struct_message myData;



// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) 
{
  if (len == 0)
  {
    return;
  }
  memcpy(&myData, incomingData, sizeof(myData));
  String inputData ;
  inputData = inputData + "values " + myData.xAxisValue + "  " + myData.yAxisValue + "  " + myData.switchPressed;
  Serial.println(inputData);
  
  lastRecvTime = millis();   
}


void setup() 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
myservo.attach(5);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() 
{
  myservo.write(myData.xAxisValue);
  unsigned long now = millis(); //Check Signal lost.
  Serial.println(myData.xAxisValue);
  delay(10);
}
