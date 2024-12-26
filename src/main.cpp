#include <iostream>

#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_http_server.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "variables.hpp"

//#define WIFI_SSID "wlan name"
//#define WIFI_PASS "wlan passwort"

// HTTP GET Handler
esp_err_t hello_get_handler(httpd_req_t *req) {
    const char* resp_str = "Deine Mutter!";
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// HTTP Server initialisieren
httpd_handle_t start_webserver() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Ändere den Port für den Webserver (falls noch nicht angepasst)
    config.server_port = 8080;

    httpd_handle_t server = NULL;
    esp_err_t err = httpd_start(&server, &config);

    if (err == ESP_OK) {
        ESP_LOGI("HTTP_SERVER", "Webserver erfolgreich gestartet auf Port %d", config.server_port);

        httpd_uri_t hello = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = hello_get_handler,
            .user_ctx = NULL
        };
        esp_err_t reg_err = httpd_register_uri_handler(server, &hello);

        if (reg_err == ESP_OK) {
            ESP_LOGI("HTTP_SERVER", "URI-Handler erfolgreich registriert.");
        } else {
            ESP_LOGE("HTTP_SERVER", "Fehler beim Registrieren des URI-Handlers: %s", esp_err_to_name(reg_err));
        }

    } else {
        ESP_LOGE("HTTP_SERVER", "Fehler beim Starten des Webservers: %s", esp_err_to_name(err));
    }

    return server;
}


extern "C" void app_main() {
    // NVS initialisieren
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    // WiFi initialisieren
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t wifi_config = {};
    strncpy((char*)wifi_config.sta.ssid, WIFI_SSID, sizeof(wifi_config.sta.ssid));
    strncpy((char*)wifi_config.sta.password, WIFI_PASS, sizeof(wifi_config.sta.password));

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(static_cast<wifi_interface_t>(ESP_IF_WIFI_STA), &wifi_config);
    esp_wifi_start();
    esp_wifi_connect();

    // Warten, bis eine IP-Adresse zugewiesen wurde
    vTaskDelay(5000 / portTICK_PERIOD_MS);

    // IP-Adresse abrufen und ausgeben
    esp_netif_ip_info_t ip_info;
    esp_netif_t* netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (netif) {
        esp_netif_get_ip_info(netif, &ip_info);
        ESP_LOGI("IP_INFO", "ESP32 IP-Adresse: " IPSTR, IP2STR(&ip_info.ip));
    } else {
        ESP_LOGE("IP_INFO", "Netif handle nicht gefunden.");
    }

    // Webserver starten
    start_webserver();

    while (true) {
        vTaskDelay(10000 / portTICK_PERIOD_MS); // 10 Sekunden warten
    }
}
