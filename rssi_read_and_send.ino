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
// #include "src/network/wifi_request.h"
// #include "src/network/mqtt_request.h"
#include "src/dispatcher.h"

#include <vector>

/* WiFi Config */
const char* ssid = "Shoukaku";
const char* password = "00000000";
// String serverName = "http://192.168.70.23:8000/post_json";
String serverName = "http://192.168.70.23:8080/api/v1/beacon/calibrate";

/* MQTT Config */
const char broker[] = "test.mosquitto.org";
int port            = 1883;
const char topic[]  = "KRUT/THA";

/* BLE Config */
int scanTime = 5; //In seconds
BLEScan* pBLEScan;

/* Request Dispatcher */
RequestDispatcher request_dispatcher(broker, topic, DISPATCHER_MODE_MQTT);

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

  /* Dispatcher Init */
  while(!request_dispatcher.init()) {
    Serial.println("Failed to connect to MQTT Broker. Retrying...");
    delay(1500);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  printBLEResult(foundDevices);
  Serial.println("Scan done!");
  Serial.println("JSON String: " + getJSONFromBLEResult(foundDevices));
  // wifi_request_post(serverName, getJSONFromBLEResult(foundDevices));
  // mqtt_send(topic, getJSONFromBLEResult(foundDevices));
  request_dispatcher.send_message(getJSONFromBLEResult(foundDevices));
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}

/// Given a BLEScanResults object, Generate a JSON string
String getJSONFromBLEResult(BLEScanResults &bleResults) {
  int total_count = bleResults.getCount();
  bool isFirst = true;
  String JSON_String = "{\"beacons\":[";

  for (int i = 0; i < total_count; i++) {
    BLEAdvertisedDevice device = bleResults.getDevice(i);

    if (!isPartOfNetwork(device)) {
      continue;
    }

    if (!isFirst) {
      JSON_String += ",";
      isFirst = false;
    }

    JSON_String += "{\"macAddress\":\"";
    JSON_String += device.getAddress().toString().c_str();
    JSON_String += "\",";
    JSON_String += "\"rssi\":";
    JSON_String += device.getRSSI();
    JSON_String += "}";
  }
  JSON_String += "]}";

  return JSON_String;
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
