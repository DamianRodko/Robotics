#ifndef WIFI_H
#define WIFI_H

#include <WebSocketsServer.h>

void wifiInit();
void wifiHandle();
void handleRoot();
void handleNotFound();
void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);

#endif