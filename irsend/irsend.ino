#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

/* PubSubClient.h の中の MQTT_MAX_PACKET_SIZE を大きくする */

const char *ssid = "***********";
const char *password = "***********";
const char *token = "token:***********";
const char *mqttserver = "mqtt.beebotte.com";



IRsend irsend(14);
WiFiClient wifiClient;
PubSubClient client(wifiClient);
char device[20];



// メッセージを受信したときのコールバック関数
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("callback invoked");

  const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(200) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(5) + 630;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  uint16_t codebuffer[200];

  payload[length] = '\0';
  String msg = String((char*) payload);
  Serial.println(msg);

  // jsonデコード
  JsonObject& object = jsonBuffer.parseObject(msg);
  if (!object.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  // コマンド取得
  const char* command = object["data"][0]["cmd"];
  String cmd = String(command);
  Serial.println(cmd);

  // コード取得
  int codelength =  object["data"][0]["code"].size();
  Serial.println(codelength);
  if (codelength < 200) {
    for (int i = 0; i < codelength; i++) {
      codebuffer[i] = object["data"][0]["code"][i];
    }
  } else {
    Serial.println("code is tooooo long!");
    return;
  }

  // シグナル送信！
  Serial.println("send signal.");
  irsend.sendRaw(codebuffer, codelength, 38);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  irsend.begin();
  Serial.println("");

  //wait for connection
  while ( WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttserver, 1883);
  client.setCallback(callback);

  String(ESP.getChipId()).toCharArray(device, 20);
}


void loop() {

  if (!!!client.connected()) {
    Serial.print("Reconnecting client to ");
    Serial.println(mqttserver);
    while (!!!client.connect(device , token, "")) {
      Serial.print(".");
      delay(500);
    }
    if (client.subscribe("IFTTT/ir")) {
      Serial.println("Subscribe ok");
    } else {
      Serial.println("Subscribe failed");
    }
    Serial.println();
  }
  client.loop();
}

