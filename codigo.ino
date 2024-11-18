#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define TOPICO_SUBSCRIBE    "/TEF/device022/cmd"
#define TOPICO_PUBLISH      "/TEF/device022/attrs"
#define TOPICO_PUBLISH_LUMI "/TEF/device022/attrs/l"
#define TOPICO_PUBLISH_HUM  "/TEF/device022/attrs/h"
#define TOPICO_PUBLISH_TEMP "/TEF/device022/attrs/t"
#define TOPICO_ALERT        "/TEF/device022/alerts"

#define ID_MQTT  "fiware_022"
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char* SSID = "Wokwi-GUEST";
const char* PASSWORD = "";
const char* BROKER_MQTT = "SEU_IP"; //Coloque o Broker
int BROKER_PORT = 1883;

WiFiClient espClient;
PubSubClient MQTT(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(100);
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
  dht.begin();
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) msg += (char)payload[i];
  if (msg.equals("device022@on|")) digitalWrite(2, HIGH);
  if (msg.equals("device022@off|")) digitalWrite(2, LOW);
}

void VerificaConexoes() {
  if (WiFi.status() != WL_CONNECTED) WiFi.reconnect();
  if (!MQTT.connected()) MQTT.connect(ID_MQTT);
  MQTT.loop();
}

void lerSensoresEEnviar() {
  const int potPin = 34; 
  char msgBuffer[4];
  
  // Luminosidade
  int sensorValue = analogRead(potPin);
  float luminosity = map(sensorValue, 0, 4095, 0, 100);
  dtostrf(luminosity, 4, 1, msgBuffer);
  MQTT.publish(TOPICO_PUBLISH_LUMI, msgBuffer);

  // Exibir valor de luminosidade
  Serial.print("Luminosidade: ");
  Serial.print(luminosity);
  Serial.println("%");

  // Enviar alerta se a luminosidade for alta
  if (luminosity > 80) {
    MQTT.publish(TOPICO_ALERT, "Luminosidade alta!");
    Serial.println("Aviso: Luminosidade alta!");
  }

  // Umidade e Temperatura
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (!isnan(humidity) && !isnan(temperature)) {
    dtostrf(humidity, 4, 1, msgBuffer);
    MQTT.publish(TOPICO_PUBLISH_HUM, msgBuffer);
    
    dtostrf(temperature, 4, 1, msgBuffer);
    MQTT.publish(TOPICO_PUBLISH_TEMP, msgBuffer);

    // Exibir valores de umidade e temperatura
    Serial.print("Umidade: ");
    Serial.print(humidity);
    Serial.print("% Temperatura: ");
    Serial.print(temperature);
    Serial.println("°C");
  } else {
    Serial.println("Falha na leitura do sensor DHT22.");
  }
}

void loop() {
  VerificaConexoes();
  lerSensoresEEnviar();
  delay(3000);
}
