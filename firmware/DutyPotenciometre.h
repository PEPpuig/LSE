/*
 * DutyPotenciometre.h
 *
 * Lectura del potenciómetro de consigna (PB5 = AIN11) con el ADC0.
 */

#ifndef DUTYPOTENCIOMETRE_H_
#define DUTYPOTENCIOMETRE_H_

#include "config_placa.h"

/* Configura el ADC0 (secuenciador 3, disparo por software) y el pin PB5. */
void initADC_Potenciometre(void);

/* Lectura cruda del ADC: 0 a 4095 (0 a 3,3 V). */
uint16_t llegirADC_Potenciometre(void);

/* Duty de open loop según el potenciómetro: de DUTY_POT_MIN a DUTY_POT_MAX. */
float llegirDutyPotenciometre(void);

#endif /* DUTYPOTENCIOMETRE_H_ */
