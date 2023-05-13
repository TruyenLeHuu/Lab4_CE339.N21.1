#ifndef _SYSCONFIG__H
#define _SYSCONFIG__H

/**
 * Debugger?
 */
#define DEBUG 1

/**
 * GPIOs defs
 */
#define WIFI_SSID      "1111"
#define WIFI_PASS      "01245678"

/**
 * Mqtt config
 */
#define MQTT_ADDRESS 		"mqtt.eclipseprojects.io"
#define MQTT_PORT 		8883
#define MQTT_URI 		"mqtts://mqtt.eclipseprojects.io:8883"
#define USER_NAME 		"1111"
#define PASSWORD 		"123456"
/**
 * Topic
 */
#define DATA_TOPIC_PUB 		"/Data/"
#define CONNECT_TOPIC_PUB_0 		"/Status/Connected"
#define CONNECT_TOPIC_PUB_1 		"/Status/Connected1"
#define DISCONNECT_TOPIC_PUB 		"/Status/Disconnected"
#define CONTROL_TOPIC_SUB 		"/Control/"

/**
 * Globals defs
 */
#define TRUE  1
#define FALSE 0

#endif 