/*
 * Init_Periferics.c
 *
 * Orden de inicialización:
 *   1. Reloj del sistema a 50 MHz (PLL con el cristal de 16 MHz).
 *   2. MOSFET bajos: salidas en nivel bajo (apagados).
 *   3. MOSFET altos: primero GPIO en nivel bajo, después PWM con las salidas
 *      deshabilitadas. Así ningún driver ve una entrada indefinida.
 *   4. TIMER3 para la temporización.
 *   5. ADC del potenciómetro.
 *   6. Entradas de los comparadores BEMF (solo lectura).
 *   7. LEDs y pulsador SW1.
 */

#include "Init_Periferics.h"
#include "LowMOSFETs.h"
#include "PWM_HighMOSFETs.h"
#include "Delay.h"
#include "DutyPotenciometre.h"
#include "SensoresBEMF.h"
#include "Interfaz.h"

void initPeriferics(void)
{
    /* 1. 400 MHz (PLL) / 2 / 4 = 50 MHz */
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    /* 2 y 3. Etapa de potencia apagada */
    configLowMOSFETs();
    configPWM_Altos();

    /* 4. Temporización */
    initDelay();

    /* 5. Potenciómetro */
    initADC_Potenciometre();

    /* 6. Comparadores BEMF */
    configSensoresBEMF();

    /* 7. LEDs y pulsador de la LaunchPad */
    configInterfaz();
}
