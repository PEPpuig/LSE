/*
 * OpenLoop.h
 *
 * Arranque y funcionamiento en lazo abierto: la conmutación la marca el
 * tiempo, no la BEMF.
 */

#ifndef OPENLOOP_H_
#define OPENLOOP_H_

#include "config_placa.h"

/* Variables para observar desde el depurador (Expressions de CCS) */
extern volatile uint32_t g_tfaseOL_us;          /* duración actual de cada estado */
extern volatile float    g_dutyOL;              /* duty aplicado ahora */
extern volatile uint32_t g_ciclosOL;            /* ciclos eléctricos completados */
extern volatile uint8_t  g_bemfPorEstado[6];    /* SensorA/B/C (bits ABC) al final de cada estado */

/* Enciende los tres MOSFET bajos T_PRECARGA_BOOT_MS para cargar los condensadores de bootstrap. */
void precargarBootstrap(void);

/* Rampa de arranque. Devuelve false si se ha pedido paro con SW1. */
bool rampaArrencada(void);

/* Mantiene el motor en open loop con TFASE_FINAL_US y el duty del potenciómetro.
 * Vuelve cuando se pulsa SW1. */
void maintainOpenLoop(void);

#endif /* OPENLOOP_H_ */
