#include <Arduino.h>
#include "ESP8266WiFi.h"
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <Servo.h>

#include <ArduinoJson.h>

#include "main.h"

Servo servo;

DynamicJsonDocument doc(128);
char json[128];

WiFiClient espClient;
PubSubClient client(espClient);

const char *mqtt_server = "api.easyprint.abbgymnasiet.se";

#include <SoftwareSerial.h>
#define TX 15
#define RX 13
#define pin1 12
#define servoPin 14

SoftwareSerial GSM = SoftwareSerial(RX, TX);

bool lock;
bool grind;
unsigned long long lastPress = millis();

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
  else if (String(topic) == String("unlock")) {
    unlockGate();
  }
  else if (String(topic) == String("lock")) {
    lockGate();
  }
}

void lockGate()
{
  change_servo(false);
  doc.clear();
  doc["state"] = false;
  lock = false;
  serializeJson(doc, json, 128);

  client.publish("le_lock", json);
}
void unlockGate()
{
  change_servo(true);
  doc.clear();
  doc["state"] = true;
  lock = true;
  serializeJson(doc, json, 128);

  client.publish("le_lock", json);
}

void change_servo(bool state)
{
  if (state) {
    servo.write(0);
  } else {
    servo.write(180);
  }
  // servo.wrire(23143);
}

void setup()
{
  Serial.begin(9600);

  pinMode(pin1, INPUT_PULLUP);
  servo.attach(servoPin);

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
  servo.write(90);
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
  if (newButton != oldButton && millis() > lastPress + 100)
  {
    Serial.print("ButtonChange: ");
    Serial.println(!newButton);
    oldButton = newButton;
    doc.clear();
    doc["state"] = newButton;
    serializeJson(doc, json, 128);

    client.publish("le_grind", json);
    lastPress = millis();
  }

    
  if (Serial.available() > 0) {
    // read the incoming byte:
    String s = Serial.readStringUntil('\n');
    s.replace("\r", "");
    // say what you got:
    Serial.print("I got: @");
    Serial.print(s);
    Serial.println("@");
    char buf[100];
    s.toCharArray(buf, 100);
    client.publish("Serial", buf);
    servo.write(s.toInt());
    
  }
}
// flush input