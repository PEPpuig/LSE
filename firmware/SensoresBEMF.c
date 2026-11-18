/*
 * SensoresBEMF.c
 *
 * Las salidas del LM339 son de colector abierto con pull-up de 4,7 kΩ a +3V3
 * (RPull1..3), así que llegan a la Tiva como niveles de 0 / 3,3 V.
 *
 * En la placa, la BEMF de cada fase entra por la entrada - del comparador y
 * el neutro dividido por la +. Por eso la salida vale 1 cuando la BEMF está
 * POR DEBAJO del neutro: al revés que en las simulaciones LTspice 07 y 08.
 * Con BEMF_INVERTIR_SENSORES = 1 se invierten los tres bits al leerlos.
 *
 * PD1 está unido a PB7 por una resistencia de 0 Ω de la LaunchPad (R10):
 * PB7 no se configura y se queda como entrada (estado de reset).
 */

#include "SensoresBEMF.h"

void configSensoresBEMF(void)
{
    SysCtlPeripheralEnable(BEMF_PERIPH_GPIO);
    while (!SysCtlPeripheralReady(BEMF_PERIPH_GPIO)) { }

    GPIOPinTypeGPIOInput(BEMF_PUERTO, BEMF_PINES);
}

uint8_t leerSensoresBEMF(void)
{
    uint32_t lectura = (uint32_t)GPIOPinRead(BEMF_PUERTO, BEMF_PINES);

#if BEMF_INVERTIR_SENSORES
    lectura ^= BEMF_PINES;                  /* deshace la inversión de la placa */
#endif

    uint8_t a = ((lectura & BEMF_PIN_A) != 0U) ? 1U : 0U;
    uint8_t b = ((lectura & BEMF_PIN_B) != 0U) ? 1U : 0U;
    uint8_t c = ((lectura & BEMF_PIN_C) != 0U) ? 1U : 0U;

    return (uint8_t)((a << 2) | (b << 1) | c);
}
