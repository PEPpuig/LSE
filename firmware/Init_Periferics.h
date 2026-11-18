/*
 * Init_Periferics.h
 *
 * Inicialización de todo el hardware que usa el firmware open loop.
 */

#ifndef INIT_PERIFERICS_H_
#define INIT_PERIFERICS_H_

#include "config_placa.h"

/* Reloj a 50 MHz, pines de los drivers en estado seguro, PWM, TIMER3, ADC,
 * entradas BEMF, LEDs y pulsador. Al salir, los seis MOSFET están apagados. */
void initPeriferics(void);

#endif /* INIT_PERIFERICS_H_ */
