/*
 * Conmutacion.c
 *
 * Secuencia de 6 estados. Misma secuencia y numeración que las fuentes de
 * tiempo de la simulación 08_closed_loop_placa (HA_OL..LC_OL), así las formas
 * de onda medidas se pueden comparar directamente con las simuladas:
 *
 *   estado   alto (PWM)   bajo (ON)
 *     0         AH           BL
 *     1         AH           CL
 *     2         BH           CL
 *     3         BH           AL
 *     4         CH           AL
 *     5         CH           BL
 *
 * Cada fase pasa por alto (2 estados) -> flotante (1) -> bajo (2) -> flotante (1),
 * así que nunca se enciende el alto y el bajo de la misma fase a la vez.
 * El IR2301 no tiene tiempo muerto ni bloqueo interno, por eso además se
 * apaga primero lo saliente, se espera T_MUERTO_US y luego se enciende lo nuevo.
 */

#include "Conmutacion.h"
#include "PWM_HighMOSFETs.h"
#include "LowMOSFETs.h"
#include "Delay.h"

typedef struct
{
    Fase alta;      /* fase con el MOSFET alto en PWM */
    Fase baja;      /* fase con el MOSFET bajo encendido */
} EstadoPuente;

static const EstadoPuente kTablaEstados[NUM_ESTADOS] =
{
    { FASE_A, FASE_B },     /* 0: AH - BL */
    { FASE_A, FASE_C },     /* 1: AH - CL */
    { FASE_B, FASE_C },     /* 2: BH - CL */
    { FASE_B, FASE_A },     /* 3: BH - AL */
    { FASE_C, FASE_A },     /* 4: CH - AL */
    { FASE_C, FASE_B },     /* 5: CH - BL */
};

volatile uint8_t g_estadoPuente = 0U;

void conmutarEstado(uint8_t estado, float duty)
{
    const EstadoPuente *e = &kTablaEstados[estado % NUM_ESTADOS];

    pwmAltosFijarDuty(duty);

    /* 1. Apagar lo que no pertenece al nuevo estado */
    pwmAltosApagarExcepto(e->alta);
    bajosApagarExcepto(e->baja);

    /* 2. Tiempo muerto */
    retardoCortoUs(T_MUERTO_US);

    /* 3. Encender el nuevo par (el que ya estaba encendido sigue igual) */
    bajoActivar(e->baja);
    pwmAltoActivar(e->alta);

    g_estadoPuente = (uint8_t)(estado % NUM_ESTADOS);
}

void conmutacionApagarTodo(void)
{
    pwmAltosApagar();
    bajosApagar();
}
