#define LWIP_COMPAT_SOCKETS 0
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_heap_caps.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "websetup.h"
#include "port.h"

#include "default_wifi_config.h"

extern "C" {
    void app_main();
}

void connected();

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        printf("SYSTEM_EVENT_STA_START\n");
        ESP_ERROR_CHECK(esp_wifi_connect());
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        printf("SYSTEM_EVENT_STA_GOT_IP\n");
        printf("got ip: %s",
            ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
        static bool alreadyConnected = false;
        if (!alreadyConnected) {
            alreadyConnected = true;
            startThread(connected, "main", 5000);
            // startThread(bench_tcp, "bench", 5000);
        }
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        printf("SYSTEM_EVENT_STA_DISCONNECTED\n");
        ESP_ERROR_CHECK(esp_wifi_connect());
        break;
    default:
        break;
    }
    return ESP_OK;
}

void wifiConnect() {
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config {};
    strcpy((char*)wifi_config.sta.ssid, DEFAULT_SSID);
    strcpy((char*)wifi_config.sta.password, DEFAULT_PASS);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void connected() {
    WebSetup::start("", "ESP32");
    printf("IP: %s\n", WebSetup::nodeAddress.str().c_str());
    printf("websetup secret: %s\n", WebSetup::sharedSecret.c_str());

    while (true) {
        printf("free memory: %d\n", (int)esp_get_free_heap_size());
        vTaskDelay(4000 / portTICK_PERIOD_MS);
    }
}

extern "C" void app_main() {
    ESP_ERROR_CHECK(nvs_flash_init());
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

    wifiConnect();
}
