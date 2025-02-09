
#ifndef MOTOR_H
#define MOTOR_H

#include "pico/stdlib.h"

#define LEDPin 22 // Pino do LED conectado a GPIO como PWM
#define WRAP_VALUE 25000 // Período de 20ms (50Hz) - 20 ms

void wrapHandler();
void setup_pwm();
void set_pwm_duty_cycle(uint slice_num, uint16_t duty_cycle);
void move_servo(uint16_t duty_cycle);
uint pwm_setup_irq();

#endif
