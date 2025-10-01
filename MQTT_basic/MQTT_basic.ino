#include <WiFi.h>
#include <PubSubClient.h>

const String SSID = "iPhone";
const String PSWD = "iot_sul_123";

const String brokenUrl = "test.mosquitto.org"; // URL do broker
const int port = 1883;                         // Porta do broker

WiFiClient espClient;                          
PubSubClient mqttClient(espClient);            

void connectToWiFi();

void setup() {
  Serial.begin(115200);       
  connectToWiFi();

  // Garante que está conectado antes de seguir
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

  Serial.println("Conectando ao broker");   
  mqttClient.setServer(brokenUrl.c_str(), port);

  // Conecta com o ID diretamente como string literal
  mqttClient.connect("ESP-Arthur");  

  while (!mqttClient.connected()) {
    Serial.println("Erro de conexão ao broker...");
    delay(500);
  }

  Serial.println(" Conectado com sucesso ao broker!");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(" Conexão Wi-Fi perdida! Tentando reconectar...");
    connectToWiFi();
  }

  mqttClient.loop();
}

void scanLocalNetworks() {
  Serial.println("Iniciando Scan de Rede Wi-Fi...");
  int number = WiFi.scanNetworks();
  delay(500);

  if (number == -1) {
    Serial.println("ERRO ao escanear!");
  } else {
    for (int net = 0; net < number; net++) {
      Serial.printf("%d - %s | %d dB\n", net, WiFi.SSID(net).c_str(), WiFi.RSSI(net));
    }
  }
}

void connectToWiFi() {
  Serial.print("Iniciando conexão com a rede Wi-Fi: ");
  Serial.println(SSID);

  WiFi.begin(SSID.c_str(), PSWD.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println("\n Wi-Fi conectado com sucesso!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}