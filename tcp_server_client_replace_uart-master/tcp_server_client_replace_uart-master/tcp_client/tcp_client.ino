#include <ESP8266WiFi.h>
#include <String.h>

#define STASSID "ESP8266ttt"
#define STAPSK  "123456789"

const char* remote_ip = "192.168.4.1";
unsigned int remote_port = 2233;      // local port to listen on
uint8_t sbuf[5];
size_t counti = 0;
WiFiClient client;

void setup()
{
  Serial.begin(115200);
  Serial.println("STA MODE");
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  Serial.println("WiFi connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  Serial.println("WiFi connected!!!");

  Serial.println("tcp server connecting...");
  while (client.status() == CLOSED) {
    client.connect(remote_ip, remote_port);
    delay(100);
  }
  client.setNoDelay(true); //无发送延时
  Serial.println("tcp server connected!");

}

void loop()
{
  if (client.status() == CLOSED) {
    //意外断开重连
    Serial.println("tcp server Connecting...");
    while (client.status() == CLOSED) {
      client.connect(remote_ip, remote_port);
      delay(100);
    }
    Serial.println("tcp server Connected!");
  }

   if (Serial.available()) {   //串口读取到的转发到wifi，因为串口是一位一位的发送所以在这里缓存完再发送
    counti = Serial.available();
    Serial.readBytes(sbuf, counti);
    client.write(sbuf, counti);
    delay(200);
   }
}
