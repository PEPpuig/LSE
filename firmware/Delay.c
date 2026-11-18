/*
 * Delay.c
 *
 * Temporización con el TIMER3 en modo "one-shot" de 32 bits: cuenta hacia
 * abajo desde el valor cargado y se para solo al llegar a 0, activando el
 * flag de time-out. Se consulta el flag por sondeo (sin interrupción).
 *
 * A 50 MHz cada microsegundo son 50 cuentas; el máximo son unos 85 s.
 */

#include "Delay.h"
#include "driverlib/timer.h"

#define TICKS_POR_US    (RELOJ_SISTEMA_HZ / 1000000U)   /* 50 */

void initDelay(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER3))
    {
    }

    TimerDisable(TIMER3_BASE, TIMER_A);
    TimerConfigure(TIMER3_BASE, TIMER_CFG_ONE_SHOT);     /* 32 bits, cuenta atrás, una vez */
    TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
}

void temporizadorArrancarUs(uint32_t us)
{
    uint32_t ticks = us * TICKS_POR_US;

    if (ticks == 0U)
    {
        ticks = 1U;                                      /* evita cargar 0 - 1 = 0xFFFFFFFF */
    }

    TimerDisable(TIMER3_BASE, TIMER_A);
    TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);      /* borra un time-out anterior */
    TimerLoadSet(TIMER3_BASE, TIMER_A, ticks - 1U);
    TimerEnable(TIMER3_BASE, TIMER_A);
}

bool temporizadorTerminado(void)
{
    return (TimerIntStatus(TIMER3_BASE, false) & TIMER_TIMA_TIMEOUT) != 0U;
}

void temporizadorEsperarFin(void)
{
    while (!temporizadorTerminado())
    {
    }
    TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
}

void delayUs(uint32_t us)
{
    temporizadorArrancarUs(us);
    temporizadorEsperarFin();
}

void delayMs(uint32_t ms)
{
    while (ms > 0U)
    {
        delayUs(1000U);
        ms--;
    }
}

void retardoCortoUs(uint32_t us)
{
    /* SysCtlDelay() tarda 3 ciclos de reloj por vuelta: a 50 MHz, unas 16 vueltas por µs.
     * Con 0 vueltas daría 2^32 vueltas, por eso se sale antes. */
    if (us == 0U)
    {
        return;
    }
    SysCtlDelay((RELOJ_SISTEMA_HZ / 3000000U) * us);
}
