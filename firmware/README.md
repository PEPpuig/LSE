# Firmware open loop · Controlador BLDC (PEPpuig/LSE)

Firmware de lazo abierto para la placa Controlador_BLDC (LaunchPad EK-TM4C123GXL, drivers IR2301, MOSFET IRFR1205).
Está basado en `open-loop-baseline` del proyecto de referencia y adaptado a los pines de esta placa.

## Archivos

| Archivo | Qué hace |
| --- | --- |
| `config_placa.h` | Pines de la placa y parámetros de arranque. Es el único archivo que hay que tocar para ajustar. |
| `main.c` | Espera a SW1, arranca (precarga, rampa, open loop) y para con SW1. |
| `Init_Periferics.c/.h` | Reloj a 50 MHz e inicialización de todos los periféricos en orden seguro. |
| `PWM_HighMOSFETs.c/.h` | PWM de 20 kHz en los MOSFET altos (AH = PB4, BH = PE5, CH = PE4). |
| `LowMOSFETs.c/.h` | MOSFET bajos con GPIO (AL = PF1, BL = PA7, CL = PA6). |
| `Conmutacion.c/.h` | Tabla de los 6 estados y conmutación con tiempo muerto. |
| `OpenLoop.c/.h` | Precarga de bootstrap, rampa de arranque y mantenimiento en open loop. |
| `DutyPotenciometre.c/.h` | Lectura del potenciómetro (PB5 = AIN11) y duty de open loop. |
| `SensoresBEMF.c/.h` | Lectura de SensorA/B/C (PD3, PD2, PD1) corrigiendo la inversión de la placa. |
| `Interfaz.c/.h` | LED azul (PF2), LED verde (PF3) y pulsador SW1 (PF4). |
| `Delay.c/.h` | Temporización con el TIMER3. |
| `tm4c123gh6pm_startup_ccs.c`, `tm4c123gh6pm.cmd` | Arranque y mapa de memoria estándar de TI para CCS (sin interrupciones propias). |

## Crear el proyecto en Code Composer Studio

1. *File → New → CCS Project*. Dispositivo **Tiva TM4C123GH6PM**, conexión *Stellaris In-Circuit Debug Interface*, plantilla *Empty Project*.
2. Borrar el `main.c`, el `.cmd` y el `startup` que crea CCS y copiar todos los archivos de esta carpeta al proyecto.
3. *Project → Properties → Build → ARM Compiler → Include Options*: añadir la carpeta de TivaWare (por ejemplo `C:/ti/TivaWare_C_Series-2.2.0.295`).
4. *ARM Compiler → Predefined Symbols*: añadir `PART_TM4C123GH6PM` (y `ccs` si no está).
5. *ARM Linker → File Search Path*: añadir `C:/ti/TivaWare_C_Series-2.2.0.295/driverlib/ccs/Debug/driverlib.lib`.
6. Compilar y cargar con el depurador.

## Uso

- **LED azul**: esperando. Pulsar **SW1** para arrancar.
- **LED verde**: motor en marcha. Primero se cargan los bootstrap (10 ms), luego la rampa (unos 3,4 s) y después sigue en open loop a 1,2 ms por estado. El potenciómetro ajusta el duty entre 0,20 y 0,50.
- Mantener **SW1** pulsado al menos 20 ms para **parar**: se apagan los seis MOSFET y el motor queda en rueda libre. A los 2 s se puede volver a arrancar.
- El LED rojo de la LaunchPad parpadea con el motor en marcha: es la señal AL (PF1).

Desde el depurador (ventana *Expressions*) se pueden ver `g_estadoPuente`, `g_tfaseOL_us`, `g_dutyOL`, `g_ciclosOL` y `g_bemfPorEstado[]`.

## Primera prueba

Fuente de 12 V con límite de corriente bajo (por ejemplo 2 A) y el potenciómetro al mínimo. La placa no tiene medida de corriente ni protección de sobrecorriente.
