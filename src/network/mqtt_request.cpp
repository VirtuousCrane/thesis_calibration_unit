#include "mqtt_request.h"

/* MQTT Client Config */
WiFiClient wifi_client;
MqttClient mqtt_client(wifi_client);

/// Initializes MQTT Connection
int mqtt_connect(const char *broker, int port) {
    int status = mqtt_client.connect(broker, port);
    if (!status) {
        Serial.println("Failed to connect to MQTT Broker");
    }

    return status;
}

/// Sends an MQTT POLL message (heartbeat)
void mqtt_poll() {
    mqtt_client.poll();
}

/// Sends a message to mqtt
void mqtt_send(String topic, String message) {
    mqtt_client.beginMessage(topic);
    mqtt_client.print(message);
    mqtt_client.endMessage();
}