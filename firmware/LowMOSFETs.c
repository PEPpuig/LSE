/*
 * LowMOSFETs.c
 *
 * Los MOSFET bajos no llevan PWM: en cada estado de la secuencia, el bajo
 * activo está encendido todo el estado.
 *
 *   AL = PF1 -> IC6 LIN (fase A)  (también enciende el LED rojo de la LaunchPad)
 *   BL = PA7 -> IC4 LIN (fase B)
 *   CL = PA6 -> IC5 LIN (fase C)
 */

#include "LowMOSFETs.h"

static void escribirBajo(Fase fase, bool encendido)
{
    switch (fase)
    {
    case FASE_A:
        GPIOPinWrite(AL_PUERTO, AL_PIN, encendido ? AL_PIN : 0);
        break;
    case FASE_B:
        GPIOPinWrite(BL_PUERTO, BL_PIN, encendido ? BL_PIN : 0);
        break;
    default:
        GPIOPinWrite(CL_PUERTO, CL_PIN, encendido ? CL_PIN : 0);
        break;
    }
}

void configLowMOSFETs(void)
{
    SysCtlPeripheralEnable(AL_PERIPH_GPIO);
    SysCtlPeripheralEnable(BL_PERIPH_GPIO);
    SysCtlPeripheralEnable(CL_PERIPH_GPIO);
    while (!SysCtlPeripheralReady(AL_PERIPH_GPIO)) { }
    while (!SysCtlPeripheralReady(BL_PERIPH_GPIO)) { }
    while (!SysCtlPeripheralReady(CL_PERIPH_GPIO)) { }

    GPIOPinTypeGPIOOutput(AL_PUERTO, AL_PIN);
    GPIOPinTypeGPIOOutput(BL_PUERTO, BL_PIN);
    GPIOPinTypeGPIOOutput(CL_PUERTO, CL_PIN);

    bajosApagar();
}

void bajoActivar(Fase fase)
{
    escribirBajo(fase, true);
}

void bajosApagarExcepto(Fase fase)
{
    if (fase != FASE_A) { escribirBajo(FASE_A, false); }
    if (fase != FASE_B) { escribirBajo(FASE_B, false); }
    if (fase != FASE_C) { escribirBajo(FASE_C, false); }
}

void bajosApagar(void)
{
    escribirBajo(FASE_A, false);
    escribirBajo(FASE_B, false);
    escribirBajo(FASE_C, false);
}

void bajosActivarTodos(void)
{
    escribirBajo(FASE_A, true);
    escribirBajo(FASE_B, true);
    escribirBajo(FASE_C, true);
}
