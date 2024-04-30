#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/pulse_cnt.h"
#define DATA_PROCESSING_H
#include "data_processing.h"
#define SYS_PARA_H
#include "sysPara.h"


#define DATA_PROCESSING_TAG "DATA_PROCESSING"

void pulseCounterInit(void)
{
    /* Configuration of a new unit */
    pcnt_unit_config_t unit_config = {
        .high_limit = PCNT_UPPER_LIMIT,
        .low_limit = -1000,
    };
    pcnt_new_unit(&unit_config,&pcnt_unit);

    /* Configuration of a channel within a unit */
    pcnt_chan_config_t chan_config = {
        .edge_gpio_num = PCNT_PIN_1,
        .level_gpio_num = -1,
    };
    pcnt_channel_handle_t pcnt_chan = NULL;
    pcnt_new_channel(pcnt_unit,&chan_config,&pcnt_chan);

    /* Configuring the behaviour of the channel on rising and falling edges. 
    Below statement can be rotated depending on sensor output*/
    pcnt_channel_set_edge_action(pcnt_chan,PCNT_CHANNEL_EDGE_ACTION_INCREASE,PCNT_CHANNEL_EDGE_ACTION_HOLD);
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = PCNT_INPUT_FILTER_NS,
    };
    pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config);

    /* Enable and start the counter*/
    pcnt_unit_enable(pcnt_unit);
    pcnt_unit_start(pcnt_unit);
}

void rtc_config(void)
{
    time(&now);
    setenv("TZ", "UTC-05:30", 1);
    tzset();
}

void calculateRPM(void)
{
    int pulse_count = 0;
    pcnt_unit_get_count(pcnt_unit, &pulse_count);
    ESP_LOGI(DATA_PROCESSING_TAG,"%d",pulse_count);
    processPara.rpm = pulse_count;
    

    rpm_for_decision[2] = rpm_for_decision[1];
    rpm_for_decision[1] = rpm_for_decision[0];
    rpm_for_decision[0] = pulse_count;

    pcnt_unit_clear_count(pcnt_unit);

    if(rpm_for_decision[0] == 0 && rpm_for_decision[1] > 0)
    {
        processPara.operational_state = 0;
        localtime_r(&now, &timeinfo);
        strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
        processPara.stops_ts[processPara.no_of_stops] = strftime_buf;
        processPara.no_of_stops++;
        /* Code to send message */
    }

    if(rpm_for_decision[0] > 0 && processPara.operational_state == 0)
    {
        processPara.operational_state = 1;
        localtime_r(&now, &timeinfo);
        strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
        processPara.stops_ts[processPara.no_of_starts] = strftime_buf;
        processPara.no_of_starts++;
        /* Code to send message */
    }

}

