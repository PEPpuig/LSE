/*
 * PWM_HighMOSFETs.c
 *
 * El PWM solo se aplica a los MOSFET altos (como en las simulaciones):
 *   AH = PB4 = M0PWM2 (generador 1)
 *   BH = PE5 = M0PWM5 (generador 2)
 *   CH = PE4 = M0PWM4 (generador 2)
 *
 * Los generadores se configuran una sola vez al arrancar. En cada conmutación
 * solo se cambia el ancho de pulso y qué salida está habilitada. Una salida
 * deshabilitada queda en nivel bajo, así que su MOSFET alto está apagado.
 */

#include "PWM_HighMOSFETs.h"

static uint32_t bitDeFase(Fase fase)
{
    switch (fase)
    {
    case FASE_A: return AH_BIT_PWM;
    case FASE_B: return BH_BIT_PWM;
    default:     return CH_BIT_PWM;
    }
}

void configPWM_Altos(void)
{
    /* 1. Mientras se configura el PWM, los tres pines son GPIO en nivel bajo:
     *    así el IR2301 nunca ve un nivel indefinido en HIN. */
    SysCtlPeripheralEnable(AH_PERIPH_GPIO);
    SysCtlPeripheralEnable(BH_PERIPH_GPIO);
    SysCtlPeripheralEnable(CH_PERIPH_GPIO);
    while (!SysCtlPeripheralReady(AH_PERIPH_GPIO)) { }
    while (!SysCtlPeripheralReady(BH_PERIPH_GPIO)) { }
    while (!SysCtlPeripheralReady(CH_PERIPH_GPIO)) { }

    GPIOPinTypeGPIOOutput(AH_PUERTO, AH_PIN);
    GPIOPinTypeGPIOOutput(BH_PUERTO, BH_PIN);
    GPIOPinTypeGPIOOutput(CH_PUERTO, CH_PIN);
    GPIOPinWrite(AH_PUERTO, AH_PIN, 0);
    GPIOPinWrite(BH_PUERTO, BH_PIN, 0);
    GPIOPinWrite(CH_PUERTO, CH_PIN, 0);

    /* 2. Reloj del PWM = reloj del sistema (50 MHz) -> 2500 cuentas por periodo a 20 kHz */
    SysCtlPWMClockSet(PWM_DIVISOR);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0)) { }

    /* 3. Generadores 1 y 2: cuenta descendente; los cambios de duty se aplican al momento */
    PWMGenConfigure(PWM0_BASE, PWM_GENERADOR_AH,    PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM0_BASE, PWM_GENERADOR_BH_CH, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM0_BASE, PWM_GENERADOR_AH,    PWM_PERIODO_TICKS);   /* LOAD = 2499 */
    PWMGenPeriodSet(PWM0_BASE, PWM_GENERADOR_BH_CH, PWM_PERIODO_TICKS);

    /* 4. Salidas deshabilitadas (nivel bajo) antes de dar los pines al PWM */
    PWMOutputState(PWM0_BASE, PWM_BITS_ALTOS, false);
    pwmAltosFijarDuty(DUTY_RAMPA_INICIAL);

    PWMGenEnable(PWM0_BASE, PWM_GENERADOR_AH);
    PWMGenEnable(PWM0_BASE, PWM_GENERADOR_BH_CH);
    PWMSyncTimeBase(PWM0_BASE, PWM_BITS_GENERADORES);    /* los dos generadores en fase */

    /* 5. Ahora sí, los pines pasan a ser salidas del PWM */
    GPIOPinConfigure(AH_PINMUX);
    GPIOPinConfigure(BH_PINMUX);
    GPIOPinConfigure(CH_PINMUX);
    GPIOPinTypePWM(AH_PUERTO, AH_PIN);
    GPIOPinTypePWM(BH_PUERTO, BH_PIN);
    GPIOPinTypePWM(CH_PUERTO, CH_PIN);
}

void pwmAltosFijarDuty(float duty)
{
    uint32_t ancho;

    if (duty > DUTY_ABS_MAX)
    {
        duty = DUTY_ABS_MAX;
    }
    if (duty < 0.0f)
    {
        duty = 0.0f;
    }

    /* Ancho del pulso en cuentas del PWM (el driverlib exige ancho < LOAD) */
    ancho = (uint32_t)(duty * (float)PWM_PERIODO_TICKS);
    if (ancho < 1U)
    {
        ancho = 1U;
    }

    PWMPulseWidthSet(PWM0_BASE, AH_SALIDA_PWM, ancho);
    PWMPulseWidthSet(PWM0_BASE, BH_SALIDA_PWM, ancho);
    PWMPulseWidthSet(PWM0_BASE, CH_SALIDA_PWM, ancho);
}

void pwmAltoActivar(Fase fase)
{
    PWMOutputState(PWM0_BASE, bitDeFase(fase), true);
}

void pwmAltosApagarExcepto(Fase fase)
{
    PWMOutputState(PWM0_BASE, PWM_BITS_ALTOS & ~bitDeFase(fase), false);
}

void pwmAltosApagar(void)
{
    PWMOutputState(PWM0_BASE, PWM_BITS_ALTOS, false);
}
