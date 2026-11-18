/*
 * Interfaz.h
 *
 * LEDs y pulsador de la LaunchPad: LED azul (PF2), LED verde (PF3) y SW1 (PF4).
 * El LED rojo (PF1) no se puede usar: en esta placa es la señal AL.
 */

#ifndef INTERFAZ_H_
#define INTERFAZ_H_

#include "config_placa.h"

/* Configura PF2 y PF3 como salidas y PF4 como entrada con pull-up. */
void configInterfaz(void);

/* Enciende o apaga los LED azul y verde. */
void ledsFijar(bool azul, bool verde);

/* true mientras SW1 está pulsado (lectura directa, sin filtro). */
bool pulsadorSW1Pulsado(void);

/* Espera a que SW1 se pulse de forma estable y se suelte. */
void esperarPulsacionSW1(void);

/* Espera a que SW1 esté suelto de forma estable. */
void esperarSueltaSW1(void);

#endif /* INTERFAZ_H_ */
