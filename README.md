# **Projeto IoT com ESP32 e MQTT - Economia de Energia e Energia Sustentável**

## **Integrantes**
- Arthur Fellipe Estevão da Silva RM553320
- Eduardo Pires Escudero RM556527
- Leonardo Munhoz Prado RM556824

## **Descrição do Projeto**

Este projeto visa desenvolver um sistema IoT que conecta um **ESP32** a um servidor **MQTT**, utilizando sensores para monitoramento ambiental em tempo real. O sistema utiliza os seguintes sensores:

- **DHT22 (Temperatura e Umidade)**: Este sensor mede a temperatura e a umidade do ambiente. Ele será utilizado para monitorar as condições climáticas, permitindo a análise de variações de temperatura e umidade, essenciais para <ins>otimizar</ins> o uso de <ins>energia</ins> em ambientes internos.
  
- **LDR (Sensor de Luminosidade)**: O sensor de luminosidade será responsável por medir a intensidade da luz ambiente. Ele ajuda a determinar a necessidade de <ins>iluminação artificial</ins>, contribuindo para a <ins>economia de energia</ins> ao ajustar automaticamente os sistemas de iluminação com base na <ins>luz natural disponível</ins>.

O objetivo do projeto é coletar dados ambientais de forma contínua e enviá-los para um broker MQTT configurado em uma máquina virtual Ubuntu. Esses dados poderão ser usados para analisar as condições do ambiente em tempo real, com foco na <ins>economia de energia</ins> e <ins>sustentabilidade</ins>.

Além disso, o sistema permite o envio de comandos para atuar sobre dispositivos conectados, como um **LED**, para criar um controle inteligente de energia com base nas condições monitoradas. Dessa forma, o projeto visa melhorar a eficiência energética ao otimizar o consumo de eletricidade de acordo com as variáveis ambientais.


## **Arquitetura Proposta**

A solução IoT foi projetada com a seguinte arquitetura:

1. **Dispositivo IoT (ESP32)**:
   - Coleta de dados ambientais usando sensores de temperatura (DHT22), umidade (DHT22) e luminosidade (LDR).
   - Envio dos dados via Wi-Fi para o broker MQTT.
   - Recebimento de comandos para controlar um LED ou outro atuador.

2. **Servidor MQTT (Back-End)**:
   - Implementado com o broker **Mosquitto** em uma máquina virtual Ubuntu.
   - Gerencia a comunicação entre o ESP32 e clientes/subscritores.
   - Utiliza o protocolo **MQTT** para troca de mensagens em tópicos.

3. **Cliente MQTT (Front-End - opcional)**:
   - Qualquer cliente MQTT, como **MyMQTT** no celular ou dashboards como **Node-RED**, pode se conectar ao broker, visualizar os dados recebidos e enviar comandos para o ESP32.
   - Pode ser integrado com interfaces para visualização dos dados e controle em tempo real.

### **Diagrama da Arquitetura**

``` plaintext
+-------------------+           +---------------------+         +-----------------+
|    Dispositivo     |           |       Back-End      |         |    Cliente      |
|       IoT          |           |    (Servidor MQTT)  |         |  (Front-End)    |
|   (ESP32 + Sensores)  --------> |   (Mosquitto em     | <------> |  (MyMQTT ou     |
|  Publica dados de   |           |   VM Ubuntu)        |         |  Node-RED)      |
|  temperatura,       |           | Recebe e envia      |         |                |
|  umidade e          |           | mensagens           |         |                |
|  luminosidade       |           | entre IoT e cliente |         |                |
+-------------------+           +---------------------+         +-----------------+
```

## **Recursos Necessários**

### **1. Dispositivos IoT**
- **ESP32**: Microcontrolador que se conecta à rede Wi-Fi e envia dados para o servidor MQTT.
- **DHT22**: Sensor de temperatura e umidade.
- **Sensor de Luminosidade**: Para medir a intensidade da luz ambiente.

### **2. Back-End**
- **Mosquitto MQTT Broker**: Instalado em uma máquina virtual **Ubuntu** para gerenciar a comunicação via MQTT.
  - **Mosquitto Clients**: Para testes e subscrição aos tópicos.

### **3. Front-End**
- **Cliente MQTT**: Pode ser um aplicativo como **MyMQTT** (disponível para Android) ou dashboards como **Node-RED** para visualizar e interagir com os dados enviados pelo ESP32.

---

## **Instruções de Uso**

### **1. Configuração do Servidor MQTT**
- Instale o Mosquitto na máquina virtual Ubuntu:
  ``` bash
  sudo apt update
  sudo apt install mosquitto mosquitto-clients
  ```
- Inicie o serviço Mosquitto:
  ``` bash
  sudo systemctl start mosquitto
  sudo systemctl enable mosquitto
  ```
- Configure o broker para aceitar conexões externas, se necessário (edição do arquivo /etc/mosquitto/mosquitto.conf):
  ``` plaintext
  listener 1883
  allow_anonymous true
  ```
  ### **2. Configuração do ESP32**
- Faça o upload do código no ESP32 utilizando o **Arduino IDE** ou **PlatformIO**.
- Configure os seguintes parâmetros no código:
  - **SSID**: Rede Wi-Fi à qual o ESP32 se conectará.
  - **BROKER_MQTT**: Endereço IP do servidor Mosquitto.
  - **ID_MQTT**: Identificação do ESP32 no servidor.
  - **Tópicos MQTT**: Ajuste os tópicos conforme necessário, como:
    - \`/TEF/device022/cmd\`: Tópico para receber comandos.
    - \`/TEF/device022/attrs\`: Tópico para enviar dados dos sensores.

### **3. Conexão do Cliente MQTT**
- Use um cliente MQTT como **MyMQTT** ou **Node-RED** para se conectar ao broker e visualizar os dados publicados pelo ESP32.
  - **Host**: Endereço IP do servidor Mosquitto.
  - **Porta**: 1883.

---

## **Requisitos**

- **ESP32**: Deve estar configurado com as bibliotecas:
  - \`WiFi.h\` para conexão à internet.
  - \`PubSubClient.h\` para comunicação via MQTT.
  - \`DHT.h\` para leitura de temperatura e umidade.
  
- **Broker Mosquitto**:
  - **Ubuntu 18.04 ou superior**: Máquina virtual configurada com Mosquitto para gerenciar a comunicação MQTT.
---

## **Dependências**

### **No ESP32**
- Bibliotecas necessárias:
  - \`WiFi.h\`
  - \`PubSubClient.h\`
  - \`DHT.h\`

### **No Ubuntu**
- **Mosquitto MQTT Broker**: Deve estar instalado e rodando corretamente.
- **Clientes MQTT**: Para testes, instale o cliente Mosquitto:
  \`\`\`bash
  sudo apt install mosquitto-clients
  \`\`\`

---

## **Exemplo de Publicação e Subscrição**

### **Publicação (ESP32 envia dados)**:
- **Tópico**: \`/TEF/device022/attrs\`
- **Payload**: Informações de temperatura, umidade, e luminosidade.
  
### **Subscrição (Recebendo comandos)**:
- **Tópico**: \`/TEF/device022/cmd\`
- **Exemplo de comando**: \`"device022@on|"\` para ligar o LED, \`"device022@off|"\` para desligar.

---

## **Conclusão**

Este projeto demonstra a aplicação de um sistema IoT completo, utilizando um ESP32 para coleta de dados e comunicação via MQTT com um servidor Mosquitto instalado em uma máquina virtual Ubuntu. Ele possibilita o monitoramento de sensores em tempo real e a interação com atuadores, como o controle de um LED.
