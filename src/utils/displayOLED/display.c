#include "display.h"
#include "ssd1306.h"

// Instância do display
ssd1306_t display; 

/**
 * @brief Inicializa a comunicação I2C para o display OLED.
 * @note Configura os pinos SDA e SCL para a função I2C e ativa os resistores de pull-up.
 *       Define a velocidade de comunicação para 400kHz.
 */
void inic_barr_i2c()
{
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

/**
 * @brief Inicializa o display OLED.
 * @note Configura o tamanho do display, endereço I2C e a instância I2C.
 *       Chama a função de inicialização do display da biblioteca SSD1306.
 */
void inic_display()
{
    ssd1306_init(&display, 128, 64, ADD_DISPLAY, I2C_PORT);
}

/**
 * @brief Escreve um texto no display OLED.
 * @param texto Texto a ser exibido.
 * @param larg_texto Posição X do texto no display.
 * @param alt_texto Posição Y do texto no display.
 * @param tam_font Tamanho da fonte (escala).
 * @note Chama a função de desenho de string da biblioteca SSD1306 para exibir o texto.
 */
void escrever_display(const char* texto, uint32_t larg_texto, uint32_t alt_texto, uint32_t tam_font)
{
    ssd1306_draw_string(&display, larg_texto, alt_texto, tam_font, texto);
}

/**
 * @brief Mostra o conteúdo do buffer no display OLED.
 * @note Chama a função de exibição da biblioteca SSD1306 para atualizar o display.
 */
void mostrar_display()
{
    ssd1306_show(&display);
}

/**
 * @brief Limpa o buffer do display OLED.
 * @note Chama a função de limpeza da biblioteca SSD1306 para limpar o display.
 */
void limpar_display()
{
    ssd1306_clear(&display);
}