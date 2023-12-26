#include <WiFi.h>
#include <HTTPClient.h>

/* Response Status */
enum ResponseStatus {
    RESPONSE_SUCCESS,
    RESPONSE_FAILURE,
    RESPONSE_WIFI_FAILURE,
    RESPONSE_NONE
};

/* HTTP Response Class */
class RequestResponse {
    private:
        enum ResponseStatus rStatus;
        String rData;

        void setStatus(ResponseStatus status);
    
    public:
        RequestResponse();
        RequestResponse(ResponseStatus status, String data);
        RequestResponse(ResponseStatus status);
        
        bool isSuccess() const;
        ResponseStatus getStatus() const;
        
        void setData(String d);
        String getData() const;
};

/// Sends a POST Request
RequestResponse wifi_request_post(String server_name, String request_data);
