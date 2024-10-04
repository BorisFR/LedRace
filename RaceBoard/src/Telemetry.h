#ifndef TELEMETRY_h
#define TELEMETRY_h

#include "Arduino.h"
#include "HardwareConfiguration.h"
#include "EnumsAndConstants.h"
/*
#include <esp_now.h>
#include "WiFi.h"
#include <esp_err.h>
#include <esp_wifi.h>
*/
class Telemetry
{
public:
    Telemetry();
    void Setup();
    void Send(char txbuff[TX_COMMAND_BUFLEN]);

private:
    uint8_t broadcastAddress[ADDRESS_LENGTH];
    // esp_now_peer_info_t peerInfo;
    bool networkOk = false;
    struct_message myData;
};

#endif