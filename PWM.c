#include <stdio.h> // Biblioteca padrão da linguagem C
#include "pico/stdlib.h" // Subconjunto central de bibliotecas do SDK Pico
#include "hardware/irq.h" // Biblioteca para gerenciamento de interrupções
#include "hardware/pwm.h" // Biblioteca para controlar o hardware de PWM
#include "pico/time.h" // Necessário para usar get_absolute_time()

#define GPIO_MOTOR 22 // Pino do motor conectado a GPIO como PWM
#define WRAP_VALUE 25000 // Período de 20ms (50Hz) - 20 ms

#define DC_180 2400  // 180 graus (2400us) - Duty Cycle - 0,12%
#define DC_90 1470  // 90 graus (1470us) - Duty Cycle - 0,0735%
#define DC_0 500   // 0 graus (500us) - Duty Cycle - 0,025%

// Função de interrupção PWM
void wrapHandler() { 
    static int fade = 0; // Nível do motor
    static bool rise = true; // Flag para elevar ou reduzir o nível 
    static absolute_time_t lastUpdateTime = 0; // Marca de tempo da última atualização
    absolute_time_t currentTime = get_absolute_time(); // Tempo atual

    pwm_clear_irq(pwm_gpio_to_slice_num(GPIO_MOTOR)); // Resetar o flag de interrupção

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
    pwm_set_gpio_level(GPIO_MOTOR, fade * fade); // Define o ciclo ativo (Ton) de forma quadrática, para acentuar a variação de luminosidade.
}

// Configuração do PWM
void setup_pwm() {
    gpio_set_function(GPIO_MOTOR, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(GPIO_MOTOR);
    pwm_set_wrap(slice_num, WRAP_VALUE);
    pwm_set_clkdiv(slice_num, 124.0f);
    pwm_set_enabled(slice_num, true);
}

// Definição do duty cycle
void set_pwm_duty_cycle(uint slice_num, uint16_t duty_cycle) {
    pwm_set_gpio_level(GPIO_MOTOR, duty_cycle);
}

// Movimento do servo
void move_servo(uint16_t duty_cycle) {
    uint slice_num = pwm_gpio_to_slice_num(GPIO_MOTOR);
    pwm_set_enabled(slice_num, true);
    set_pwm_duty_cycle(slice_num, duty_cycle);
    sleep_ms(1000);
}

// Configuração do PWM com interrupção
uint pwm_setup_irq() {
    gpio_set_function(GPIO_MOTOR, GPIO_FUNC_PWM); // Habilitar o pino GPIO como PWM
    uint sliceNum = pwm_gpio_to_slice_num(GPIO_MOTOR); // Obter o canal PWM da GPIO

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

int main() {
    stdio_init_all(); // Inicializa o sistema padrão de I/O

    setup_pwm(); // Configura o PWM ANTES de mover o servo

    move_servo(DC_180); // 180°
    move_servo(DC_90);  // 90°
    move_servo(DC_0);   // 0°

    pwm_setup_irq(); // Agora inicializa a interrupção

    while (1) {
        
    }
}
