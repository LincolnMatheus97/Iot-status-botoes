#ifndef CLIENTE_HTTP
#define CLIENTE_HTTP

// --- Inclui as bibliotecas necessárias ---
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/cyw43_arch.h"
#include "lwip/dns.h"
#include "lwip/ip_addr.h"
#include "lwip/tcp.h"

// --- Define o endereço do proxy e a porta---
#define PROXY_HOST "maglev.proxy.rlwy.net"
#define PROXY_PORT 48443

typedef struct {
    bool botao_a;
    bool botao_b;
} StatusBotoes;

void enviar_dados_para_nuvem(const StatusBotoes *dados_a_enviar);

#endif