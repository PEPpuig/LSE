/*
 * PWM_HighMOSFETs.h
 *
 * PWM de 20 kHz para los tres MOSFET altos (AH = PB4, BH = PE5, CH = PE4),
 * con el módulo PWM0 del TM4C123.
 */

#ifndef PWM_HIGHMOSFETS_H_
#define PWM_HIGHMOSFETS_H_

#include "config_placa.h"

/* Configura el PWM0 y los pines. Las tres salidas quedan apagadas (nivel bajo). */
void configPWM_Altos(void);

/* Fija el duty (0 a DUTY_ABS_MAX) de las tres salidas. Solo se ve en la que esté activa. */
void pwmAltosFijarDuty(float duty);

/* Activa el PWM del MOSFET alto de una fase. */
void pwmAltoActivar(Fase fase);

/* Apaga los PWM altos salvo el de 'fase'. */
void pwmAltosApagarExcepto(Fase fase);

/* Apaga los tres PWM altos. */
void pwmAltosApagar(void);

#endif /* PWM_HIGHMOSFETS_H_ */
