#include "dispatcher.h"

RequestDispatcher::RequestDispatcher() {
    mode = DISPATCHER_MODE_POST;
    host = "localhost";
    mqtt_topic = "LOLI";
    mqtt_port = 1883;
}

RequestDispatcher::RequestDispatcher(DispatcherMode dispatcher_mode) {
    mode = dispatcher_mode;
    host = "localhost";
    mqtt_topic = "LOLI";
    mqtt_port = 1883;
}

RequestDispatcher::RequestDispatcher(String host_addr, DispatcherMode dispatcher_mode) {
    mode = dispatcher_mode;
    host = host_addr;
    mqtt_topic = "LOLI";
    mqtt_port = 1883;
}

RequestDispatcher::RequestDispatcher(String host_addr, String topic, DispatcherMode dispatcher_mode) {
    mode = dispatcher_mode;
    host = host_addr;
    mqtt_topic = topic;
    mqtt_port = 1883;
}

RequestDispatcher::RequestDispatcher(String host_addr, int port, DispatcherMode dispatcher_mode) {
    mode = dispatcher_mode;
    host = host_addr;
    mqtt_topic = "LOLI";
    mqtt_port = port;
}

int RequestDispatcher::init() {
    switch (mode) {
        case DISPATCHER_MODE_MQTT:
            return mqtt_connect(host.c_str(), mqtt_port);
        default:
            return 1;
    }
}

void RequestDispatcher::setMode(DispatcherMode dispatcher_mode) {
    mode = dispatcher_mode;
}

void RequestDispatcher::setTopic(String topic) {
    mqtt_topic = topic;
}

void RequestDispatcher::send_message(String message) {
    switch (mode) {
        case DISPATCHER_MODE_POST:
            wifi_request_post(host, message);
            break;
        case DISPATCHER_MODE_MQTT:
            mqtt_send(mqtt_topic, message);
            break;
        default:
            break;
    }
}