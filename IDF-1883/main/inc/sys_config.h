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
#define MQTT_ADDRESS 		"192.168.137.1"
#define MQTT_PORT 		1883
#define MQTT_URI 		"mqtt://mqtt.eclipseprojects.io"
#define USER_NAME 		"1111"
#define PASSWORD 		"123456"
/**
 * Topic
 */
#define DATA_TOPIC_PUB 		"/Data/"
#define CONNECT_TOPIC_PUB 		"/Status/Connected"
#define DISCONNECT_TOPIC_PUB 		"/Status/Disconnected"
#define CONTROL_TOPIC_SUB 		"/Control/"

/**
 * Globals defs
 */
#define TRUE  1
#define FALSE 0

#endif 