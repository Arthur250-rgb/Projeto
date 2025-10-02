#include <WiFi.h> 
#include <PubSubClient.h>
const String SSID = "iPhone";
const String PSWD = "iot_sul_123";

const String brokerUrl = "test.mosquitto.org";
const int port = 1883;

String message = "";
const byte pin_led = 20;



WiFiClient espClient;
PubSubClient mqttClient(espClient);

void conexaoBroker(){
  Serial.println("Conectando ao broker");
  mqttClient.setServer(brokerUrl.c_str(),port);
  String userId = "ESP-BANANINHA";
  while(!mqttClient.connected()){
    mqttClient.connect(userId.c_str());
    Serial.println(".");
    delay(5000);
  }
  mqttClient.subscribe("BANANAO/Chat");
  mqttClient.setCallback(callback);
  Serial.println("mqtt Connectado com sucesso!");
}

void callback(char* topico, byte* payload, unsigned long length){
  String mensagem = "";
  for(int i=0; i < length; i++){
    mensagem += (char) payload[i];
  }
  Serial.println(mensagem);
  verificaComando(mensagem);
}

void verificaComando(String comando){
  if(comando == "Eduardo e Lipe:on"){
    digitalWrite(pin_led,HIGH);
  }else if(comando == "Eduardo e Lipe:off"){
    digitalWrite(pin_led,LOW);
  }
}

void conexaoWifi() {
  Serial.println("Iniciando conexão com rede Wi-Fi");
  Serial.print("Conectando");

  WiFi.begin(SSID, PSWD);

  int retry_count = 0;
  while (WiFi.status() != WL_CONNECTED && retry_count < 20) {
    Serial.print(".");
    delay(500);
    retry_count++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado");
  } else {
    Serial.println("\nFalha ao conectar");
  }
}



void setup() {
  Serial.begin(115200);
  conexaoWifi();
  conexaoBroker();
  pinMode(pin_led,OUTPUT);
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Conexão Wi-Fi perdida");
    conexaoWifi();
  }
  if(!mqttClient.connected()){
    Serial.println("Conexão Broker perdida");
    conexaoBroker();
  }
  if(Serial.available()){
    message = Serial.readStringUntil('\n');
    message = "Azeitona:" + message;
    mqttClient.publish("BANANAO/Chat",message.c_str());
    delay(1000);
  }

  mqttClient.loop();
}