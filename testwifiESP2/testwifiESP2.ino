
#include <esp_now.h>
#include <WiFi.h>

#define A0 26
#define A1 25
#define A2 34
#define A3 39
#define A4 36
#define A5 4

#define D2 14
#define D3 32
#define D4 15
#define D5 14

#define RX RX
#define TX TX

#define SCL 22
#define SDA 23
#define BB 32
#define BC 14

bool valide = false;

unsigned long last=0;
unsigned long last_deplacement=0;
unsigned long attente=0;

void IRAM_ATTR tmp(){
  if(millis()-last>200){
    valide = !valide;
    last=millis();
  }
}

uint8_t broadcastAddress[] = {0xA8, 0x03, 0x2A, 0xEA, 0xF1, 0x1C};// REPLACE WITH RECEIVER MAC ADDRESS
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
 int x,y;
} struct_message;
struct_message myData;

char dataRcv[15];
// callbacks for sending and receiving data
/*void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
 memcpy(&dataRcv, incomingData, sizeof(dataRcv));
 Serial.print("\r\nBytes received: ");
 Serial.println(len);
 Serial.print("From slave: ");
 Serial.println(dataRcv);
 Serial.println();
}*/

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println(myData.x);
  Serial.println(myData.y);
}


void setup() {
  pinMode(D4, INPUT_PULLUP); // 
  pinMode(A2, INPUT); // joystique
  pinMode(A3, INPUT); // joystique
  attachInterrupt (D4, tmp, FALLING);

  
 // Init Serial Monitor
 Serial.begin(115200);
 // Set device as a Wi-Fi Station
 WiFi.mode(WIFI_STA);
 // Init ESP-NOW
 if (esp_now_init() != ESP_OK) {
   Serial.println(F("Error initializing ESP-NOW"));
   return;
 }
 Serial.print(F("Reciever initialized : "));
 Serial.println(WiFi.macAddress());
 
 // Define callback functions
 esp_now_register_send_cb(OnDataSent);
 esp_now_register_recv_cb(OnDataRecv);
 // Register peer
 esp_now_peer_info_t peerInfo;
 memcpy(peerInfo.peer_addr, broadcastAddress, 6);
 peerInfo.channel = 0;
 peerInfo.encrypt = false;
 // Add peer
 if (esp_now_add_peer(&peerInfo) != ESP_OK) {
   Serial.println(F("Failed to add peer"));
   return;
 }
}

int absolue(int a){
  if (a>=0){
    return a;
  }
  else{
    return -a;
  }
}

void demande_coordonne(int& x, int& y,int j) {
  x=0;
  y=0;
  //affiche_tableau(j);
  //rectangle_plein(x*6+67,y*6,6,6);
  //display.display();
  while (!valide){
    int sensorValuex = analogRead(A3)-1800;
    int sensorValuey = analogRead(A2)-1800;
    if (absolue(sensorValuex)>absolue(sensorValuey) and sensorValuex!=2295){
        if(sensorValuex>500){
          if(millis()-last_deplacement>200){
            x=(x+1)%10;
            Serial.print(x);
            Serial.print(" ");
            Serial.println(y);

            //affiche_tableau(j);
            //rectangle_plein(x*6+67,y*6,6,6);
            //display.display();
            last_deplacement=millis();
          }
        }
        if(sensorValuex<-500){
          if(millis()-last_deplacement>200){
            x--;
            Serial.print(x);
            Serial.print(" ");
            Serial.println(y);
            if(x==-1){
              x=9;
            }

            //affiche_tableau(j);
            //rectangle_plein(x*6+67,y*6,6,6);
            //display.display();
            last_deplacement=millis();
          }
        }
      }
      else{
        if(sensorValuey<-500){
          if(millis()-last_deplacement>200){
            y--;
            Serial.print(x);
            Serial.print(" ");
            Serial.println(y);
            if(y==-1){
              y=9;
            }

            //affiche_tableau(j);
            //rectangle_plein(x*6+67,y*6,6,6);
            //display.display();
            last_deplacement=millis();
          }
        }
        if(sensorValuey>500){
          if(millis()-last_deplacement>200){
            y=(y+1)%10;
            Serial.print(x);
            Serial.print(" ");
            Serial.println(y);

            //affiche_tableau(j);
            //rectangle_plein(x*6+67,y*6,6,6);
            //display.display();
            last_deplacement=millis();
          }
        }
      }
    }
  valide=!valide;
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
}


void loop() {
  demande_coordonne(myData.x,myData.y,1);
}
