#include <WiFi.h>

const String SSID = "iPhone Uber";
const String PSWD = "uber2307";

void scanLocalNetworks();
void conectarWiFi();

void setup() {
  Serial.begin(115200);

  scanLocalNetworks();        
  conectarWiFi();           
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(" Conexão perdida! Tentando reconectar...");
    conectarWiFi();
  }

  delay(3000); 
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

void conectarWiFi() {
  Serial.print("Iniciando conexão com a rede Wi-Fi: ");
  Serial.println(SSID);

  WiFi.begin(SSID.c_str(), PSWD.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println("\n Conectado com sucesso!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

