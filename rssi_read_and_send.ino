/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/
#define SERVICE_UUID "422da7fb-7d15-425e-a65f-e0dbcc6f4c6a"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include <WiFi.h>
#include <HTTPClient.h>

#include <vector>

/* WiFi Config */
const char* ssid = "Shoukaku";
const char* password = "00000000";
const char* serverName = "http://192.168.70.23:8000/post_json";

/* BLE Config */
int scanTime = 5; //In seconds
BLEScan* pBLEScan;

/* Internal HTTP Response Status */
enum ResponseStatus {
  Success,
  Failure,
  WiFiFailure,
  None
};

/* Internal HTTP Response Class */
class RequestResponse {

  private:
    /* Class Private Fields */
    enum ResponseStatus rStatus;
    String rData;

    /* Function Declarations */
    /// Sets the internal request status of the RequestResponse Object
    void setStatus(ResponseStatus status) {
      rStatus = status;
    }

  public:
    RequestResponse() {
      rStatus = None;
      rData = "";
    }

    RequestResponse(ResponseStatus status, String data) {
      rStatus = status;
      rData = data;
    }

    RequestResponse(ResponseStatus status) {
      rStatus = status;
      rData = "";
    }

    /// Checks if the Request was made successfully
    bool isSuccess() {
      if (rStatus == Success) {
        return true;
      }
      return false;
    }

    /// Gets the Status of the Request
    ResponseStatus getStatus() {
      return rStatus;
    }

    /// Sets the Response data
    void setData(String d) {
      rData = d;
    }

    /// Gets the Response data
    String getData() {
      return rData;
    }

};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  /* BLE Init */
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  // pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value

  /* WiFi Init */
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wifi!");
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  printBLEResult(foundDevices);
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  post("placeholder");
  delay(2000);
}

/// Sends a post request
RequestResponse post(String request_data) {
  Serial.println("Initiating POST Request");
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi Not Connected");
    return RequestResponse(WiFiFailure);
  }

  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);

  http.addHeader("Content-Type", "application/json");
  // String httpRequestData = "p1=test";
  String httpRequestData = "{\"p1\":\"test\"}";
  int httpResponseCode = http.POST(httpRequestData);
  http.end();

  if (httpResponseCode >= 200 && httpResponseCode < 300) {
    return RequestResponse(Success);
  }
  return RequestResponse(Failure);

}

/// Given a BLEScanResults object, print out all discovered BLE Devices
void printBLEResult(BLEScanResults &bleResults) {
  int resultCount = bleResults.getCount();

  Serial.println("============================");

  for (int i = 0; i < resultCount; i++) {
    BLEAdvertisedDevice device = bleResults.getDevice(i);
    Serial.printf("Address: %s\tRSSI: %i\n", device.getAddress().toString().c_str(), device.getRSSI());
  }

  Serial.println("============================");
}

/// Checks if our device has the correct Service UUID.
/// The 'Correct' Service UUID is defined by the #define SERVICE_UUID clause at the top of the file
bool isPartOfNetwork(BLEAdvertisedDevice &device) {
  if (device.getServiceUUID().equals(BLEUUID(SERVICE_UUID))) {
    return true;
  }
  return false;
}
