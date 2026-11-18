/*
 * Conmutacion.h
 *
 * Tabla de los 6 estados de la conmutación trapezoidal (6-step) y función
 * que aplica un estado al puente. Se reutilizará tal cual en el closed loop.
 */

#ifndef CONMUTACION_H_
#define CONMUTACION_H_

#include "config_placa.h"

#define NUM_ESTADOS     6U

/* Estado aplicado ahora mismo (0..5), visible desde el depurador. */
extern volatile uint8_t g_estadoPuente;

/* Aplica el estado 'estado' (0..5) con el duty indicado en el MOSFET alto. */
void conmutarEstado(uint8_t estado, float duty);

/* Apaga los seis MOSFET: el motor queda en rueda libre. */
void conmutacionApagarTodo(void);

#endif /* CONMUTACION_H_ */
