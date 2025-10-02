#include <WiFi.h>
#include <PubSubClient.h>

const char* SSID = "iPhone";
const char* PSWD = "iot_sul_123";

const char* brokerUrl = "test.mosquitto.org";
const int brokerPort = 1883;

const char* meuTopico = "controle/dupla1";
const char* topicoParceira = "controle/dupla2";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

const int ledPin = 22; 

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  WiFi.begin(SSID, PSWD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado: " + String(WiFi.localIP()));

  mqttClient.setServer(brokerUrl, brokerPort);
  mqttClient.setCallback(callback);
  
  connectToBroker();
}

void loop() {
  if (!mqttClient.connected()) connectToBroker();
  mqttClient.loop();

  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();
    if (comando.length() > 0) {
      mqttClient.publish(topicoParceira, comando.c_str());
      Serial.println("Enviado: " + comando);
    }
  }
}

void callback(char* topic, byte* payload, unsigned long length) {
  String comando = "";
  for (unsigned int i = 0; i < length; i++) comando += (char)payload[i];

  Serial.println("Recebido: " + comando);

  if (comando.equalsIgnoreCase("LIGAR")) digitalWrite(ledPin, HIGH);
  else if (comando.equalsIgnoreCase("DESLIGAR")) digitalWrite(ledPin, LOW);
}

void connectToBroker() {
  while (!mqttClient.connected()) {
    String clientId = "ESP32-" + String(random(0xffff), HEX);
    if (mqttClient.connect(clientId.c_str())) {
      mqttClient.subscribe(meuTopico);
      Serial.println("Conectado ao broker e inscrito em: " + String(meuTopico));
    } else {
      delay(2000);
    }
  }
}
