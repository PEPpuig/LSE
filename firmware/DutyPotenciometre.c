/*
 * DutyPotenciometre.c
 *
 * El cursor de RPOT1 (10 kΩ entre GND y +3V3) llega a PB5 con un filtro de
 * 100 nF (CPOT1). Se usa el secuenciador 3 del ADC0 (una sola muestra),
 * disparado por software y leído por sondeo, como en el proyecto de referencia.
 * La media por hardware de 16 muestras quita el ruido del PWM.
 */

#include "DutyPotenciometre.h"

#define ADC_SECUENCIADOR    3U

void initADC_Potenciometre(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(POT_PERIPH_GPIO);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)) { }
    while (!SysCtlPeripheralReady(POT_PERIPH_GPIO)) { }

    GPIOPinTypeADC(POT_PUERTO, POT_PIN);                 /* PB5 como entrada analógica */

    ADCHardwareOversampleConfigure(ADC0_BASE, POT_SOBREMUESTREO);

    ADCSequenceDisable(ADC0_BASE, ADC_SECUENCIADOR);
    ADCSequenceConfigure(ADC0_BASE, ADC_SECUENCIADOR, ADC_TRIGGER_PROCESSOR, 0);
    /* Un solo paso: canal AIN11, marca fin de conversión (IE) y fin de secuencia */
    ADCSequenceStepConfigure(ADC0_BASE, ADC_SECUENCIADOR, 0,
                             POT_CANAL_ADC | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, ADC_SECUENCIADOR);
    ADCIntClear(ADC0_BASE, ADC_SECUENCIADOR);
    /* No hace falta ADCIntEnable(): se lee el flag "raw", que se activa igual. */
}

uint16_t llegirADC_Potenciometre(void)
{
    uint32_t valor = 0U;

    ADCIntClear(ADC0_BASE, ADC_SECUENCIADOR);
    ADCProcessorTrigger(ADC0_BASE, ADC_SECUENCIADOR);

    while (!ADCIntStatus(ADC0_BASE, ADC_SECUENCIADOR, false))   /* espera fin de conversión */
    {
    }
    ADCIntClear(ADC0_BASE, ADC_SECUENCIADOR);

    ADCSequenceDataGet(ADC0_BASE, ADC_SECUENCIADOR, &valor);
    return (uint16_t)(valor & 0x0FFFU);
}

float llegirDutyPotenciometre(void)
{
    float x = (float)llegirADC_Potenciometre() / 4095.0f;   /* 0,0 a 1,0 */
    return DUTY_POT_MIN + x * (DUTY_POT_MAX - DUTY_POT_MIN);
}
