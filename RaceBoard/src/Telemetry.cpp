#include "Telemetry.h"

Telemetry::Telemetry()
{
    BROADCAT_ADDRESS
    memcpy(broadcastAddress, THE_ADDRESS, ADDRESS_LENGTH);
}

void Telemetry::Setup()
{ /*
     WiFi.mode(WIFI_MODE_STA);

     WiFi.disconnect();
     ESP_ERROR_CHECK(esp_wifi_set_channel(WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE));
     if (esp_now_init() != ESP_OK)
     {
         ESP.restart();
         return;
     }

     Serial.print("* MAC Address: ");
     Serial.println(WiFi.macAddress());
     // Initialize ESP-NOW
     if (esp_now_init() != ESP_OK)
     {
         Serial.println("* Error initializing ESP-NOW");
         return;
     }
     // Once ESPNow is successfully Init, we will register for Send CB to
     // get the status of Transmitted packet
     // esp_now_register_send_cb(OnDataSent);

     // Register peer
     memcpy(peerInfo.peer_addr, broadcastAddress, 6);
     peerInfo.channel = WIFI_CHANNEL;
     peerInfo.encrypt = false;

     // Add peer
     if (esp_now_add_peer(&peerInfo) != ESP_OK)
     {
         Serial.println("* Failed to add peer");
         return;
     }
     networkOk = true;
     Serial.println("* Network ready");
     */
}

void Telemetry::Send(char txbuff[TX_COMMAND_BUFLEN])
{ /*
     if (!networkOk)
         return;
     strcpy(myData.message, txbuff);
     esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
     if (result != ESP_OK)
     {
         Serial.println("* Error sending the data");
     }
     */
}

/*
BROADCAT_ADDRESS

esp_now_peer_info_t peerInfo;
bool networkOk = false;
*/
// Create a struct_message called myData
// struct_message myData;

// callback when data is sent
/*void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}*/

/*
void networkInit()
{
    WiFi.mode(WIFI_MODE_STA);
    Serial.print("* MAC Address: ");
    Serial.println(WiFi.macAddress());
    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("* Error initializing ESP-NOW");
        return;
    }
    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Transmitted packet
    // esp_now_register_send_cb(OnDataSent);

    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("* Failed to add peer");
        return;
    }
    networkOk = true;
    Serial.println("* Network ready");
}
*/

/*
void networkSend()
{
    if (!networkOk)
        return;
    // Set values to send
    strcpy(myData.a, "THIS IS A CHAR");
    myData.b = random(1, 20);
    myData.c = 1.2;
    myData.d = false;

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

    if (result == ESP_OK)
    {
        Serial.println("Sent with success");
    }
    else
    {
        Serial.println("Error sending the data");
    }
}
*/