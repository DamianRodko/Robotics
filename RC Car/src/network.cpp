#include "network.h"
#include "motor.h"
#include "servo.h"
#include <WebSocketsServer.h>
#include <WebServer.h>
#include <WiFi.h>
#include <LittleFS.h>
#include "neopixel.h"
#include "volt.h"

const char* ssid = "Damian's ESP32";
const char* password = "12345678";
WebServer server(80);
WebSocketsServer ws = WebSocketsServer(81);

void wifiInit()
{
  WiFi.softAP(ssid, password);
  Serial.println("WiFi Started");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());
  ws.begin();
  ws.onEvent(onWebSocketEvent);
  Serial.println("WebSocket Connected");
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
}

void wifiHandle()
{
  server.handleClient();
  ws.loop();
  //neopixel battery status
  static unsigned long lastVolt = 0;
    if (millis() - lastVolt > 5000)
    {
        lastVolt = millis();
        float v = voltRead();

        // WebSocket broadcast
        String msg = "{\"voltage\":" + String(v, 1) + "}";
        ws.broadcastTXT(msg);

        // NeoPixel status
        if (v > 11.1)
        {
            setNeopixel(0, 255, 0);   // green
        }
        else if (v > 10.5)
        {
            setNeopixel(255, 120, 0); // yellow
        }
        else
        {
            setNeopixel(255, 0, 0);   // red
        }
    }
}

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length)
{
  if (type == WStype_DISCONNECTED)
  {
    motorStop();
    setServo(90);
  }
  else if (type == WStype_TEXT)
  {
    String msg = String((char*)payload, length);
    Serial.println("WS received: " + msg);
    int motorVal = 0;
    int servoVal = 90;

    int mIndex = msg.indexOf("\"motor\":");
    int sIndex = msg.indexOf("\"servo\":");

    if (mIndex != -1)
    {
      motorVal = msg.substring(mIndex + 8).toInt();
    }
    if (sIndex != -1)
    {
      servoVal = msg.substring(sIndex + 8).toInt();
    }
    setServo(servoVal);

    int spd = constrain(abs(motorVal), 0, 255);

    if (motorVal > 0)
    {
      motorForward(spd);
    }
    else if (motorVal < 0)
    {
      motorReverse(spd);
    }
    else
    {
      motorStop();
    }
  }
}

void handleRoot()
{
  server.serveStatic("/", LittleFS, "/index.html");
}

void handleNotFound()
{
  Serial.print("No handler for: ");
  Serial.println(server.uri());
  server.send(404, "text/plain", "Not found");
}