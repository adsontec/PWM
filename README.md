## Servomotor - PWM 💹
Este projeto demonstra como usar o Raspberry Pi Pico W para girar um servomotor usando PWM(Modulação por Largura de Pulso) em 180 Graus, 90 Graus e 0 Graus, além de criar uma rotina para movimentação periódica do braço do servomotor entre os ângulos de 0 e 180 graus, consolidando os conceitos sobre ciclo de trabalho(Duty Cycle) e interrupção vinculada ao período (Wrap).
## Funcionalidade: 📊
Ao iniciar o programa o servomotor vai girar em uma ángulo de 180 Graus, depois 90 graus, em seguinda 0 Graus e por último foi criado uma função de interrupção do PWM que permite o braço do servomotor gira os ângulos de 0 e 180 graus indefinidamente com o uso da função **get_absolute_time()** permite obter o tempo absoluto do microcontrolador gerando um atraso de 10ms na rotação do motor.
## Requisitos de Hardware: 👨‍🔧
- Microcontrolador Raspberry Pi Pico W
- Servomotor
## Requisitos de Software: ☑️
- Instalar o Visual Studio Code
- Instalar a extensão Wokwi no VSCode
- Instalar os pacotes do Pico SDK
## Pinos GPIO ⏰
- Servomotor: GPIO 22
- LED (RGB): GPIO 12
## Explicação do Código: 🕵️
- **stdio_init_all()**: Inicializar comunicação padrão
- **setup_pwm()**: Configura o PWM
- **wrapHandler()**: Função de interrupção PWM
- **set_pwm_duty_cycle(uint slice_num, uint16_t duty_cycle)**: Definição do duty cycle
- **move_servo(uint16_t duty_cycle)**: Movimento do servomotor

## Cálculo da Frequência do PWM
# ⚫ Wrap
# $f_{\text{PWM}} = \frac{125 \times 10^6}{\left( 1 + \frac{0}{16} \right) \times 2500 \times 10^3}$ ➡️ $f_{\text{PWM}} = 50 Hz$
# $f_{\text{TPWM}} = \frac{1}{50}$ ➡️ $f_{\text{TPWM}} = 20 ms$

# ⚫ Duty Cycle 180
# $D_{\text{C}} = \frac{2400}{20000}$ ➡️ $D_{\text{C}} = 0,12$

# ⚫ Duty Cycle 90
# $D_{\text{C}} = \frac{1470}{20000}$ ➡️ $D_{\text{C}} = 0,0735$

# ⚫ Duty Cycle 0
# $D_{\text{C}} = \frac{500}{20000}$ ➡️ $D_{\text{C}} = 0,025$

## Link com a produção do vídeo 📀


## Imagem do Projeto ✔️
![image](https://github.com/user-attachments/assets/85a31dd9-7946-401b-b1aa-aead5a62eadc)


