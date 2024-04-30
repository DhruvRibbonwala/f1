#include "esp_wifi.h"

#ifdef WIFI_CON_H
    void connect_to_wifi(void);
    uint8_t wifi_connected, wifi_reconnect_counter, wifi_reconnect_limit;
#else
    extern void connect_to_wifi(void);
    extern uint8_t wifi_connected, wifi_reconnect_counter, wifi_reconnect_limit;
#endif