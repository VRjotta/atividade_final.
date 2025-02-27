#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define BUZZER1_PIN 10
#define BUZZER2_PIN 21
#define BUTTON_ALARM 6
#define BUTTON_RESET 5
#define LED_R 13
#define LED_G 11
#define LED_B 12

void set_led_color(bool r, bool g, bool b) {
    gpio_put(LED_R, r);
    gpio_put(LED_G, g);
    gpio_put(LED_B, b);
}

// Função para fazer o buzzer tocar alternando frequências
void sirene() {
    for (int i = 0; i < 3; i++) { // Repete o som 3 vezes
        gpio_put(BUZZER1_PIN, 1);
        gpio_put(BUZZER2_PIN, 0);
        sleep_ms(200);
        gpio_put(BUZZER1_PIN, 0);
        gpio_put(BUZZER2_PIN, 1);
        sleep_ms(200);
    }
    gpio_put(BUZZER1_PIN, 0);
    gpio_put(BUZZER2_PIN, 0);
}

int main() {
    // Inicializa GPIOs dos LEDs
    gpio_init(LED_R);
    gpio_init(LED_G);
    gpio_init(LED_B);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_set_dir(LED_B, GPIO_OUT);

    // Inicializa GPIOs dos buzzers
    gpio_init(BUZZER1_PIN);
    gpio_init(BUZZER2_PIN);
    gpio_set_dir(BUZZER1_PIN, GPIO_OUT);
    gpio_set_dir(BUZZER2_PIN, GPIO_OUT);

    // Inicializa GPIOs dos botões
    gpio_init(BUTTON_ALARM);
    gpio_init(BUTTON_RESET);
    gpio_set_dir(BUTTON_ALARM, GPIO_IN);
    gpio_set_dir(BUTTON_RESET, GPIO_IN);
    gpio_pull_up(BUTTON_ALARM);
    gpio_pull_up(BUTTON_RESET);

    bool alarm_on = false;

    // Começa com LED verde ligado (sistema normal)
    set_led_color(0, 1, 0);

    while (true) {
        if (!gpio_get(BUTTON_ALARM)) {  // Pressionado = 0
            alarm_on = true;
            sleep_ms(200);
        }
        if (!gpio_get(BUTTON_RESET)) {  // Pressionado = 0
            alarm_on = false;
            sleep_ms(200);
        }

        if (alarm_on) {
            // Alarme ativado: LED vermelho e azul piscando e sirene tocando
            set_led_color(1, 0, 0);
            sirene();
            set_led_color(0, 0, 1);
            sirene();
            set_led_color(1, 0, 0);
            sirene();
            sleep_ms(150);
            set_led_color(0, 0, 0);
            sleep_ms(150);
        } else {
            // Sistema normal: LED verde fixo
            set_led_color(0, 1, 0);
            gpio_put(BUZZER1_PIN, 0);
            gpio_put(BUZZER2_PIN, 0);
        }
    }

    return 0;
}
