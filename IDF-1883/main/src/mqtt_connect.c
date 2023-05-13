#include <stdio.h>

#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "mqtt_connect.h"
/**
 * System Config;
 */
#include "sys_config.h"


static const char *TAG = "mqtt connection";
esp_mqtt_client_handle_t client = NULL;
extern const uint8_t certificate_pem_start[] asm("_binary_certificate_pem_start");
extern const uint8_t certificate_pem_end[] asm("_binary_certificate_pem_end");

void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}
void mqtt_receive_control(void * data)
{
    printf("TOPIC = %s\r\n", CONTROL_TOPIC_SUB);
    printf("DATA = %s\n",   (char*)data);
    vTaskDelay(100 / portTICK_PERIOD_MS); 
    vTaskDelete(0);
}
void mqtt_receive_unknown(void * data)
{
    printf("TOPIC = %s\r\n", "Unknown");
    printf("DATA = %s\n",  (char*)data);
    vTaskDelay(100 / portTICK_PERIOD_MS); 
    vTaskDelete(0);
}
void create_task_mqtt ( int topic_len, char* _topic, int data_len, char* _data )
{   
    char topic[50];
    char data[500];
    snprintf(topic, 100, "%.*s", topic_len, _topic);
    snprintf(data, 500, "%.*s", data_len, _data);
    if(strcmp(topic, CONTROL_TOPIC_SUB) == 0)
    {
        if( xTaskCreate( mqtt_receive_control, "mqtt_receive_control", 1024 * 2, data, 2, NULL) != pdPASS )
        {
            #if DEBUG
            ESP_LOGI( TAG, "ERROR - mqtt_receive_unknown NOT ALLOCATED :/\r\n" );  
            #endif
            return;   
        }    
    }
    else 
    {
        if( xTaskCreate( mqtt_receive_unknown, "mqtt_receive_unknown", 1024 * 2, data, 2, NULL) != pdPASS )
        {
            #if DEBUG
            ESP_LOGI( TAG, "ERROR - mqtt_receive_unknown NOT ALLOCATED :/\r\n" );  
            #endif
            return;   
        } 
    }
}
/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_subscribe(client, CONTROL_TOPIC_SUB, 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        mqtt_client_publish(CONNECT_TOPIC_PUB, "Esp32");
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        create_task_mqtt(event->topic_len, event->topic, event->data_len, event->data);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}
bool mqtt_client_publish(char* topic, char *publish_string)
{
    if (client) {
        int msg_id = esp_mqtt_client_publish(client, topic, publish_string, 0, 1, 0);
        ESP_LOGI(TAG, "sent publish returned msg_id=%d", msg_id);
        return true;
    }
    return false;
}
void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
            .uri = MQTT_URI,
            .port = MQTT_PORT,
            .lwt_topic = DISCONNECT_TOPIC_PUB,
            .lwt_msg = "Esp32",
            .lwt_msg_len = 0,
            .lwt_qos = 1,
            .lwt_retain = 0,
            .username = USER_NAME,
            .password = PASSWORD,
    };
    client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}


