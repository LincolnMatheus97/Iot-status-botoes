/**
 * @file iot_status_botoes.c
 * @brief Código principal do projeto IoT Status Botões.
 * @author Lincoln Matheus Costa Campelo Pinho
 * @date 2025-05-08
 * @version 1.0
 * @details
 * Este código implementa um sistema IoT que monitora o estado de dois botões (A e B) e envia os dados para a nuvem.
 * O sistema utiliza um microcontrolador Raspberry Pi Pico W, que se conecta a uma rede Wi-Fi e envia os dados coletados via HTTP.
 */

// -- INCLUDES --
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// -- INCLUDES DO PROJETO --
#include "botoes.h"
#include "cliente_http.h"
#include "wifi.h"
#include "display.h"

// -- DECLARAÇÕES  DE VARIAVEIS--
StatusBotoes *status_botoes = NULL; // Estrutura para armazenar o estado dos botões
bool estado_ant_botao_a = false;    // Variável para armazenar o estado anterior do botão A
bool estado_ant_botao_b = false;    // Variável para armazenar o estado anterior do botão B

/**
 * @brief Inicia a conexão Wi-Fi e exibe o status no display.
 * @return 0 se a conexão for bem-sucedida, -1 caso contrário.
 * @note Inicializa o módulo Wi-Fi, ativa o modo estação e tenta conectar à rede especificada.
 */
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

/**
 * @brief Atualiza o status dos botões no display OLED.
 * @param status_botao_a Status do botão A (pressionado ou solto).
 * @param status_botao_b Status do botão B (pressionado ou solto).
 * @note Limpa o display e exibe o status atual dos botões.
 */
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

/**
 * @brief Inicializa os periféricos do sistema.
 * @note Inicializa a comunicação serial, o I2C, o display OLED e os botões.
 */
void iniciar_perifericos()
{
    stdio_init_all();
    inic_barr_i2c();
    inic_display();

    status_botoes = (StatusBotoes*)malloc(sizeof(StatusBotoes));
    status_botoes->botao_a = false;
    status_botoes->botao_b = false;
}

/**
 * @brief Função principal do programa.
 * @return 0 se o programa for executado com sucesso.
 * @note Inicializa os periféricos, a conexão Wi-Fi e lê os estados dos botões.
 *      Atualiza o status dos botões no display e envia os dados para a nuvem.
 *      O loop principal verifica continuamente o estado dos botões e atualiza o display e a nuvem conforme necessário.
 */
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