# 🔘 IoT Status dos Botões com Placa BitDogLab (Raspberry Pi Pico W)

**Esta é uma aplicação embarcada desenvolvida como parte da Tarefa Prática da Unidade 2 – Capítulo 3 ("Aplicações com Microcontroladores") da Residência Tecnológica em Sistemas Embarcados.**

O objetivo é utilizar a placa BitDogLab (com o Raspberry Pi Pico W) para monitorar o estado de dois botões (Botão A e Botão B), exibir esse estado em um display OLED local e transmitir essas informações para a nuvem. A aplicação reforça conceitos de conectividade Wi-Fi, interação com periféricos (display OLED, GPIO para botões), e integração com uma API Web hospedada na Railway.

---

## 🎯 Funcionalidades Principais:

* **Monitoramento dos Botões:** Verifica continuamente o estado dos botões A e B integrados à placa BitDogLab (pressionado ou solto).
* **Display OLED Local:** Apresenta o status atual de cada botão (ex: "Botao A: Pressionado", "Botao B: Solto") no display OLED.
* **Conectividade Wi-Fi:** Utiliza o módulo Wi-Fi do Raspberry Pi Pico W na BitDogLab para se conectar à rede e à internet.
* **Envio de Dados para Nuvem:** Transmite o estado dos botões (true para pressionado, false para solto) para uma API REST na nuvem (hospedada na Railway) via HTTP POST sempre que o estado de algum botão é alterado.

---

## 🛠️ Hardware Utilizado:

* **Placa de Desenvolvimento:** BitDogLab (com Raspberry Pi Pico W embarcado).
* **Sensores de Entrada:** Botões A e B integrados à BitDogLab.
* **Display:** Display OLED I2C 128x64 integrado à BitDogLab.

---

## 📁 Estrutura do Projeto:

```
iot-status_botoes/
├── lib/                            // Arquivos de bibliotecas externas
│   ├── ssd1306/                    // Biblioteca para o display OLED      
│   │   ├── font.h                  
│   │   ├── ssd1306.c               
│   │   └── sdd1306.h               
├── src/                            // Código fonte do projeto
│   ├── connection/                 // Código fonte para conexão com a API
│   │   ├── cliente_http.c
│   │   └── cliente_http.h
│   ├── utils/                      // Código fonte para funções utilitárias
│   │   ├── botoes/                 // Código fonte para os botões
│   │   │   ├── botoes.c
│   │   │   └── botoes.h
│   │   ├── display/                // Código fonte para o display OLED     
│   │   │   ├── displayOLED.c
│   │   │   └── displayOlED.h
│   │   ├── wifi/                   // Código fonte para conexão Wi-Fi 
│   │   │   ├── wifi.c
│   │   │   └── wifi.h
│   └── iot_status_botoes.c         // Código principal do projeto
├── .gitignore
├── CMakeLists.txt
├── pico_sdk_import.cmake
└── README.md
```

---

## ☁️ Comunicação com a Nuvem (API via Proxy Railway).

A placa BitDogLab envia os dados para uma API Flask hospedada na plataforma Railway, utilizando um **proxy direto por host e porta**.

### 🔌 Como funciona:

- O Railway fornece um **host e uma porta TCP** dedicados (ex: `containers.up.railway.app:12345`) para que dispositivos externos possam se comunicar diretamente com a aplicação, mesmo sem HTTPS.
- A placa envia as requisições HTTP diretamente para esse host, usando o caminho `PROXY_HOST`.

### 📌 Exemplo de configuração:
```c
#define PROXY_HOST "containers.up.railway.app"
#define PROXY_PORT 12345
```

> ⚠️ A API está preparada para receber esses dados e retransmiti-los via WebSocket em tempo real para dashboards HTML acessados via navegador.

Essa abordagem é ideal para microcontroladores como o Raspberry Pi Pico W, que têm suporte limitado para conexões HTTPS.

---

## 🔐 Por que HTTP e não HTTPS na BitDogLab?

Implementar HTTPS diretamente no microcontrolador é custoso. A plataforma Railway atua como um **proxy reverso**, permitindo que a placa use **HTTP** simples enquanto a comunicação com o usuário final (navegador) permanece segura via **HTTPS**.

---

## 🚀 Como Compilar, Configurar e Gravar na sua Placa.

Este projeto usa o **Pico SDK versão 1.5.1** e deve ser importado corretamente para compilar.

### 📥 Passos para usar:

1. **Clone o repositório:**
   ```bash
   git clone https://github.com/LincolnMatheus97/Iot-status-botoes.git
   cd Iot-status-botoes
   ```

2. **Importe no VS Code com a extensão "Raspeberry Pi Pico Project":**
   - Abra o VS Code
   - Vá na extensão, clice em Import Project
   - Selecione o diretorio aonde esta o repositório clonado
   - Altere a versar para **1.5.1**, e clique em create

3. **Configure a conexão Wi-Fi**
   - No arquivo `wifi.h`, altere os seguintes campos:
     ```c
     #define NOME_REDE_WIFI "SEU_SSID"
     #define SENHA_REDE_WIFI "SUA_SENHA"
     ```

4. **Compile:**
   - Pressione `Ctrl + Shift + B` para compilar
   - O arquivo `.uf2` será gerado automaticamente na pasta `build/`

5. **Grave na BitDogLab:**
   - Pressione e segure o botão `BOOTSEL` enquanto conecta a placa ao computador
   - Solte o botão após a montagem como unidade USB
   - Copie o arquivo `.uf2` para dentro da unidade

---
