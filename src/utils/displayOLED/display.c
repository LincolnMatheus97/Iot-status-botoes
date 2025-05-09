#include "display.h"
#include "ssd1306.h"


ssd1306_t display; 


void inic_barr_i2c()
{
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}


void inic_display()
{
    ssd1306_init(&display, 128, 64, ADD_DISPLAY, I2C_PORT);
}


void escrever_display(const char* texto, uint32_t larg_texto, uint32_t alt_texto, uint32_t tam_font)
{
    ssd1306_draw_string(&display, larg_texto, alt_texto, tam_font, texto);
}


void mostrar_display()
{
    ssd1306_show(&display);
}


void limpar_display()
{
    ssd1306_clear(&display);
}