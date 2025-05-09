#ifndef BOTOES_H
#define BOTOES_H

// -- INCLUDES --
#include "pico/stdlib.h"

// -- DEFINES --
#define PIN_BOTAO_A 5
#define PIN_BOTAO_B 6

void inicializar_botoes();
bool botao_a_pressionado();
bool botao_b_pressionado();

#endif