#include <stdio.h>
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "botoes.h"
#include "cliente_http.h"
#include "wifi.h"
#include "display.h"

StatusBotoes *status_botoes = NULL;

bool estado_ant_botao_a = false;
bool estado_ant_botao_b = false;

int iniciar_conexao_wifi() 
{
    limpar_display();
    escrever_display("Conectando em:", 23, 20, 1);
    escrever_display(NOME_REDE_WIFI, 23, 32, 1);
    mostrar_display();

    int status_conexao = conexao_wifi();

    limpar_display();
    if (status_conexao == 0) {
        escrever_display("CONEXAO ESTABECIDA", 10, 30, 1);
        printf("IP do dispositivo: %s\n", ipaddr_ntoa(&netif_default->ip_addr));
    } else {
        escrever_display("FALHA NA CONEXAO", 10, 30, 1);
    }
    mostrar_display();
    sleep_ms(3000);

    return status_conexao;
}

void atualizar_status_no_display(bool status_botao_a, bool status_botao_b)
{
    limpar_display();

    escrever_display("Status Botões", 22, 0, 1);

    char status_a[15];
    (status_botao_a = true) ? (strcmp(status_a, "Pressionado")) : (strcmp(status_a, "Solto"));

    char status_b[15];
    (status_botao_b = true) ? (strcmp(status_b, "Pressionado")) : (strcmp(status_b, "Solto"));

    char dados_botao_a[20];
    snprintf(dados_botao_a, sizeof(dados_botao_a), "Botão A: %s ", status_a);
    escrever_display(dados_botao_a, 5, 32, 1);

    char dados_botao_b[20];
    snprintf(dados_botao_b, sizeof(dados_botao_b), "Botão B: %s ", status_b);
    escrever_display(dados_botao_b, 6, 32, 1);

    mostrar_display();
}

void iniciar_perifericos()
{
    stdio_init_all();
    inic_barr_i2c();
    inic_display();

    status_botoes = (StatusBotoes*)malloc(sizeof(StatusBotoes));
    status_botoes->botao_a = false;
    status_botoes->botao_b = false;
}

int main()
{
    iniciar_perifericos(); 
    sleep_ms(1500);

    iniciar_conexao_wifi();
    
    estado_ant_botao_a = botao_a_pressionado();
    estado_ant_botao_b = botao_b_pressionado();
    
    atualizar_status_no_display(estado_ant_botao_a, estado_ant_botao_b);
    
    enviar_dados_para_nuvem(status_botoes);

    while (true)
    {
        cyw43_arch_poll(); // Verifica se há pacotes recebidos e processa-os

        bool estado_atual_botao_a = botao_a_pressionado();
        bool estado_atual_botao_b = botao_b_pressionado();
        
        
        if ((estado_ant_botao_a != estado_atual_botao_a) || (estado_ant_botao_b != estado_atual_botao_b))
        {
            
            status_botoes->botao_a = estado_atual_botao_a;
            status_botoes->botao_b = estado_atual_botao_b;

            atualizar_status_no_display(status_botoes->botao_a, status_botoes->botao_b);
            
            enviar_dados_para_nuvem(status_botoes);

            // Atualiza os valores anteriores para a próxima comparação
            
            estado_ant_botao_a = status_botoes->botao_a;
            estado_ant_botao_b = status_botoes->botao_b;
        }
        
        atualizar_status_no_display(estado_atual_botao_a, estado_atual_botao_b);

        sleep_ms(100); 
    }

    cyw43_arch_deinit();  
    free(status_botoes); 
    return 0;
}