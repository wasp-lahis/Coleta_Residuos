#include <WiFi.h>
#include <PubSubClient.h> //Biblioteca para as publicações via mqtt

#define WIFISSID "Repyama" 
#define PASSWORD "JACARANDA2005" 
#define TOKEN "BBFF-4yJTjTtuGtL9OYPIK3YcsWbALOBy10" // Ubidots TOKEN
#define VARIABLE_LABEL_DISTANCE "distance"    // Label referente a variável de temperatura criada no ubidots
#define DEVICE_ID "5e77d5851d847261eb896058" // ID do dispositivo (Device id, também chamado de client name)
#define SERVER "things.ubidots.com" // Servidor do Ubidots (broker)

//Porta padrão
#define PORT 1883

//Tópico aonde serão feitos os publish, "esp32-dht" é o DEVICE_LABEL
#define TOPIC "/v1.6/devices/sonar_sensor"

//Objeto WiFiClient usado para a conexão wifi
WiFiClient ubidots;
//Objeto PubSubClient usado para publish–subscribe
PubSubClient heltechClient(ubidots);


void reconnect(){  
  //Loop até que o MQTT esteja conectado
  while (!heltechClient.connected()){
    
    Serial.println("Attempting MQTT connection...");
    
    //Tenta conectar
    if (heltechClient.connect(DEVICE_ID, TOKEN,"")) 
      Serial.println("connected");
    else{
      Serial.print("failed, rc=");
      Serial.print(heltechClient.state());
      Serial.println(" try again in 2 seconds");
      //Aguarda 2 segundos antes de retomar
      delay(2000);
    }
  }
}

bool mqttInit(){
  //Inicia WiFi com o SSID e a senha
  WiFi.begin(WIFISSID, PASSWORD);
 
  //Loop até que o WiFi esteja conectado
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Establishing connection to WiFi..");
  }
 
  //Exibe no monitor serial
  Serial.println("Connected to network");

  //Seta servidor com o broker e a porta
  heltechClient.setServer(SERVER, PORT);
  
  //Conecta no ubidots com o Device id e o token, o password é informado como vazio
  while(!heltechClient.connect(DEVICE_ID, TOKEN, "")){
      Serial.println("MQTT - Connect error");
      return false;
  }

  Serial.println("MQTT - Connect ok");
  return true;
}

//Envia valores por mqtt
//Exemplo: {"temperature":{"value":24.50, "context":{"temperature":24.50, "humidity":57.20}}}
bool sendValues(float distance)
{
  char json[250];
 
  // Atribui para a cadeia de caracteres "json" os valores referentes a disntacia e envia para a variável do ubidots correspondente
  sprintf(json,  "{\"%s\":{\"value\":%02.02f, \"context\":{\"temperature\":%02.02f}}}", VARIABLE_LABEL_DISTANCE, distance, distance);  

  if(!heltechClient.publish(TOPIC, json))
    return false;

  // Se tudo der certo retorna true
  return true;
}
