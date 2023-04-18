#include <ESP8266WiFi.h>
#include "DHT.h"
#include "ESP8266ZabbixSender.h"
#define DHTTYPE DHT22     // Define o modelo do DHT
#define DHTPin 4          // Pino digital D2 (GPIO5) conectado ao DHT11
DHT dht(DHTPin, DHTTYPE); // Inicializa o sensor
/* Parametros Zabbix server */
#define SERVERADDR 10, 1, 1, 180                   // IP do Servidor
#define ZABBIXPORT 10051                           // Porta do Servidor
#define ZABBIXAGHOST "NOME_DO_HOST"                // Nome do Host
#define ZABBIX_KEY1 "temp"                         // Chave 1
#define ZABBIX_KEY2 "umid"                         // Chave 2
#define ZABBIX_KEY3 "versao"                       // Chave 3
#define ZABBIX_KEY4 "ipaddr"                       // Chave 4
/*Parametros de Conexao*/
String ssid = "NOME_DA_REDE";      // ssid da rede Wi-Fi
String password = "SENHA_DA_REDE; // senha da rede Wi-Fi
/*Outros Parametros*/
float Temperature;
float Humidity;
String Version = "v1.2";
String ipaddr;
int valid_key = 0;
ESP8266ZabbixSender zSender;

void setup()
{
    /* Inicia a comunicação serial */
    Serial.begin(115200);
    Serial.println();

    /* Inicia a conexão com a rede Wi-Fi*/
    Serial.println("Conectando-se a: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
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

void loop()
{

    dht.begin();                         // Inicia o sensor
    Temperature = dht.readTemperature(); // Lê a Temperatura
    Humidity = dht.readHumidity();       // Lê a umiddade

    /*Valida a Temperatura */
    if (isnan(Temperature) || Temperature < 0 || Temperature > 200)
    {
        Serial.println("Erro na Coleta da Temperatura, valor coletado: " + String(Temperature));
    }
    else
    { // Cria o item temperatura para o Envio
        Serial.println("Temperatura: " + String(Temperature));
        zSender.AddItem(ZABBIX_KEY1, String(Temperature));
        valid_key = valid_key + 1;
    }

    /*Valida a Umidade */
    if (isnan(Humidity) || Humidity < 0 || Humidity > 100)
    {
        Serial.println("Erro na Coleta da Umidade, valor coletado: " + String(Humidity));
    }
    else
    { // Cria o item umidade para o Envio
        Serial.println("Umidade: " + String(Humidity));
        zSender.AddItem(ZABBIX_KEY2, String(Humidity));
        valid_key = valid_key + 1;
    }

    zSender.AddItem(ZABBIX_KEY3, Version); // Cria o item Versao para o Envio
                                           /* Atribui o IP a variavel e depois a envia */
    ipaddr = WiFi.localIP().toString();
    zSender.AddItem(ZABBIX_KEY4, ipaddr);

    if (valid_key != 0)
    { // verica se há algum dado para envio
        Serial.println("Enviando dados ao Zabbix Server");
        zSender.Send() == EXIT_SUCCESS;
    }
    else
    {
        Serial.println("Sem dados Validos para o Envio");
    }
    zSender.ClearItem(); // Limpa os itens para o proximo loop.
    Serial.println("60 Segundos para a proxima coleta");
    delay(60000);
}
