#include "network.h"
#include "motor.h"
#include "neopixel.h"
#include "servo.h"
#include "volt.h"
#include <LittleFS.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <WiFi.h>

namespace
{
unsigned long lastControlMessageMs = 0;
bool controlWatchdogArmed = false;
WebServer server(80);
WebSocketsServer webSocket(81);

void enterControlFailsafe()
{
  motorStop();
  centerServo();
  controlWatchdogArmed = false;
}

void handleRoot()
{
  File file = LittleFS.open("/index.html", "r");
  if (!file)
  {
    server.send(404, "text/plain", "File not found");
    return;
  }

  server.sendHeader("Cache-Control", "no-store, no-cache, must-revalidate, max-age=0");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "0");
  server.streamFile(file, "text/html");
  file.close();
}

void handleNotFound()
{
  Serial.print("No handler for: ");
  Serial.println(server.uri());
  server.send(404, "text/plain", "Not found");
}

void onWebSocketEvent(uint8_t clientNumber, WStype_t type, uint8_t* payload, size_t length)
{
  (void)clientNumber;

  if (type == WStype_DISCONNECTED)
  {
    enterControlFailsafe();
    return;
  }

  if (type != WStype_TEXT)
  {
    return;
  }

  const String message(reinterpret_cast<char*>(payload), length);
  const int motorIndex = message.indexOf("\"motor\":");
  const int servoIndex = message.indexOf("\"servo\":");

  if (motorIndex == -1 || servoIndex == -1)
  {
    Serial.println("Ignoring malformed control message");
    return;
  }

  const int motorValue = static_cast<int>(constrain(message.substring(motorIndex + 8).toInt(), -100L, 100L));
  const int servoValue = static_cast<int>(constrain(message.substring(servoIndex + 8).toInt(),static_cast<long>(servoMinAngle),static_cast<long>(servoMaxAngle)));

  setServo(servoValue);
  const int speed = abs(motorValue);

  if (motorValue > 0)
  {
    motorForward(speed);
  }
  else if (motorValue < 0)
  {
    motorReverse(speed);
  }
  else
  {
    motorStop();
  }

  lastControlMessageMs = millis();
  controlWatchdogArmed = true;
}
}

void wifiInit()
{
  enterControlFailsafe();

  if (!WiFi.softAP("Damian's ESP32", "12345678"))
  {
    Serial.println("ERROR: WiFi access point failed to start");
    return;
  }

  Serial.println("WiFi started");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  Serial.println("WebSocket started");

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
}

void wifiHandle()
{
  server.handleClient();
  webSocket.loop();

  const unsigned long now = millis();
  if (controlWatchdogArmed && now - lastControlMessageMs >= 500UL)
  {
    Serial.println("Control timeout: stopping motor");
    enterControlFailsafe();
  }

  static unsigned long lastVoltageBroadcastMs = 0;
  if (now - lastVoltageBroadcastMs < 5000UL)
  {
    return;
  }

  lastVoltageBroadcastMs = now;
  const float voltage = voltRead();
  webSocket.broadcastTXT("{\"voltage\":" + String(voltage, 1) + "}");

  if (voltage > 11.1f)
  {
    setNeopixel(0, 255, 0);
  }
  else if (voltage > 10.5f)
  {
    setNeopixel(255, 120, 0);
  }
  else
  {
    setNeopixel(255, 0, 0);
  }
}
