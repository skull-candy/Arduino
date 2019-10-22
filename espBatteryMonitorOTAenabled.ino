#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <SimpleTimer.h>
ADC_MODE(ADC_VCC);


// Paramètre WiFi, MQTT - WiFi, MQTT parameters
const char* ssid = "Al Ahsan";                // WiFi SSID
const char* password = "a4c100b3bb";        // WiFi Password
const char* mqtt_server = "192.168.2.252";  // IP Broker MQTT
const char* topic_pan = "servo/pan";          // Topic MQTT pour servo Pan - Topic MQTT for Pan servor
const char* topic_tilt = "servo/tilt";        // Topic MQTT pour servo Tilt - Topic MQTT for Tilt servor

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

SimpleTimer tempreading;

void setup() {
  tempreading.setInterval(5000L, temp);
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("BatteryMonitor");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String string;
  // Affiche le topic entrant - display incoming Topic
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  delay(15);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.subscribe(topic_pan);
      client.subscribe(topic_tilt);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  ArduinoOTA.handle();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  tempreading.run();
  delay(100);
}

void temp()
{
  int vcc;
  vcc = ESP.getVcc();
  String internalvcc_str; 
  char internalvccout[50];
  internalvcc_str = String(vcc); //converting ftemp (the float variable above) to a string
  internalvcc_str.toCharArray(internalvccout, internalvcc_str.length() + 1); //packaging up the data to publish to mqtt whoa...
  client.publish("EspVoltage", internalvccout); //money shot




}
