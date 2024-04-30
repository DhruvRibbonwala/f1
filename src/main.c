#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/task.h"
#include "driver/pulse_cnt.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "data_processing.h"
#include "wifi_con.h"
#include "http_file.h"

// extern const uint8_t ClientCert_pem_start[] asm("_binary_AmazonRSA2048M02_pem_start");
extern const uint8_t ClientCert_pem_start[] asm("_binary_AmazonRSA2048M01_pem_start");

void dataProcessingTask(void *arg)
{
    while(1)
    {
        calculateRPM();
        vTaskDelay(pdMS_TO_TICKS(5000));
    }

}

void dataTransmitTask(void *arg)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10000));
        generateJsonData();
        transmitJsonData(json_data_output);
    }
}

void app_main() {
    nvs_flash_init();
    connect_to_wifi();
    http_client_init((char *)ClientCert_pem_start);
    pulseCounterInit();
    xTaskCreate(dataProcessingTask,"Data_processing_task",4096,NULL,10,NULL);
    xTaskCreate(dataTransmitTask,"Data_Transmitting_task",4096,NULL,10,NULL);
}