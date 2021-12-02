#include <Arduino.h>
#include "ESP8266WiFi.h"
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

const char *mqtt_server = "api.easyprint.abbgymnasiet.se";

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish(topic, "hello world");
      // ... and resubscribe
      client.subscribe("get_state");
      client.subscribe("lock");
      client.subscribe("unlock");

      // client.subscribe("unlocked");
      // client.subscribe("locked");
      // client.subscribe("opend");
      // client.subscribe("closed");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  String topicStr;
  String payloadStr;

  for (int i = 0; topic[i]; i++)
  {
    topicStr += topic[i];
  }

  for (int i = 0; i < length; i++)
  {
    payloadStr += (char)payload[i];
  }

}

void setup()
{
  Serial.begin(9600);

  WiFiManager wifiManager;

  if (!wifiManager.autoConnect("HASPAP", "Hasp"))
  {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
  Serial.println("connected to" + WiFi.localIP());
  client.setServer(mqtt_server, 6969);
  client.setCallback(callback);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}
// flush input