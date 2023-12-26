#include <WiFi.h>
#include <ArduinoMqttClient.h>

/// Initializes MQTT Connection
int mqtt_connect(const char *broker, int port);

/// Sends an MQTT POLL message (heartbeat)
void mqtt_poll();

/// Sends a message to mqtt
void mqtt_send(String topic, String message);

