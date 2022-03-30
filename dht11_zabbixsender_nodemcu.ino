#include <ESP8266WiFi.h>
#include "DHT.h"
#include "ESP8266ZabbixSender.h"

ESP8266ZabbixSender zSender;

// Descomentar o tipo do sensor DHT a ser usado
#define DHTTYPE DHT11   // DHT 11
  //#define DHTTYPE DHT21   // DHT 21 (AM2301)
  //#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

/* Configurações WiFi */
String ssid = "guilherme-NET";
String pass = "@gui1234";

/* Parametros Zabbix server */
#define SERVERADDR 192, 168, 0, 200 // IP Address example 192.168.0.123
#define ZABBIXPORT 10051      
#define ZABBIXAGHOST "Termometro"
#define ZABBIX_KEY1 "temp"
#define ZABBIX_KEY2 "umid"

/* Strings de Validações Globais*/
boolean valida_k1;
boolean valida_k2;
String nan_temp ; // string para validar se o valor é vazio = nan
String nan_umid ; 
float Temperature;
float Humidity;

// Pino do Sensor
uint8_t DHTPin = D8; 

// Inicializa o sensor
DHT dht(DHTPin, DHTTYPE);           

void setup() {
  
  /* Inicia a comuicação serial */
  Serial.begin(115200);
  Serial.println();

  
  

  //conecta a rede
  Serial.println("Conectando-se a: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  //verifica a conexão
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("######################################################");
  Serial.println("WiFi conectado..!");
  Serial.print("IP do NodeMCU : ");  Serial.println(WiFi.localIP());
  Serial.println("######################################################");

  // Inicia o Zabbix sender
  zSender.Init(IPAddress(SERVERADDR), ZABBIXPORT, ZABBIXAGHOST);

}

void loop() {
  

  
  Temperature = dht.readTemperature(); //Lê a Temperatura
  Humidity = dht.readHumidity(); //Lê a umiddade 

  // Initialize Zabbix sender
  zSender.Init(IPAddress(SERVERADDR), ZABBIXPORT, ZABBIXAGHOST);
  
  
  nan_temp = String(Temperature);
  if (nan_temp != "nan" && Temperature >=0 && Temperature <200 ){
      zSender.AddItem(ZABBIX_KEY1, Temperature);
      valida_k1=1;
      Serial.println();
      Serial.print("Temperatura: ");Serial.println(Temperature);
       }
       else { 
              Serial.print("Erro na Coleta da Temperatura, valor coletado: ");Serial.println(Temperature);
              valida_k1=0;
             }

  nan_umid = String(Humidity);
  if (nan_umid != "nan" && Humidity >=0 && Humidity <=100 ){
      zSender.AddItem(ZABBIX_KEY2, Humidity);
        valida_k2=1;
        Serial.print("Umidade: ");Serial.println(Humidity);
        Serial.println();
      }
      else {
              Serial.print("Erro na Coleta da umiddade, valor coletado: ");Serial.println(Humidity);
              valida_k2=0;
              
            }

  if (valida_k1 != 0 || valida_k2 != 0){
      Serial.println("Enviando Dados para o Zabbix Server");
      zSender.Send() == EXIT_SUCCESS;   
      }
      else {
              Serial.println("Dados não Enviados - Verifique os Sensores");
              Serial.println();                              
            }
 
  zSender.ClearItem();
  Serial.println("60 Segundos para a proxima coleta");
  delay(30000); 
  Serial.println("30 Segundos para a proxima coleta");  
}
