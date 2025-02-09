#include "../include/motor.h"
#include <stdio.h> // Biblioteca padrão da linguagem C
#include "pico/stdlib.h" // Subconjunto central de bibliotecas do SDK Pico
#include "hardware/irq.h" // Biblioteca para gerenciamento de interrupções
#include "hardware/pwm.h" // Biblioteca para controlar o hardware de PWM
#include "pico/time.h" // Necessário para usar get_absolute_time()


// Função de interrupção PWM
void wrapHandler() { 
    static int fade = 0; // Nível do motor
    static bool rise = true; // Flag para elevar ou reduzir o nível 
    static absolute_time_t lastUpdateTime = 0; // Marca de tempo da última atualização
    absolute_time_t currentTime = get_absolute_time(); // Tempo atual

    pwm_clear_irq(pwm_gpio_to_slice_num(LEDPin)); // Resetar o flag de interrupção

    // Controla o intervalo de atualização baseado em um tempo específico
    if (absolute_time_diff_us(lastUpdateTime, currentTime) >= 10000) { // 10 ms
        lastUpdateTime = currentTime;
        if (rise){ // Caso o motor seja elevada
            fade++; // Aumenta o nível
            if (fade > 255) { // Caso o fade seja maior que 255
                fade = 255; // Iguala fade a 255
                rise = false; // Muda o flag rise para redução do nível
            }
        }else{ // Caso a motor seja reduzida
            fade--; // Reduz o nível 
            if (fade < 0) { // Caso o fade seja menor que 0
                fade = 0; // Iguala fade a 0
                rise = true; // Muda o flag rise para elevação no nível
            }
        }
    }
    pwm_set_gpio_level(LEDPin, fade * fade); // Define o ciclo ativo (Ton) de forma quadrática, para acentuar a variação de luminosidade.
}

// Configuração do PWM
void setup_pwm() {
    gpio_set_function(LEDPin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(LEDPin);
    pwm_set_wrap(slice_num, WRAP_VALUE);
    pwm_set_clkdiv(slice_num, 124.0f);
    pwm_set_enabled(slice_num, true);
}

// Definição do duty cycle
void set_pwm_duty_cycle(uint slice_num, uint16_t duty_cycle) {
    pwm_set_gpio_level(LEDPin, duty_cycle);
}

// Movimento do servo
void move_servo(uint16_t duty_cycle) {
    uint slice_num = pwm_gpio_to_slice_num(LEDPin);
    pwm_set_enabled(slice_num, true);
    set_pwm_duty_cycle(slice_num, duty_cycle);
    sleep_ms(1000);
}

// Configuração do PWM com interrupção
uint pwm_setup_irq() {
    gpio_set_function(LEDPin, GPIO_FUNC_PWM); // Habilitar o pino GPIO como PWM
    uint sliceNum = pwm_gpio_to_slice_num(LEDPin); // Obter o canal PWM da GPIO

    pwm_clear_irq(sliceNum); // Resetar o flag de interrupção para o slice
    pwm_set_irq_enabled(sliceNum, true); // Habilitar a interrupção de PWM para um dado slice
    irq_set_exclusive_handler(PWM_IRQ_WRAP, wrapHandler); // Definir um tipo de interrupção
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // Esta interrupção (PWM_IRQ_WRAP) é gerada quando um contador de um slice atinge seu valor de wrap
    pwm_config config = pwm_get_default_config(); // Obtem a configuração padrão para o PWM
    pwm_config_set_clkdiv(&config, 5.f); // Define o divisor de clock do PWM
    pwm_init(sliceNum, &config, true); // Inicializa o PWM com as configurações do objeto

    return sliceNum;
}

