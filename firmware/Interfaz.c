/*
 * Interfaz.c
 *
 * El pulsador SW1 de la LaunchPad conecta PF4 a GND al pulsarlo y no tiene
 * resistencia externa, así que se activa el pull-up interno.
 * Los LED de la LaunchPad se encienden con nivel alto.
 */

#include "Interfaz.h"
#include "Delay.h"

#define LECTURAS_ESTABLES   3U      /* 3 lecturas iguales separadas 10 ms */
#define T_LECTURA_MS        10U

void configInterfaz(void)
{
    SysCtlPeripheralEnable(LED_PERIPH_GPIO);
    while (!SysCtlPeripheralReady(LED_PERIPH_GPIO)) { }

    GPIOPinTypeGPIOOutput(LED_PUERTO, LED_AZUL | LED_VERDE);
    ledsFijar(false, false);

    GPIOPinTypeGPIOInput(PULSADOR_PUERTO, PULSADOR_SW1);
    GPIOPadConfigSet(PULSADOR_PUERTO, PULSADOR_SW1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

void ledsFijar(bool azul, bool verde)
{
    GPIOPinWrite(LED_PUERTO, LED_AZUL | LED_VERDE,
                 (azul ? LED_AZUL : 0U) | (verde ? LED_VERDE : 0U));
}

bool pulsadorSW1Pulsado(void)
{
    return GPIOPinRead(PULSADOR_PUERTO, PULSADOR_SW1) == 0;   /* activo a nivel bajo */
}

void esperarPulsacionSW1(void)
{
    uint8_t seguidas = 0U;

    while (seguidas < LECTURAS_ESTABLES)
    {
        delayMs(T_LECTURA_MS);
        seguidas = pulsadorSW1Pulsado() ? (uint8_t)(seguidas + 1U) : 0U;
    }
    esperarSueltaSW1();
}

void esperarSueltaSW1(void)
{
    uint8_t seguidas = 0U;

    while (seguidas < LECTURAS_ESTABLES)
    {
        delayMs(T_LECTURA_MS);
        seguidas = pulsadorSW1Pulsado() ? 0U : (uint8_t)(seguidas + 1U);
    }
}
