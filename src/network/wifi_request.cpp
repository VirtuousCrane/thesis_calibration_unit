#include "wifi_request.h"

/* RequestResponse Class Definition */
RequestResponse::RequestResponse() {
    rStatus = RESPONSE_NONE;
    rData = "";
}

RequestResponse::RequestResponse(ResponseStatus status, String data) {
    rStatus = status;
    rData = data;
}

RequestResponse::RequestResponse(ResponseStatus status) {
    rStatus = status;
    rData = "";
}

/// Sets the response status
void RequestResponse::setStatus(ResponseStatus status) {
    rStatus = status;
}

/// Checks if the request was made successfully
bool RequestResponse::isSuccess() const {
    if (rStatus == RESPONSE_SUCCESS) {
        return true;
    }
    
    return false;
}

/// Gets the response status of the request
ResponseStatus RequestResponse::getStatus() const {
    return rStatus;
}

/// Sets the Response Data
void RequestResponse::setData(String d) {
    rData = d;
}

/// Gets the Response Data
String RequestResponse::getData() const {
    return rData;
}

/* HTTP Requests */

/// Sends a POST request
RequestResponse wifi_request_post(String server_name, String request_data) {
    Serial.println("Initiating POST Request...");

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi Not Connected");
        return RequestResponse(RESPONSE_WIFI_FAILURE);
    }
    
    WiFiClient client;
    HTTPClient http;
    
    int httpResponseCode;

    // Constructing the HTTP Request
    http.begin(client, server_name);
    http.addHeader("Content-Type", "application/json");
    httpResponseCode = http.POST(request_data);
    http.end();
        
    if (httpResponseCode >= 200 && httpResponseCode < 300) {
        return RequestResponse(RESPONSE_SUCCESS);
    }
    return RequestResponse(RESPONSE_FAILURE);
}