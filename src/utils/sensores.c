#include "sensores.h"

void inicializar_botoes()
{
    gpio_init(PIN_BOTAO_A);
    gpio_set_dir(PIN_BOTAO_A, GPIO_IN);
    gpio_pull_up(PIN_BOTAO_A);

    gpio_init(PIN_BOTAO_B);
    gpio_set_dir(PIN_BOTAO_B, GPIO_IN);
    gpio_pull_up(PIN_BOTAO_B);

}

bool botao_a_pressionado()
{
    return !gpio_get(PIN_BOTAO_A);
}

bool botao_b_pressionado()
{
    return !gpio_get(PIN_BOTAO_B);
}