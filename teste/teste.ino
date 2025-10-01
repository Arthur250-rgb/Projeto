#include <WiFi.h>
const String SSID = "iPhone";
const String PSWD = "iot_sul_123";

void scanLocalNetworks();

void setup() {
  Serial.begin(115200); 
  scanLocalNetworks();
  Serial.println("Iniciando conexão com a rede WiFi");
  WiFi.begin(SSID,PSWD);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("\nConectado!");
}

void loop() {
  
}

void scanLocalNetworks(){
  Serial.println("Iniciando Scan de Rede Wi-Fi");
  int number = WiFi.scanNetworks();
  delay(500);
  if (number == -1){
    Serial.println("ERRO!");
  }
  else{
    for(int net = 0; net < number; net++){
      Serial.printf("%d - %s | %d db\n", net ,WiFi.SSID(net), WiFi.RSSI(net));
    }
  }
}
