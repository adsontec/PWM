#include <stdio.h> // Biblioteca padrão da linguagem C
#include "pico/stdlib.h" // Subconjunto central de bibliotecas do SDK Pico
#include "hardware/pwm.h" // Biblioteca para controlar o hardware de PWM

#define PWM_MOTOR 22 // Pino do servo motor conectado a GPIO como PWM
#define PWM_WRAP 20000 // Valor máximo do contador - WRAP

// Função para configurar o módulo PWM
void setup_pwm() {
    gpio_set_function(PWM_MOTOR, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PWM_MOTOR);
    pwm_set_wrap(slice_num, PWM_WRAP);
    pwm_set_clkdiv(slice_num, 125.0f);
    pwm_set_enabled(slice_num, true);
}

// Função do ciclo ativo
void set_pwm_duty_cycle(uint slice_num, uint16_t duty_cycle) {
    pwm_set_gpio_level(PWM_MOTOR, duty_cycle);
}

// Função para move o servo motor
void move_servo(uint16_t duty_cycle) {
    uint slice_num = pwm_gpio_to_slice_num(PWM_MOTOR);
    set_pwm_duty_cycle(slice_num, duty_cycle);
    sleep_ms(1000);
}

// Rotina principal
int main(){
    stdio_init_all(); // Inicializa o sistema padrão de I/O
    setup_pwm(); // Configura o PWM

    move_servo(2400); // 180°
    move_servo(1470); // 90°
    move_servo(500);  // 0°

    // Loop principal
    while (1) {
        /*
        for (uint16_t duty_cycle = 500; duty_cycle <= 2400; duty_cycle += 5) {
            set_pwm_duty_cycle(pwm_gpio_to_slice_num(PWM_MOTOR), duty_cycle);
            sleep_ms(10);
        }
        for (uint16_t duty_cycle = 2400; duty_cycle >= 500; duty_cycle -= 5) {
            set_pwm_duty_cycle(pwm_gpio_to_slice_num(PWM_MOTOR), duty_cycle);
            sleep_ms(10);
        }
        */
    }
    return 0;
}
