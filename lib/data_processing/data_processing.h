#define PCNT_PIN_1 19
#define PCNT_UPPER_LIMIT 30000
#define PCNT_INPUT_FILTER_NS 10000

#include "driver/pulse_cnt.h"
#include <time.h>

#ifdef DATA_PROCESSING_H
    time_t now;
    char strftime_buf[64];
    struct tm timeinfo;
    void rtc_config(void);
    void pulseCounterInit(void);
    pcnt_unit_handle_t pcnt_unit = NULL;
    void calculateRPM(void);
#else
    extern time_t now;
    extern char strftime_buf[64];
    extern struct tm timeinfo;
    extern void rtc_config(void);
    extern void pulseCounterInit(void);
    extern pcnt_unit_handle_t pcnt_unit;
    extern void calculateRPM(void);
#endif