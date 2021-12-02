#include <Arduino.h>
#include "ESP8266WiFi.h"
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <PubSubClient.h>

#include <ArduinoJson.h>
DynamicJsonDocument doc(128);
char json[128];

WiFiClient espClient;
PubSubClient client(espClient);

const char *mqtt_server = "api.easyprint.abbgymnasiet.se";

#include <SoftwareSerial.h>
#define TX 15
#define RX 13
#define pin1 12

SoftwareSerial MaixPY = SoftwareSerial(RX, TX);

bool lock;
bool grind;

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

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");

  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  DynamicJsonDocument doc(1024);
  char json[256];

  if (String(topic) == String("get_state"))
  {
    doc["grind"] = grind;
    doc["lock"] = lock;

    serializeJson(doc, json, 256);

    client.publish("state", json);
  }
}

void lockGate()
{
  change_servo(false);
  doc.clear();
  doc["state"] = false;
  serializeJson(doc, json, 128);

  client.publish("le_lock", json);
}
void unlockGate()
{
  change_servo(true);
  doc.clear();
  doc["state"] = true;
  serializeJson(doc, json, 128);

  client.publish("le_lock", json);
}

void change_servo(bool state)
{

  // servo.wrire(23143);
}

void setup()
{
  Serial.begin(9600);

  pinMode(pin1, INPUT_PULLUP);

  WiFiManager wifiManager;

  if (!wifiManager.autoConnect("HASPAP", "Hasp"))
  {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
  Serial.println("connected to ");
  Serial.print(WiFi.localIP());
  client.setServer(mqtt_server, 6969);
  client.setCallback(callback);
}

bool oldButton;
bool newButton;

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  newButton = digitalRead(pin1);
  // Serial.println(newButton);
  if (newButton != oldButton)
  {
    Serial.print("ButtonChange: ");
    Serial.println(!newButton);
    oldButton = newButton;
    doc.clear();
    doc["state"] = newButton;
    serializeJson(doc, json, 128);

    client.publish("le_grind", json);
  }
}
// flush input