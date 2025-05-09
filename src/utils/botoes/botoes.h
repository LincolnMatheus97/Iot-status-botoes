#ifndef BOTOES_H
#define BOTOES_H

// --- Inclui as bibliotecas necessárias ---
#include "pico/stdlib.h"

// --- Define os pinos dos botões ---
#define PIN_BOTAO_A 5
#define PIN_BOTAO_B 6

void inicializar_botoes();
bool botao_a_pressionado();
bool botao_b_pressionado();

#endif