#define HTTP_FILE
#include "http_file.h"
#include <sys/param.h>
#include <string.h>
#include "sysPara.h"
#include "data_processing.h"
#include "wifi_con.h"
#include "esp_log.h"

#define HTTP_TAG "HTTP"


esp_err_t client_event_post_handler(esp_http_client_event_handle_t evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        // printf("HTTP_EVENT_ON_DATA: %.*s\n", evt->data_len, (char *)evt->data);
        break;
    default:
        break;
    }
    return ESP_OK;
}

void http_client_init(char* cert) 
{
    esp_http_client_config_t config_client = {
        .url = "https://havurkbo22.execute-api.ap-south-1.amazonaws.com/Dep2",
        .method = HTTP_METHOD_POST,
        .cert_pem = cert,
        .event_handler = client_event_post_handler,
    };
    client = esp_http_client_init(&config_client);
    esp_http_client_set_header(client,"Content-Type","application/json");
    ESP_LOGI(HTTP_TAG,"Client Init\n");

}

void generateJsonData(void)
{
    ESP_LOGI(HTTP_TAG,"Processing JSON data");
    cJSON *root, *stop_ts, *start_ts;
    root = cJSON_CreateObject();
    // rpm_array = cJSON_CreateArray();
    stop_ts = cJSON_CreateArray();
    start_ts = cJSON_CreateArray();

    uint8_t i; 
    cJSON_AddNumberToObject(root,"factory_id",sysPara.factory_id);
    cJSON_AddNumberToObject(root,"node_id",sysPara.node_id);
    /* Use this code if we want to transmit an array of rpms
        cJSON_AddItemToObject(root,"RPM",rpm_array);

    for(i=0; i<=4; i++)
    {
        cJSON_AddItemToArray(rpm_array,cJSON_CreateNumber(processPara.rpm[i]));
    }
    */
    cJSON_AddNumberToObject(root,"rpm",processPara.rpm);
    cJSON_AddNumberToObject(root,"no_of_stops",processPara.no_of_stops);
    cJSON_AddNumberToObject(root,"no_of_starts",processPara.no_of_starts);

    cJSON_AddItemToObject(root,"Stop_timestamps",stop_ts);
     for(i=0; i< processPara.no_of_stops; i++)
    {
        cJSON_AddItemToArray(stop_ts,cJSON_CreateString(processPara.stops_ts[i]));
    }
    cJSON_AddItemToObject(root,"Start_timestamps",start_ts);
    for(i=0; i< processPara.no_of_starts; i++)
    {
        cJSON_AddItemToArray(start_ts,cJSON_CreateString(processPara.start_ts[i]));
    }
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    cJSON_AddStringToObject(root,"current_time",strftime_buf);
    json_data_output = cJSON_Print(root);
    printf("%s\n",json_data_output);
}

void transmitJsonData(char* data)
{
    if(wifi_connected == 1)
    {
        ESP_LOGI(HTTP_TAG,"Transmitting JSON");
        esp_http_client_set_post_field(client,data,strlen(data));
        esp_http_client_perform(client);
        // esp_http_client_cleanup(client);
    }
    else if (wifi_connected == 0)
    {
        /* @todo EEPROM Logic */
    }
    
}