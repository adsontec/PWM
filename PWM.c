#include <stdio.h> // Biblioteca padrão da linguagem C
#include "pico/stdlib.h" // Subconjunto central de bibliotecas do SDK Pico
#include "hardware/irq.h" // Biblioteca para gerenciamento de interrupções
#include "hardware/pwm.h" // Biblioteca para controlar o hardware de PWM
#include "pico/time.h" // Necessário para usar get_absolute_time()
#include "./includes/motor.h"

#define PULSE_MAX 2400  // Pulso para 180 graus (2400us) - Duty Cycle - 0,12%
#define PULSE_90 1470   // Pulso para 90 graus (1470us) - Duty Cycle - 0,0735%
#define PULSE_MIN 500   // Pulso para 0 graus (500us) - Duty Cycle - 0,025%


int main() {
    stdio_init_all(); // Inicializa o sistema padrão de I/O

    setup_pwm(); // Configura o PWM ANTES de mover o servo

    move_servo(PULSE_MAX); // 180°
    move_servo(PULSE_90);  // 90°
    move_servo(PULSE_MIN); // 0°

    pwm_setup_irq(); // Agora inicializa a interrupção

    while (1) {
        
    }
}
