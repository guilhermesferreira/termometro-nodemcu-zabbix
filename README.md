# Termômetro Iot com NodeMCU e Zabbix Sender
Termômetro IoT utilizando nodemcu (esp8266) e enviando as métricas para o zabbix. 

# Bibliotecas Consumidas
 - ESP8266WiFi.h
 - DHT.h
 - ESP8266ZabbixSender.h

 # Hardwares Utilizados
 - NodeMCU v2.0
 - Módulo do Sensor DHT11 3 Pinos


# Zabbix Server

Itens Criados No Zabbix Server:
![](/Imagens/Host.png)



- -Item Temperatura:
![](/Imagens/Item_temperatura.png)
- Tipo: Trapper Zabbix
- Chave: temp
- Tipo de Informação: Float
- Unidade: °C



- Itens:
![](/Imagens/NodeMCU%20Zabbix%20HOST.png)
 - Temperatura
 - Umidade

- Grafana:
![](/Imagens/NodeMCU%20Grafana.png)
 - Grafico de Temperatura
 - Grafico de Umidade
 - Gauge de Temperatura
 - Gauge de Umidade
