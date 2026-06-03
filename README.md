# 🌬️ Sistema de Monitoramento da Qualidade do Ar — IoT

**Universidade Presbiteriana Mackenzie — Faculdade de Computação e Informática**  
Luana Fernandes, Marcelo Takao, Kauan Rodrigues

---

## 📋 Descrição do Projeto

Sistema baseado em Internet das Coisas (IoT) para monitoramento da qualidade do ar em regiões urbanas de alto fluxo veicular. Dois ESP32 simulados no Wokwi coletam dados de qualidade do ar (MQ-135), temperatura e umidade (DHT22) em tempo real. Os dados são transmitidos via protocolo MQTT, processados no Node-RED com regras de negócio de classificação, enriquecidos com dados climáticos da API OpenWeatherMap, armazenados no InfluxDB Cloud e visualizados em dashboards interativos no Grafana. O projeto está alinhado aos ODS 3 (Saúde e Bem-estar) e ODS 11 (Cidades Sustentáveis) da ONU.

---

## 🏗️ Arquitetura do Sistema

```
[ESP32 Sensor 1] ──┐
                   ├──► [HiveMQ Broker MQTT] ──► [Node-RED] ──► [InfluxDB Cloud] ──► [Grafana]
[ESP32 Sensor 2] ──┘                                  ▲
                                                       │
                                          [OpenWeatherMap API]
```


---

## ✅ Pré-requisitos

### Softwares
- [Node.js](https://nodejs.org) v18 ou superior
- Navegador moderno (Chrome ou Firefox)

### Contas necessárias (todas gratuitas)
| Plataforma | Link | Para que serve |
|---|---|---|
| Wokwi | [wokwi.com](https://wokwi.com) | Simulação dos ESP32 |
| HiveMQ | Não precisa de conta | Broker MQTT público |
| OpenWeatherMap | [openweathermap.org](https://openweathermap.org) | API de dados climáticos |
| InfluxDB Cloud | [cloud2.influxdata.com](https://cloud2.influxdata.com) | Banco de dados time-series |
| Grafana Cloud | [grafana.com](https://grafana.com) | Dashboard e visualização |
| GitHub | [github.com](https://github.com) | Repositório do projeto |

---

## ⚙️ Configuração Passo a Passo

### 1. Instalar o Node-RED

Abra o terminal e execute:

```bash
npm install -g --unsafe-perm node-red
node-red
```

Acesse: http://localhost:1880

### 2. Instalar o pacote InfluxDB no Node-RED

1. Menu ☰ → **Manage palette** → aba **Install**
2. Busque `node-red-contrib-influxdb3` e clique em **Install**

### 3. Configurar o InfluxDB Cloud

1. Crie uma conta em [cloud2.influxdata.com](https://cloud2.influxdata.com)
2. Crie um bucket chamado `qualidade_ar`
3. Gere um token de acesso em **Load Data → API Tokens**
4. Anote a URL do servidor (ex: `https://us-east-1-1.aws.cloud2.influxdata.com`)

### 4. Configurar a OpenWeatherMap

1. Crie uma conta em [openweathermap.org](https://openweathermap.org)
2. Acesse **My API Keys** e copie sua chave
3. Insira a chave na URL do node HTTP Request no Node-RED

### 5. Configurar o Wokwi

1. Acesse [wokwi.com](https://wokwi.com) e crie um projeto ESP32
2. Adicione os componentes DHT22 e potenciômetro
3. Cole o código do sketch disponível em `wokwi/esp32_sensor1/sketch.ino`
4. Repita para o Sensor 2 com o código em `wokwi/esp32_sensor2/sketch.ino`

> 📸 <img width="1919" height="941" alt="image" src="https://github.com/user-attachments/assets/b974f1b0-d7ec-418c-9b80-ffe077a5e639" />


---

## 📥 Como Importar o flows.json no Node-RED

1. Acesse http://localhost:1880
2. Clique no menu ☰ → **Import**
3. Clique em **"select a file to import"**
4. Selecione o arquivo `node-red/flows.json`
5. Clique em **Import**
6. Atualize as credenciais:
   - **Token do InfluxDB** no node `InfluxDB Cloud`
   - **API Key da OpenWeatherMap** no node `HTTP Request`
7. Clique em **Implementar**

> 📸 <img width="1919" height="859" alt="image" src="https://github.com/user-attachments/assets/83fe46f8-9979-4561-8d4a-28f5347fe2f3" />


---

## 📊 Como Importar o dashboard.json no Grafana

1. Acesse seu Grafana Cloud
2. Menu lateral → **Dashboards** → **New** → **Import dashboard**
3. Clique em **"Upload dashboard JSON file"**
4. Selecione o arquivo `grafana/dashboard.json`
5. Em **InfluxDB**, selecione o seu Data Source
6. Clique em **Import**

---

## 📡 Tópicos MQTT Utilizados

| Tópico | Sensor | Descrição |
|---|---|---|
| `qualidade-ar/sensor1` | ESP32 Sensor 1 | Dados de AQI, temperatura e umidade do ponto 1 |
| `qualidade-ar/sensor2` | ESP32 Sensor 2 | Dados de AQI, temperatura e umidade do ponto 2 |

**Broker:** `broker.hivemq.com` — porta `1883`

**Formato da mensagem (JSON):**
```json
{
  "sensor": "sensor1",
  "temperatura": 24.5,
  "umidade": 60.0,
  "aqi": 85
}
```

---

## 🌡️ Classificação da Qualidade do Ar

| Faixa do AQI | Classificação | Cor | Ação |
|---|---|---|---|
| 0 — 50 | ✅ Boa | Verde | Sem risco à saúde |
| 51 — 100 | ⚠️ Moderada | Amarelo | Atenção para grupos sensíveis |
| 101 — 150 | 🔴 Ruim | Vermelho | Risco à saúde — alerta disparado |

---

## 🗄️ Dados Armazenados no InfluxDB

### Measurement: `qualidade_ar`
| Campo | Tipo | Descrição |
|---|---|---|
| `aqi` | float | Índice de qualidade do ar |
| `temperatura` | float | Temperatura em °C |
| `umidade` | float | Umidade relativa em % |
| `classificacao` | string | boa / moderada / ruim |
| `alerta` | int | 0 = sem alerta, 1 = alerta crítico |
| `sensor` | string (tag) | sensor1 ou sensor2 |

### Measurement: `clima`
| Campo | Tipo | Descrição |
|---|---|---|
| `temp_externa` | float | Temperatura externa (OpenWeatherMap) |
| `umidade_externa` | float | Umidade externa |
| `pressao` | float | Pressão atmosférica |
| `vento_velocidade` | float | Velocidade do vento |
| `clima_descricao` | string | Descrição do clima |

---

## 📱 Painéis do Dashboard

| Painel | Tipo | Descrição |
|---|---|---|
| IQA — Sensor 1 | Gauge | Índice atual com cores verde/amarelo/vermelho |
| IQA — Sensor 2 | Gauge | Índice atual do segundo ponto |
| Evolução da Qualidade do Ar | Time series | Histórico temporal dos dois sensores |
| Temperatura | Time series | Variação de temperatura ao longo do tempo |
| Umidade | Time series | Variação de umidade ao longo do tempo |
| Comparação Sensor 1 vs Sensor 2 | Bar chart | Comparação visual entre os dois sensores |
| Alertas de Poluição Crítica | Table | Registros com classificação "ruim" |
| Condições Climáticas Externas | Stat | Dados em tempo real da OpenWeatherMap |

> 📸 <img width="1895" height="853" alt="image" src="https://github.com/user-attachments/assets/e83410a7-6770-4592-abe0-39e9c104307c" />



---

## 📸 Screenshots do Sistema

### Node-RED — Fluxo completo
> 📸 <img width="1307" height="852" alt="image" src="https://github.com/user-attachments/assets/8ea166aa-c5b4-4b6b-9e49-6e6d60d18b98" />


### Wokwi — Sensores simulados
> 📸 <img width="1919" height="943" alt="image" src="https://github.com/user-attachments/assets/3e4a4622-42ee-419c-9e50-92060f06eb76" />


### InfluxDB — Dados gravados
> 📸 <img width="1462" height="924" alt="image" src="https://github.com/user-attachments/assets/4e4d7a21-b88f-433e-9a7d-b9d9bf6f9665" />


### Grafana — Dashboard em tempo real
> 📸 <img width="1893" height="824" alt="image" src="https://github.com/user-attachments/assets/6347ae2a-56b8-4f1b-8a6d-fd0e253c5fd9" />


---

## 🗂️ Estrutura do Repositório

```
monitoramento-qualidade-ar-iot/
│
├── README.md
│
├── wokwi/
│   ├── esp32_sensor1/
│   │   ├── sketch.ino
│   │   └── diagram.json
│   └── esp32_sensor2/
│       ├── sketch.ino
│       └── diagram.json
│
├── node-red/
│   └── flows.json
│
├── grafana/
│   └── dashboard.json
│
└── docs/
    └── artigo.pdf
```

---

## 📚 Referências

- ALMEIDA, R. *Aplicações de Internet das Coisas em cidades inteligentes*. São Paulo: Editora Tecnológica, 2019.
- ONU. *Objetivos de Desenvolvimento Sustentável*. Disponível em: https://brasil.un.org/pt-br/sdgs
- SANTOS, M.; OLIVEIRA, P. *Visualização de dados em sistemas IoT*. Revista Brasileira de Tecnologia, 2021.
- SILVA, J. *Monitoramento ambiental com sensores inteligentes*. Rio de Janeiro: Ciência Moderna, 2020.
- WORLD HEALTH ORGANIZATION (WHO). *Air pollution*. Disponível em: https://www.who.int

---

## 📄 Licença

Projeto acadêmico desenvolvido para a disciplina de IoT — Universidade Presbiteriana Mackenzie, 2026.
