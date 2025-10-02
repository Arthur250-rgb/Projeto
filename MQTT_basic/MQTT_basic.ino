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
    mqttClient.publish("AulaIoTSul/Chat", "oi-Arthur");
    delay(1000);

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

  Serial.println("\nWi-Fi conectado com sucesso!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void connectToBroker() {
  Serial.println("Conectando ao broker");   

  String userId = "ESP-Arthur";
  userId += String(random(0xffff), HEX);

  while (!mqttClient.connected()) {
    if (mqttClient.connect(userId.c_str())) {
      Serial.println("Conectado com sucesso ao broker!");
    } else {
      Serial.println("Erro de conexão ao broker... tentando novamente em 3s");
      delay(3000);
    }
  }
}
