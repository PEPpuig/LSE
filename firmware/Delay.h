/*
 * Delay.h
 *
 * Temporización con el TIMER3 (32 bits, una sola cuenta, por sondeo).
 * Sin interrupciones: el firmware open loop no usa ninguna.
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "config_placa.h"

/* Configura el TIMER3. Llamar una vez desde initPeriferics(). */
void initDelay(void);

/* Arranca una cuenta de 'us' microsegundos y vuelve enseguida. */
void temporizadorArrancarUs(uint32_t us);

/* true si la cuenta arrancada con temporizadorArrancarUs() ya ha terminado. */
bool temporizadorTerminado(void);

/* Espera a que termine la cuenta arrancada con temporizadorArrancarUs(). */
void temporizadorEsperarFin(void);

/* Retardos bloqueantes. Usan el mismo TIMER3: no mezclar con una cuenta en marcha. */
void delayUs(uint32_t us);
void delayMs(uint32_t ms);

/* Retardo corto aproximado (bucle de CPU), para el tiempo muerto. */
void retardoCortoUs(uint32_t us);

#endif /* DELAY_H_ */
