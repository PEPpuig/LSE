/*
 * main.c
 *
 * Firmware open loop del Controlador BLDC (placa PEPpuig/LSE, LaunchPad EK-TM4C123GXL).
 * Basado en "open-loop-baseline" del proyecto de referencia y adaptado a esta placa.
 *
 * Funcionamiento:
 *   - LED azul: esperando. Pulsar SW1 para arrancar.
 *   - LED verde: motor en marcha. Precarga de bootstrap, rampa de arranque y
 *     open loop con el duty del potenciómetro.
 *   - Pulsar SW1 (unos 20 ms) para parar: se apagan los seis MOSFET y el motor
 *     queda en rueda libre. Tras T_ESPERA_TRAS_PARO_MS se puede volver a arrancar.
 *
 * No se usan interrupciones: todo se hace por sondeo en este bucle.
 */

#include "config_placa.h"
#include "Init_Periferics.h"
#include "Conmutacion.h"
#include "OpenLoop.h"
#include "Interfaz.h"
#include "Delay.h"

int main(void)
{
    bool primeraVez = true;

    initPeriferics();
    conmutacionApagarTodo();

    while (1)
    {
        /* ---- Espera de arranque ---- */
        ledsFijar(true, false);                         /* azul */

        if (primeraVez && (ARRANQUE_CON_PULSADOR == 0))
        {
            delayMs(T_ESPERA_INICIAL_MS);
        }
        else
        {
            esperarPulsacionSW1();
        }
        primeraVez = false;

        /* ---- Arranque en lazo abierto ---- */
        ledsFijar(false, true);                         /* verde */

        precargarBootstrap();
        if (rampaArrencada())
        {
            maintainOpenLoop();                         /* vuelve al pulsar SW1 */
        }

        /* ---- Paro ---- */
        conmutacionApagarTodo();                        /* rueda libre */
        ledsFijar(false, false);
        esperarSueltaSW1();
        delayMs(T_ESPERA_TRAS_PARO_MS);
    }
}
