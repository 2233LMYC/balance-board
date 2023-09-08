#include <ESP8266WiFi.h>
#include <String.h>
#include <Ticker.h>

Ticker timer1;

#define MAX_SRV_CLIENTS 6            //定义可连接的客户端数目最大值
const char* ssid = "ESP8266ttt";           //账号
const char* password = "123456789";   //密码

WiFiServer server(2233);                    //端口号
WiFiClient serverClients[MAX_SRV_CLIENTS];  //客户端

char message[20] = {0};

char* p1 = NULL;
int cnt = 0;

static int rec_cnt = 0;
uint8_t i;

char temp1[5] = {0};
char temp2[5] = {0};
char temp3[5] = {0};
char send_buff[20] = {0};

uint8_t sbuf[20];
size_t counti = 0;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);           //AP模式
  WiFi.softAP(ssid, password);  //配置WIFI
  IPAddress m_ip = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(m_ip);
  timer1.attach(0.01,tome1_callback);
  server.begin();          //启动UART传输和服务器
  server.setNoDelay(true); //无发送延时
}

void loop() {
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
        //Serial.print("CONNECTED");
        ESP.wdtFeed();
        continue;
      }
    }
    ESP.wdtFeed();
  }
}

void tome1_callback(void)
{
  //读取WIFI端接收到的信息
  if (serverClients[rec_cnt] && serverClients[rec_cnt].connected()) {
      if (serverClients[rec_cnt].available()) {
        while (serverClients[rec_cnt].available())
        {
          message[cnt++] = serverClients[rec_cnt].read();
          if (message[cnt-1] == '\n') cnt = 0;
          ESP.wdtFeed();
        }
        p1 = strstr((const char*)message, (const char*)"[1]");
        ESP.wdtFeed();
        if (p1 != NULL)
        {
          temp1[0] = *(p1 + 3);
          temp1[1] = *(p1 + 4);
          temp1[2] = *(p1 + 5);
          sprintf((char*)send_buff, "%d",atoi((const char*)temp1));
          Serial.print(send_buff);
          ESP.wdtFeed();
        }
        //串口读取到的转发到wifi，因为串口是一位一位的发送所以在这里缓存完再发送
        if (Serial.available()) {   
        counti = Serial.available();
        Serial.readBytes(sbuf, counti);
        serverClients[rec_cnt].write(sbuf, counti);
        delay(200);
        }
        memset(message, 0, strlen(message));
        ESP.wdtFeed();
      }
    }
  }
