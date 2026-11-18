/*
 * OpenLoop.c
 *
 * 1. precargarBootstrap(): los tres MOSFET bajos ON unos ms. Cada fase queda a
 *    GND y su condensador de bootstrap (1 µF) se carga a ~12 V a través de su
 *    diodo 1N4149. Sin esto, el primer MOSFET alto no se encendería.
 * 2. rampaArrencada(): recorre la secuencia de 6 estados acelerando, de
 *    TFASE_INICIAL_US a TFASE_FINAL_US por estado, en CICLOS_RAMPA ciclos
 *    eléctricos, mientras el duty pasa de DUTY_RAMPA_INICIAL a DUTY_RAMPA_FINAL.
 * 3. maintainOpenLoop(): sigue conmutando a TFASE_FINAL_US con el duty del
 *    potenciómetro (limitado y sin saltos bruscos) hasta que se pulsa SW1.
 *
 * La duración de cada estado se mide con el TIMER3: se arranca la cuenta justo
 * después de conmutar y, mientras corre, se hacen las tareas (ADC, pulsador).
 * Así esas tareas no alargan el estado.
 */

#include "OpenLoop.h"
#include "Conmutacion.h"
#include "LowMOSFETs.h"
#include "Delay.h"
#include "DutyPotenciometre.h"
#include "SensoresBEMF.h"
#include "Interfaz.h"

volatile uint32_t g_tfaseOL_us = TFASE_INICIAL_US;
volatile float    g_dutyOL = DUTY_RAMPA_INICIAL;
volatile uint32_t g_ciclosOL = 0U;
volatile uint8_t  g_bemfPorEstado[6] = { 0U, 0U, 0U, 0U, 0U, 0U };

static uint32_t s_usPulsado = 0U;      /* tiempo que SW1 lleva pulsado sin soltar */
static bool     s_pulsadoAntes = false;

/* Se llama una vez por estado, con la duración del estado (el tiempo entre dos
 * llamadas). Pide paro cuando SW1 lleva pulsado sin soltar al menos
 * T_CONFIRMAR_PARO_MS, para filtrar rebotes y ruido. Al medirlo en tiempo,
 * responde igual al principio de la rampa (estados de 10 ms) que en régimen
 * (estados de 1,2 ms). */
static bool paroSolicitado(uint32_t tfase_us)
{
    if (pulsadorSW1Pulsado())
    {
        if (!s_pulsadoAntes)
        {
            s_usPulsado = 0U;               /* empieza a contar ahora */
        }
        else if (s_usPulsado < 1000000U)
        {
            s_usPulsado += tfase_us;        /* ha pasado un estado más pulsado */
        }
        s_pulsadoAntes = true;
    }
    else
    {
        s_usPulsado = 0U;
        s_pulsadoAntes = false;
    }
    return s_usPulsado >= (T_CONFIRMAR_PARO_MS * 1000U);
}

/* Acerca 'actual' a 'objetivo' como mucho 'paso'. */
static float acercar(float actual, float objetivo, float paso)
{
    if (objetivo > actual + paso)
    {
        return actual + paso;
    }
    if (objetivo < actual - paso)
    {
        return actual - paso;
    }
    return objetivo;
}

void precargarBootstrap(void)
{
    conmutacionApagarTodo();
    bajosActivarTodos();
    delayMs(T_PRECARGA_BOOT_MS);
    bajosApagar();
}

bool rampaArrencada(void)
{
    uint32_t ciclo;
    uint8_t estado;

    s_usPulsado = 0U;
    s_pulsadoAntes = false;
    g_ciclosOL = 0U;

    for (ciclo = 0U; ciclo < CICLOS_RAMPA; ciclo++)
    {
        /* k va de 0 (inicio) a 1 (final) en pasos iguales */
        float k = (CICLOS_RAMPA > 1U) ? (float)ciclo / (float)(CICLOS_RAMPA - 1U) : 1.0f;
        float tfase = (float)TFASE_INICIAL_US + k * ((float)TFASE_FINAL_US - (float)TFASE_INICIAL_US);
        float duty = DUTY_RAMPA_INICIAL + k * (DUTY_RAMPA_FINAL - DUTY_RAMPA_INICIAL);

        g_tfaseOL_us = (uint32_t)tfase;
        g_dutyOL = duty;

        for (estado = 0U; estado < NUM_ESTADOS; estado++)
        {
            conmutarEstado(estado, duty);
            temporizadorArrancarUs(g_tfaseOL_us);

            if (paroSolicitado(g_tfaseOL_us))
            {
                return false;           /* main() apaga el puente */
            }

            temporizadorEsperarFin();
            g_bemfPorEstado[estado] = leerSensoresBEMF();
        }
        g_ciclosOL++;
    }
    return true;
}

void maintainOpenLoop(void)
{
    uint8_t estado;
    float duty = DUTY_RAMPA_FINAL;      /* se parte del duty con el que acaba la rampa */

    g_tfaseOL_us = TFASE_FINAL_US;
    s_usPulsado = 0U;
    s_pulsadoAntes = false;

    while (1)
    {
        for (estado = 0U; estado < NUM_ESTADOS; estado++)
        {
            conmutarEstado(estado, duty);
            temporizadorArrancarUs(TFASE_FINAL_US);

            if (estado == 0U)
            {
                /* Una vez por ciclo eléctrico, mientras corre el estado 0.
                 * El nuevo duty se aplica en la siguiente conmutación (1,2 ms después). */
                float objetivo = llegirDutyPotenciometre();
                duty = acercar(duty, objetivo, DUTY_PASO_MAX);
                g_dutyOL = duty;
            }

            if (paroSolicitado(TFASE_FINAL_US))
            {
                return;                 /* main() apaga el puente */
            }

            temporizadorEsperarFin();
            g_bemfPorEstado[estado] = leerSensoresBEMF();
        }
        g_ciclosOL++;
    }
}
