#include <ESP8266WiFi.h>
#include "DHT.h"
#include "ESP8266ZabbixSender.h"

/**
*  Tipo do sensor DHT a ser usado:
*  DHT11 - DHT 11
*  DHT21 - DHT 21 (AM2301)
*  DHT22 - DHT 22  (AM2302), AM2321
*/

#define DHTTYPE DHT11 //Define o modelo do DHT
uint8_t DHTPin = D7; // Pino do Sensor
DHT dht(DHTPin, DHTTYPE); // Inicializa o sensor

/* Parametros Zabbix server */
#define SERVERADDR 192, 168, 0, 200       //IP do Servidor
#define ZABBIXPORT 10051            //Porta do Servidor
#define ZABBIXAGHOST "Termometro"   //Nome do Host
#define ZABBIX_KEY1 "temp"          //Chave 1
#define ZABBIX_KEY2 "umid"          //Chave 2

float Temperature;
float Humidity;
int valid_key =0;

String ssid = "guilherme-NET"; // wi-fi ssid
String password = "@gui1234"; // wi-fi password
ESP8266ZabbixSender zSender;

void setup() {
  /* Inicia a comunicação serial */
  Serial.begin(115200);
  Serial.println();

  /* Inicia a conexão com a rede Wi-Fi*/
  Serial.println("Conectando-se a: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.print("\n");
  Serial.println("######################################################");
  Serial.println("WiFi conectado..!");
  Serial.print("IP do NodeMCU: ");  
  Serial.println(WiFi.localIP());
  Serial.println("######################################################");

  // Inicia o Zabbix sender
  zSender.Init(IPAddress(SERVERADDR), ZABBIXPORT, ZABBIXAGHOST);
}

void loop() { 
  Temperature = dht.readTemperature(); //Lê a Temperatura
  Humidity = dht.readHumidity(); //Lê a umiddade 
    


  /*Valida a Temperatura */
  if (isnan(Temperature) || Temperature <0 || Temperature >200){
     Serial.println("Erro na Coleta da Temperatura, valor coletado: " + String(Temperature));  
  }
  else { // Cria o item temperatura para o Envio
       Serial.println("Temperatura: " + String(Temperature));
       zSender.AddItem(ZABBIX_KEY1, Temperature); 
       valid_key = valid_key +1;    
    }


  /*Valida a Umidade */
  if (isnan(Humidity) || Humidity <0 || Humidity >100){
     Serial.println("Erro na Coleta da Umidade, valor coletado: " + String(Humidity));
  }
  else {// Cria o item umidade para o Envio
       Serial.println("Umidade: " + String(Humidity));
       zSender.AddItem(ZABBIX_KEY2, Humidity);
       valid_key = valid_key +1;         
    }


    if (valid_key != 0){ // verica se há algum dado para envio
      Serial.println("Enviando dados ao Zabbix Server");
      zSender.Send() == EXIT_SUCCESS; 
    }
    else {
          Serial.println ("Sem dados Validos para o Envio");
         }
  
     zSender.ClearItem();
  
  Serial.println("60 Segundos para a proxima coleta");
  delay(60000);
}