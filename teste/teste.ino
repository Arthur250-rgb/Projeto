#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando Scan de Rede Wi-Fi");
}

void loop() {
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
