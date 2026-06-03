#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* topic = "qualidade-ar/sensor2";

#define DHT_PIN 4
#define DHT_TYPE DHT22
#define MQ135_PIN 34

DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi conectado!");
  client.setServer(mqtt_server, mqtt_port);
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP32-Sensor2-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("MQTT conectado!");
    } else {
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  int rawAqi = analogRead(MQ135_PIN);
  int aqi = map(rawAqi, 0, 4095, 0, 150);

  StaticJsonDocument<200> doc;
  doc["sensor"] = "sensor2";
  doc["temperatura"] = temperatura;
  doc["umidade"] = umidade;
  doc["aqi"] = aqi;

  char payload[200];
  serializeJson(doc, payload);
  client.publish(topic, payload);

  Serial.println(payload);
  delay(10000);
}
