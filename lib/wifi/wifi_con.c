#define WIFI_CON_H
#include "wifi_con.h"
#include "sysPara.h"
#include "esp_netif.h"
static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch(event_id)
    {
        case WIFI_EVENT_STA_START:
        {
            printf("Wifi connecting ...\n");
            // wifi_reconnect_counter = 0;
            break;
        }
        case WIFI_EVENT_STA_CONNECTED:
        {
            printf("Wifi connected ...\n");
            wifi_connected = 1;
            break;
        }
        /* @todo Think of an interval that would be appropriate to reconnect to wifi*/
        case WIFI_EVENT_STA_DISCONNECTED:
        {   
            printf("Wifi lost connection ...\n");
            wifi_connected = 0;
            esp_wifi_connect();
            vTaskDelay(pdMS_TO_TICKS(60000));
            break;
        }
        case IP_EVENT_STA_GOT_IP:
            printf("Got IP ...\n");
            break;
        default:
            break;
            printf("Default event\n");
    }
}

void connect_to_wifi(void)
{
    /* IP layer initialization */
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation);

    /* WIFI Configuraion Phase */
    esp_event_handler_register(WIFI_EVENT,ESP_EVENT_ANY_ID,wifi_event_handler,NULL);
    esp_event_handler_register(IP_EVENT,IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = SSID,
            .password = PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
            }};

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA,&wifi_configuration);

    /* WIFI Start Phase */
    esp_wifi_start();
    
    /* WIFI Connect Phase */
    esp_wifi_connect();
    vTaskDelay(pdMS_TO_TICKS(5000));
}