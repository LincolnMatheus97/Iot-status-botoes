#include "botoes.h"

/*
* Função para inicializar os botões.
* Configura os pinos dos botões como entradas e ativa o pull-up interno.
*/
void inicializar_botoes()
{
    gpio_init(PIN_BOTAO_A);
    gpio_set_dir(PIN_BOTAO_A, GPIO_IN);
    gpio_pull_up(PIN_BOTAO_A);

    gpio_init(PIN_BOTAO_B);
    gpio_set_dir(PIN_BOTAO_B, GPIO_IN);
    gpio_pull_up(PIN_BOTAO_B);

}

/*
* Função para verificar se o botão A está pressionado.
* @return true se o botão A estiver pressionado, false caso contrário.
*/
bool botao_a_pressionado()
{
    return !gpio_get(PIN_BOTAO_A);
}

/*
* Função para verificar se o botão B está pressionado.
* @return true se o botão B estiver pressionado, false caso contrário.
*/
bool botao_b_pressionado()
{
    return !gpio_get(PIN_BOTAO_B);
}