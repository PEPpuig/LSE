/*
 * config_placa.h
 *
 * Mapa de pines y parámetros del firmware open-loop del Controlador BLDC
 * (repositorio PEPpuig/LSE, placa de la entrega 3, LaunchPad EK-TM4C123GXL = U1).
 *
 * Todo lo que depende de la placa está en este archivo: si cambia un pin o un
 * parámetro de arranque, se cambia solo aquí.
 *
 * Pines comprobados en Controlador_BLDC.kicad_pcb (pads de U1 y redes):
 *
 *   Señal    Pin Tiva  Pin LaunchPad  Destino en la placa
 *   AH       PB4       J1.7           RAH1 33R -> IC6 HIN (fase A) -> MOSFET1
 *   BH       PE5       J1.6           RBH1 33R -> IC4 HIN (fase B) -> MOSFET3
 *   CH       PE4       J1.5           RCH1 33R -> IC5 HIN (fase C) -> MOSFET5
 *   AL       PF1       J3.10          RAL1 33R -> IC6 LIN (fase A) -> MOSFET2
 *   BL       PA7       J1.10          RBL1 33R -> IC4 LIN (fase B) -> MOSFET4
 *   CL       PA6       J1.9           RCL1 33R -> IC5 LIN (fase C) -> MOSFET6
 *   DUTY     PB5/AIN11 J1.2           cursor de RPOT1 (10k) + CPOT1 100 nF
 *   SensorA  PD3       J3.6           IC3 pin 2  (LM339, pull-up 4,7k a +3V3)
 *   SensorB  PD2       J3.5           IC3 pin 1
 *   SensorC  PD1       J3.4           IC3 pin 14
 *   Neu_div  PE1/AIN2  J3.7           neutro virtual dividido (no se usa en open loop)
 */

#ifndef CONFIG_PLACA_H_
#define CONFIG_PLACA_H_

#ifndef PART_TM4C123GH6PM
#define PART_TM4C123GH6PM           /* necesario para driverlib/pin_map.h */
#endif

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/adc.h"

/* Fases del motor. Se usan como índice en todas las tablas. */
typedef enum
{
    FASE_A = 0,
    FASE_B = 1,
    FASE_C = 2
} Fase;

/* =========================================================================
 * Reloj del sistema
 * ========================================================================= */
/* Cristal de 16 MHz de la LaunchPad, PLL de 400 MHz, /2 y /4 -> 50 MHz */
#define RELOJ_SISTEMA_HZ        50000000U

/* =========================================================================
 * PWM de los MOSFET altos (módulo PWM0)
 * ========================================================================= */
#define PWM_FREQ_HZ             20000U              /* 20 kHz, como en el informe */
#define PWM_DIVISOR             SYSCTL_PWMDIV_1     /* reloj PWM = reloj del sistema */
#define PWM_RELOJ_HZ            (RELOJ_SISTEMA_HZ / 1U)   /* debe cuadrar con PWM_DIVISOR */
#define PWM_PERIODO_TICKS       (PWM_RELOJ_HZ / PWM_FREQ_HZ)  /* 2500 cuentas: 0,04 % por paso */

#if (PWM_PERIODO_TICKS > 65536U) || (PWM_PERIODO_TICKS < 100U)
#error "PWM_PERIODO_TICKS fuera de rango: revisa PWM_DIVISOR y PWM_FREQ_HZ"
#endif

/* AH: PB4 -> M0PWM2 (generador 1, salida A) */
#define AH_PERIPH_GPIO          SYSCTL_PERIPH_GPIOB
#define AH_PUERTO               GPIO_PORTB_BASE
#define AH_PIN                  GPIO_PIN_4
#define AH_PINMUX               GPIO_PB4_M0PWM2
#define AH_SALIDA_PWM           PWM_OUT_2
#define AH_BIT_PWM              PWM_OUT_2_BIT

/* BH: PE5 -> M0PWM5 (generador 2, salida B) */
#define BH_PERIPH_GPIO          SYSCTL_PERIPH_GPIOE
#define BH_PUERTO               GPIO_PORTE_BASE
#define BH_PIN                  GPIO_PIN_5
#define BH_PINMUX               GPIO_PE5_M0PWM5
#define BH_SALIDA_PWM           PWM_OUT_5
#define BH_BIT_PWM              PWM_OUT_5_BIT

/* CH: PE4 -> M0PWM4 (generador 2, salida A) */
#define CH_PERIPH_GPIO          SYSCTL_PERIPH_GPIOE
#define CH_PUERTO               GPIO_PORTE_BASE
#define CH_PIN                  GPIO_PIN_4
#define CH_PINMUX               GPIO_PE4_M0PWM4
#define CH_SALIDA_PWM           PWM_OUT_4
#define CH_BIT_PWM              PWM_OUT_4_BIT

/* AH usa el generador 1; BH y CH comparten el generador 2 (mismo periodo) */
#define PWM_GENERADOR_AH        PWM_GEN_1
#define PWM_GENERADOR_BH_CH     PWM_GEN_2
#define PWM_BITS_GENERADORES    (PWM_GEN_1_BIT | PWM_GEN_2_BIT)
#define PWM_BITS_ALTOS          (AH_BIT_PWM | BH_BIT_PWM | CH_BIT_PWM)

/* =========================================================================
 * MOSFET bajos: GPIO normales (nivel alto = MOSFET encendido)
 * ========================================================================= */
/* AL: PF1. OJO: PF1 también enciende el LED rojo de la LaunchPad. */
#define AL_PERIPH_GPIO          SYSCTL_PERIPH_GPIOF
#define AL_PUERTO               GPIO_PORTF_BASE
#define AL_PIN                  GPIO_PIN_1

/* BL: PA7 */
#define BL_PERIPH_GPIO          SYSCTL_PERIPH_GPIOA
#define BL_PUERTO               GPIO_PORTA_BASE
#define BL_PIN                  GPIO_PIN_7

/* CL: PA6 */
#define CL_PERIPH_GPIO          SYSCTL_PERIPH_GPIOA
#define CL_PUERTO               GPIO_PORTA_BASE
#define CL_PIN                  GPIO_PIN_6

/* =========================================================================
 * Potenciómetro de consigna (duty en open loop)
 * ========================================================================= */
#define POT_PERIPH_GPIO         SYSCTL_PERIPH_GPIOB
#define POT_PUERTO              GPIO_PORTB_BASE
#define POT_PIN                 GPIO_PIN_5
#define POT_CANAL_ADC           ADC_CTL_CH11        /* PB5 = AIN11 */
#define POT_SOBREMUESTREO       16U                 /* media por hardware de 16 muestras */

/* =========================================================================
 * Salidas de los comparadores BEMF (LM339, colector abierto)
 * ========================================================================= */
#define BEMF_PERIPH_GPIO        SYSCTL_PERIPH_GPIOD
#define BEMF_PUERTO             GPIO_PORTD_BASE
#define BEMF_PIN_A              GPIO_PIN_3          /* SensorA */
#define BEMF_PIN_B              GPIO_PIN_2          /* SensorB */
#define BEMF_PIN_C              GPIO_PIN_1          /* SensorC (unido a PB7 por R10 en la LaunchPad) */
#define BEMF_PINES              (BEMF_PIN_A | BEMF_PIN_B | BEMF_PIN_C)

/* En la placa la BEMF entra por la entrada - del LM339 y el neutro por la +,
 * al revés que en las simulaciones 07 y 08. Con 1 se invierten las tres
 * lecturas para que 1 = BEMF por encima del neutro, como en la simulación. */
#define BEMF_INVERTIR_SENSORES  1

/* =========================================================================
 * LEDs y pulsador de la LaunchPad
 * ========================================================================= */
/* El LED rojo (PF1) NO se usa como indicador porque es la señal AL. */
#define LED_PERIPH_GPIO         SYSCTL_PERIPH_GPIOF
#define LED_PUERTO              GPIO_PORTF_BASE
#define LED_AZUL                GPIO_PIN_2          /* esperando arranque */
#define LED_VERDE               GPIO_PIN_3          /* motor en marcha */
#define PULSADOR_PUERTO         GPIO_PORTF_BASE
#define PULSADOR_SW1            GPIO_PIN_4          /* activo a nivel bajo, pull-up interno */

/* =========================================================================
 * Parámetros del arranque en lazo abierto
 * Valores de partida tomados de la versión final del proyecto de referencia
 * (rampa de 10 ms a 1,2 ms por estado, duty de 0,18 a 0,25). Ajustar en el banco.
 * ========================================================================= */
#define T_PRECARGA_BOOT_MS      10U       /* los tres MOSFET bajos ON para cargar los bootstrap */
#define T_MUERTO_US             2U        /* pausa con lo saliente apagado en cada conmutación */

#define TFASE_INICIAL_US        10000U    /* duración de cada estado al empezar la rampa */
#define TFASE_FINAL_US          1200U     /* duración final: 1389 rpm con el motor del modelo (6 pares de polos) */
#define CICLOS_RAMPA            100U      /* ciclos eléctricos (6 estados cada uno) que dura la rampa */

#define DUTY_RAMPA_INICIAL      0.18f
#define DUTY_RAMPA_FINAL        0.25f

/* =========================================================================
 * Duty en régimen open loop (lo fija el potenciómetro)
 * ========================================================================= */
#define DUTY_POT_MIN            0.20f     /* potenciómetro al mínimo */
#define DUTY_POT_MAX            0.50f     /* potenciómetro al máximo: más duty solo sube la corriente */
#define DUTY_PASO_MAX           0.01f     /* cambio máximo de duty por ciclo eléctrico */
#define DUTY_ABS_MAX            0.95f     /* nunca 100 %: el bootstrap se recarga en el tiempo OFF */

/* =========================================================================
 * Arranque y paro
 * ========================================================================= */
#define ARRANQUE_CON_PULSADOR   1         /* 1: el primer arranque espera a SW1; 0: arranca solo */
#define T_ESPERA_INICIAL_MS     2000U     /* solo si ARRANQUE_CON_PULSADOR = 0 */
#define T_ESPERA_TRAS_PARO_MS   2000U     /* deja que el motor se pare antes de poder rearrancar */
#define T_CONFIRMAR_PARO_MS     20U       /* SW1 pulsado sin soltar este tiempo para parar (filtra rebotes) */

#endif /* CONFIG_PLACA_H_ */
