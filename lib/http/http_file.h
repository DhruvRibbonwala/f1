#include "esp_http_client.h"
#include "cJSON.h"

typedef union
{
  struct
  {
    uint8_t mcuError : 1;        //bit 0
    uint8_t i2cError : 1;        //bit 1
    uint8_t canError : 1;        //bit 2
    uint8_t configError : 1;     //bit 3
    uint8_t overflow : 1;        //bit 4
    uint8_t uartError : 1;       //bit 5
    uint8_t wifiError : 1;       //bit 6
    uint8_t rsvd : 1;            //bit 7
  }bits;
  uint8_t byte;
}fault_t;


#ifdef HTTP_FILE
    void parseJsonData(char* data);
    void generateJsonData();
    void transmitJsonData(char* data);
    esp_http_client_handle_t client;
    void http_client_init(char* cert);
    char* json_data_output;
#else
    extern void parseJsonData(char* data);
    extern void generateJsonData();
    extern void transmitJsonData(char* data);
    extern esp_http_client_handle_t client;
    extern void http_client_init(char* cert);
    extern char* json_data_output;
#endif