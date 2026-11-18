/*
 * LowMOSFETs.h
 *
 * Control de los tres MOSFET bajos con GPIO normales
 * (AL = PF1, BL = PA7, CL = PA6). Nivel alto = MOSFET encendido.
 */

#ifndef LOWMOSFETS_H_
#define LOWMOSFETS_H_

#include "config_placa.h"

/* Configura los tres pines como salidas y los deja en nivel bajo. */
void configLowMOSFETs(void);

/* Enciende el MOSFET bajo de una fase. */
void bajoActivar(Fase fase);

/* Apaga los MOSFET bajos salvo el de 'fase'. */
void bajosApagarExcepto(Fase fase);

/* Apaga los tres MOSFET bajos. */
void bajosApagar(void);

/* Enciende los tres MOSFET bajos (solo para precargar los bootstrap). */
void bajosActivarTodos(void);

#endif /* LOWMOSFETS_H_ */
