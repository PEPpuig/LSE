/*
 * SensoresBEMF.h
 *
 * Lectura de las salidas de los comparadores BEMF (SensorA = PD3,
 * SensorB = PD2, SensorC = PD1). En open loop solo sirven para comprobar las
 * señales; en closed loop decidirán las conmutaciones.
 */

#ifndef SENSORESBEMF_H_
#define SENSORESBEMF_H_

#include "config_placa.h"

/* Configura PD1, PD2 y PD3 como entradas (el pull-up de 4,7 kΩ está en la placa). */
void configSensoresBEMF(void);

/* Devuelve el estado ABC en 3 bits: bit 2 = A, bit 1 = B, bit 0 = C.
 * 1 = BEMF de esa fase por encima del neutro (ya corregida la inversión de la placa). */
uint8_t leerSensoresBEMF(void);

#endif /* SENSORESBEMF_H_ */
