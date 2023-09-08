#include <ESP8266WiFi.h>
#include <espnow.h>
#include <string.h>


uint8_t esp_mac1_address[] = {0x7c,0x87,0xce,0xb2,0x38,0xfb};//模块1的mac地址
uint8_t esp_mac2_address[] = {0x40,0x22,0xd8,0x84,0x40,0x01};
uint8_t esp_mac3_address[] = {0x58,0xbf,0x25,0xcf,0xcf,0x07};
uint8_t esp_mac4_address[] = {0x2c,0xf4,0x32,0x76,0x19,0x51};
uint8_t esp_mac5_address[] = {0x84,0xf3,0xeb,0xf3,0xd6,0xf7};
uint8_t esp_mac6_address[] = {0x50,0x02,0x91,0xda,0xdc,0x0a};


typedef struct struct_message{
    char m[32];
    int n;
};
struct struct_message ComingData;
struct struct_message SendData;

//Callback when data is sent
//发送信息时的回调函数，每次发送信息会自动调用该函数
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) 
{
  ;
}
 
// Callback when data is received
//接收信息时的回调函数，每次接收信息会自动调用该函数
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) 
{
  //在此处先通过mac地址判断是哪一个设备发送的数据，再进行数据解析（在主控中写代码时）
  //或者只是使用memcpy函数存储数据，在主函数中处理（类似于DMA的形式）
  //打印发送方的mac地址
  memcpy(&ComingData,incomingData,sizeof(ComingData));
  if(ComingData.n == 2233)
  {
    Serial.printf(ComingData.m);  
  //}
    //Serial.print("\r\n");
  }

}
 
void setup() {
 
  Serial.begin(115200);//初始化串口，设置其波特率为115200
  
  WiFi.mode(WIFI_STA);//设置WIFI模式为STA
  WiFi.disconnect();//断开WIFI连接
  
  // Init ESP-NOW
  if (esp_now_init() != 0)//初始化espnow
  {
//    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else
  {
//    Serial.println("esp_now init success");
    ;
  }
 
  // Set ESP-NOW Role
  //双向通信时需要设定角色为 ESP_NOW_ROLE_COMBO
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
 
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  //执行完这个函数，每次发送数据就会自动调用回调函数了
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  //与模块之间配对
//  esp_now_add_peer(esp_mac2_address, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
//  esp_now_add_peer(esp_mac3_address, ESP_NOW_ROLE_COMBO, 2, NULL, 0);
//  esp_now_add_peer(esp_mac4_address, ESP_NOW_ROLE_COMBO, 3, NULL, 0);
  esp_now_add_peer(esp_mac5_address, ESP_NOW_ROLE_COMBO, 4, NULL, 0);
  esp_now_add_peer(esp_mac6_address, ESP_NOW_ROLE_COMBO, 5, NULL, 0);
  
  // Register for a callback function that will be called when data is received
  //执行完这个函数，每次接收数据就会自动调用回调函数了
  esp_now_register_recv_cb(OnDataRecv);
}

static int i = 0;
char temp = 0; 

void loop() 
{
    // Send message via ESP-NOW
    //向从机发送数据
    if (Serial.available() > 0) //如果接收到串口数据
    {
      int i = 0;
      while (Serial.available() > 0) //循环读取串口数据
      {
        temp = Serial.read(); //转换字符到字符串
        SendData.m[i++] += temp;
        if(temp == '\n')
        {
          i = 0;
          SendData.n = 2233;
//          esp_now_send(esp_mac2_address, (uint8_t*)&SendData, sizeof(SendData));
//          esp_now_send(esp_mac3_address, (uint8_t*)&SendData, sizeof(SendData));
//          esp_now_send(esp_mac4_address, (uint8_t*)&SendData, sizeof(SendData));
          esp_now_send(esp_mac5_address, (uint8_t*)&SendData, sizeof(SendData));
          esp_now_send(esp_mac6_address, (uint8_t*)&SendData, sizeof(SendData));
          memset(SendData.m,0,strlen(SendData.m));
        }
      }
    }
    delay(100);
}
