# Monitor de Status de Botões IoT com RP2040 (BitDogLab) e API Flask no Railway

Este projeto monitora o estado de botões conectados a uma placa BitDogLab com microcontrolador RP2040 e envia essas informações para uma API Flask hospedada na plataforma Railway.

## Visão Geral

O sistema lê continuamente o estado de botões configurados nos pinos GPIO da placa. Em intervalos regulares (definidos em `iot_status_botoes.c`), ele se conecta a uma rede Wi-Fi especificada e envia os dados de status dos botões via HTTP para um endpoint específico (`/dados`) da API Flask (`Api-Iot-RSE/app.py`) rodando no Railway. A API armazena o último estado recebido e o exibe em um dashboard web.

## Funcionalidades

- Inicialização dos pinos GPIO configurados como entrada para os botões (definido em `utils/sensores.c` e `utils/sensores.h`).
- Conexão a uma rede Wi-Fi usando credenciais pré-definidas (configurado em `iot_status_botoes.c`).
- Leitura do estado dos botões (implementado em `utils/sensores.c`).
- Envio dos dados de status para a API Flask no Railway via requisição HTTP POST (lógica em `connection/cliente_http.c`).
- API Flask (`Api-Iot-RSE/app.py`) para receber os dados e exibir um dashboard simples.
- Tratamento básico de erros de conexão Wi-Fi.

## Hardware

- **Placa:** BitDogLab com RP2040
- **Conectividade:** Módulo Wi-Fi integrado (CYW43439) compatível com `pico/cyw43_arch.h`.
- **Sensores:** Botões conectados aos pinos GPIO definidos em `utils/sensores.h` (PIN_BOTAO_A, PIN_BOTAO_B).

## Fluxo de Dados

1.  **Inicialização (Pico):** O microcontrolador (`iot_status_botoes.c`) inicializa a comunicação serial, os pinos GPIO dos botões (`inicializar_botoes()` em `utils/sensores.c`) e o módulo Wi-Fi.
2.  **Conexão Wi-Fi (Pico):** A placa tenta se conectar à rede Wi-Fi (`NOME_REDE_WIFI`, `SENHA_REDE_WIFI` em `iot_status_botoes.c`).
3.  **Leitura e Envio (Pico):**
    - O loop principal (`while(true)` em `iot_status_botoes.c`) chama `enviar_dados_para_nuvem()` (de `connection/cliente_http.c`) a cada 1 segundo.
    - `enviar_dados_para_nuvem()` lê o estado dos botões (`botao_a_pressionado()`, `botao_b_pressionado()` de `utils/sensores.c`).
    - Formata os dados em JSON (`{"botao_a": X, "botao_b": Y}`).
    - Resolve o endereço IP do host do servidor (`PROXY_HOST` em `cliente_http.h`).
    - Envia uma requisição HTTP POST para o endpoint `/dados` no servidor (`PROXY_HOST` na porta `PROXY_PORT`).
4.  **Recepção (API Flask):**
    - A API Flask (`Api-Iot-RSE/app.py`), hospedada no Railway, escuta por requisições no endpoint `/dados`.
    - Ao receber um POST, a função `receber_dados()` extrai o JSON, armazena os valores na variável `ultimo_dado` e imprime no console do servidor.
5.  **Visualização (Dashboard):**
    - Acessando o endpoint `/dashboard/botoes` da API no navegador, a função `dashboard_botoes()` lê a variável `ultimo_dado`.
    - Renderiza uma página HTML simples que mostra o estado atual ("Pressionado!" ou "Solto") de cada botão, atualizando a cada 1 segundo.

## Comunicação HTTP e HTTPS (Railway)

- **Pico W (Cliente) -> Railway (Servidor):** A comunicação entre a placa RP2040 e a API no Railway é feita usando **HTTP** (não criptografado). Isso é configurado em `connection/cliente_http.c`. A razão para usar HTTP é a simplicidade e a menor demanda de recursos (memória, processamento) no microcontrolador, já que implementar HTTPS (com TLS/SSL) é significativamente mais complexo e pesado para dispositivos embarcados como o RP2040. A placa envia a requisição HTTP POST para o host definido em `PROXY_HOST` (que deve ser o URL público da sua aplicação Railway, ex: `web-production-50a2c.up.railway.app`) na porta `PROXY_PORT` (geralmente 80 para HTTP).
- **Navegador (Cliente) -> Railway (Servidor):** Quando você acessa o dashboard (`/dashboard/botoes`) pelo navegador usando o URL fornecido pelo Railway (`https://web-production-50a2c.up.railway.app/...`), a comunicação é feita usando **HTTPS** (criptografado). A plataforma Railway gerencia automaticamente o certificado TLS/SSL e a terminação HTTPS. Isso significa que o Railway recebe sua requisição segura (HTTPS), descriptografa-a e a encaminha como uma requisição HTTP simples para a sua aplicação Flask que está rodando dentro do container.

**Em resumo:** A placa envia dados de forma simples (HTTP) para a API. O Railway garante que o acesso externo à API (como o seu acesso ao dashboard pelo navegador) seja seguro (HTTPS). O `PROXY_HOST` e `PROXY_PORT` no código do Pico devem apontar para o endereço HTTP (porta 80) da sua aplicação no Railway.

## Como Visualizar os Dados

Os dados enviados pela placa RP2040 são recebidos pela API Flask e podem ser visualizados em tempo real através do dashboard web:

1.  **Acesse o Dashboard:** Abra o seguinte URL no seu navegador:
    [https://web-production-50a2c.up.railway.app/dashboard/botoes](https://web-production-50a2c.up.railway.app/dashboard/botoes)
2.  **Visualize:** A página exibirá o último estado recebido dos botões "A" e "B" ("Pressionado!" ou "Solto"). A página se atualiza automaticamente a cada segundo para refletir novos dados recebidos pela API.

_Observação: Se nenhum dado foi enviado ainda pela placa ou se a API foi reiniciada, o status pode aparecer como "N/A"._

## Instalação na sua Placa BitDogLab RP2040

A placa BitDogLab é uma placa de aprendizado que utiliza uma placa de desenvolvimento Raspberry Pi Pico W com um microcontrolador RP2040. Para instalar este firmware na sua placa, siga os passos abaixo:

**Pré-requisitos:**

- **Ambiente de Desenvolvimento Pico:** Certifique-se de ter o ambiente de desenvolvimento para a Raspberry Pi Pico configurado, incluindo o **Pico SDK versão 1.5.1**, a toolchain ARM GCC e o CMake. Siga as instruções oficiais da Raspberry Pi para a configuração.

**Passos:**

1.  **Clone o Repositório:** Obtenha o código-fonte do projeto:

    ```bash
    git clone <url_do_repositorio_pico>
    cd iot_status_botoes
    ```

2.  **Configure as Credenciais Wi-Fi:**

    - Abra o arquivo `src/iot_status_botoes.c`.
    - Substitua `"SUA_REDE_WIFI"` e `"SUA_SENHA"` pelas credenciais da sua rede Wi-Fi nas macros `NOME_REDE_WIFI` e `SENHA_REDE_WIFI`.

3.  **Configure o Endpoint do Servidor:**

    - Abra o arquivo `src/connection/cliente_http.h` (ou onde `PROXY_HOST` e `PROXY_PORT` são definidos).
    - Certifique-se de que `PROXY_HOST` esteja definido como o URL da sua API no Railway (ex: `"web-production-50a2c.up.railway.app"`) e `PROXY_PORT` como `80`.

4.  **Compile o Projeto:**

    - Navegue até o diretório do projeto no terminal.
    - Crie um diretório de build e execute o CMake, especificando a placa Pico W e garantindo que o SDK 1.5.1 seja encontrado:

    ```bash
    mkdir build
    cd build
    cmake .. -DPICO_BOARD=pico_w -DPICO_SDK_VERSION_STRING=1.5.1
    ```

    - Compile o projeto usando `make`:

    ```bash
    make
    ```

5.  **Flashe o Firmware:**

    - Coloque sua placa BitDogLab (Pico W) em modo BOOTSEL (pressione e segure o botão BOOTSEL enquanto conecta o cabo USB).
    - A placa aparecerá como um dispositivo de armazenamento em massa (RPI-RP2).
    - Copie o arquivo `iot_status_botoes.uf2` gerado no diretório `build` para a unidade RPI-RP2.
    - A placa reiniciará automaticamente com o novo firmware.

6.  **Monitore (Opcional):**
    - Use um terminal serial para visualizar as mensagens de log da placa (conexão Wi-Fi, envio de dados, etc.).

Agora sua placa BitDogLab deve estar conectada ao Wi-Fi, enviando o status dos botões para a API no Railway, e você pode visualizar o status no dashboard web.
