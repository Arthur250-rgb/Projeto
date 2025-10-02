#include <WiFi.h>
#include <PubSubClient.h>

const String SSID = "iPhone";
const String PSWD = "iot_sul_123";

const String brokenUrl = "test.mosquitto.org";
const int port = 1883;

WiFiClient espClient;                          
PubSubClient mqttClient(espClient);            

void connectToWiFi();
void connectToBroker();

void setup() {
  Serial.begin(115200);       
  connectToWiFi();
  mqttClient.setServer(brokenUrl.c_str(), port);
  connectToBroker(); 
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Conexão Wi-Fi perdida! Tentando reconectar...");
    connectToWiFi();
  }

  if (!mqttClient.connected()) {
    Serial.println("Conexão com broker perdida! Tentando reconectar...");
    connectToBroker();
  }
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    msg.trim();
    if (msg.length() > 0) {
      mqttClient.publish("AulaIoTSul/Chat", msg.c_str());
      Serial.println("Mensagem enviada: " + msg);
    }
  }

  mqttClient.loop();
}

void connectToBroker() {
  Serial.println("Conectando ao broker");   
  String userId = "ESP-Arthur";
  userId += String(random(0xffff), HEX);

  while (!mqttClient.connected()) {
    mqttClient.connect(userId.c_str());
    Serial.print(".");
    delay(2000);
  }
  mqttClient.subscribe("AulaIoTSul/Chat");
  mqttClient.setCallback(callback);
  Serial.println("Conectado com sucesso!");
}

void callback(char* topic, byte* payload, unsigned long length) {
  String resposta = "";
  for (int i = 0; i < length; i++) {
    resposta += (char)payload[i];
  }
  Serial.println(resposta);
}

void connectToWiFi() {
  Serial.print("Iniciando conexão com a rede Wi-Fi: ");
  Serial.println(SSID);

  WiFi.begin(SSID.c_str(), PSWD.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nWi-Fi conectado com sucesso!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}