#ifndef DISPLAY_H
#define DISPLAY_H

// -- INCLUDES --
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// -- DEFINES DO DISPLAY --
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ADD_DISPLAY 0X3C

void inic_barr_i2c();
void inic_display();
void escrever_display(const char* texto, uint32_t larg_texto, uint32_t alt_texto, uint32_t tam_font);
void mostrar_display();
void limpar_display();

#endif