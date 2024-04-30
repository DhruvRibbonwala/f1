#include <stdint.h>
#define SSID "Guest Room"
#define PASS "Rajesh1970*"

typedef struct {
    uint16_t factory_id;
    uint16_t node_id;
    uint8_t no_of_machines;
    char wifi_network[20];
    char wifi_password[20];
}systemParameters_t;

typedef struct {
    uint8_t operational_state;
    uint8_t number_of_readings;
    // uint16_t rpm[5];
    uint16_t rpm;
    uint8_t no_of_stops;
    uint8_t no_of_starts;
    char* stops_ts[10];
    char* start_ts[10];
}processParameters_t;



#ifdef SYS_PARA_H
    systemParameters_t sysPara;
    processParameters_t processPara;
    uint16_t rpm_for_decision[3];
#else
    extern systemParameters_t sysPara;
    extern processParameters_t processPara;
    extern uint16_t rpm_for_decision[3];
#endif