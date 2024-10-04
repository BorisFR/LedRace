#ifndef TELEMETRY_h
#define TELEMETRY_h

#include "Arduino.h"
#include "HardwareConfiguration.h"
#include "EnumsAndConstants.h"

#include <esp_now.h>
#include "WiFi.h"

// extern uint8_t broadcastAddress[];

extern "C"
{
    // void networkInit();
};

class Telemetry
{
public:
    Telemetry();
    void Setup();
    void Send();

private:
    uint8_t broadcastAddress[ADDRESS_LENGTH];
    esp_now_peer_info_t peerInfo;
    bool networkOk = false;
    struct_message myData;
};

#endif