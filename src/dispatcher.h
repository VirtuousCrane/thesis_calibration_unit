#include "network/wifi_request.h";
#include "network/mqtt_request.h";

enum DispatcherMode {
    DISPATCHER_MODE_POST,
    DISPATCHER_MODE_MQTT
};

class RequestDispatcher {
    private:
        enum DispatcherMode mode;
        String host;
        String mqtt_topic;
        int mqtt_port;
    
    public:
        RequestDispatcher();
        RequestDispatcher(DispatcherMode dispatcher_mode);
        RequestDispatcher(String host_addr, DispatcherMode dispatcher_mode);
        RequestDispatcher(String host_addr, String topic, DispatcherMode dispatcher_mode);
        RequestDispatcher(String host_addr, int port, DispatcherMode dispatcher_mode);

        int init();
        void setMode(DispatcherMode dispatcher_mode);
        void setTopic(String topic);
        void send_message(String message);
};