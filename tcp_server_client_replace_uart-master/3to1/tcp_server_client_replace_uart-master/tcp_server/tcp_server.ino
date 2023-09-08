#include <ESP8266WiFi.h>
#include <String.h>

#define MAX_SRV_CLIENTS 6            //定义可连接的客户端数目最大值
const char* ssid = "ESP8266ttt";           //账号
const char* password = "123456789";   //密码

WiFiServer server(2233);                    //端口号
WiFiClient serverClients[MAX_SRV_CLIENTS];  //客户端

char message[128] = {0};
int cnt = 0;
int state = 0;

void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_AP);           //AP模式
  WiFi.softAP(ssid, password);  //配置WIFI
  IPAddress m_ip = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(m_ip);

  server.begin();          //启动UART传输和服务器
  server.setNoDelay(true); //无发送延时
}

void loop() {

  uint8_t i;
  char temp = 0;
  char temp1[5] = {0};
  char temp2[5] = {0};
  char temp3[5] = {0};
  char send_buff[20] = {0};
  //检查连接
  if (server.hasClient()) { //存在连接请求
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      //查找空的和已经断开连接的
      if (!serverClients[i] || !serverClients[i].connected()) {
        //如果是空的
        if (!serverClients[i]) {
          //停止连接
          serverClients[i].stop();
        }
        //连接新的
        serverClients[i] = server.available();
        //提示连接成功
        Serial.print("CONNECTED");
        continue;
      }
    }
  }
  //来自客户端的数据
  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      if (serverClients[i].available()) {
        while (serverClients[i].available()) 
        {
          char temp = serverClients[i].read();
          message[cnt++] = temp;
          if (temp == '\n') cnt = 0;
        }
        char* p1 = strstr((const char*)message, (const char*)"[1]");
        char* p2 = strstr((const char*)message, (const char*)"[2]");
        char* p3 = strstr((const char*)message, (const char*)"[3]");

        if (p1 != NULL)  
        {
          temp1[0] = *(p1+3);
          temp1[1] = *(p1+4);
          temp1[2] = *(p1+5);
          sprintf((char*)send_buff,"n0.val=%d\xff\xff\xff",atoi((const char*)temp1));
          Serial.print(send_buff);
        }
        else if (p2 != NULL)  
        {
          temp2[0] = *(p2+3);
          temp2[1] = *(p2+4);
          temp2[2] = *(p2+5); 
          sprintf((char*)send_buff,"n1.val=%d\xff\xff\xff",atoi((const char*)temp2));
          Serial.print(send_buff);
        }
        else if (p3 != NULL)  
        {
          temp3[0] = *(p3+3);
          temp3[1] = *(p3+4);
          temp3[2] = *(p3+5);
          sprintf((char*)send_buff,"n2.val=%d\xff\xff\xff",atoi((const char*)temp3));
          Serial.print(send_buff);
        }
        memset(message, 0, strlen(message));
        
      }
    }
  }
  /*
  //检查UART端口数据
  if (Serial.available()) {
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      if (serverClients[i] && serverClients[i].connected()) {
        //将UART端口数据转发到已连接的客户端
        serverClients[i].write(sbuf, len);
        delay(1);
      }
    }
  }*/
}
