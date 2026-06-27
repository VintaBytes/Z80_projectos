# Z80 Computadora Básica

Repositorio personal de proyectos, pruebas y documentación para diseñar y construir una computadora básica basada en el microprocesador Z80.

El objetivo es avanzar paso a paso desde una configuración mínima de laboratorio —CPU, clock, reset y señales básicas— hasta una computadora funcional capaz de ejecutar programas, usar memoria RAM, comunicarse por puerto serie y, más adelante, interactuar con terminales, pantallas o periféricos externos.


## Objetivo general del proyecto

Este proyecto busca construir una computadora elemental basada en Z80, sin intentar replicar una computadora comercial completa de los años 80, sino creando una plataforma propia, simple, didáctica y extensible.

La idea inicial es usar componentes clásicos, como un microprocesador Z80 real, junto con herramientas modernas de apoyo. En las primeras etapas, un Arduino Mega se utiliza como generador de clock, controlador de reset y sistema auxiliar de prueba. Más adelante podrá usarse para cargar programas en RAM, actuar como consola de depuración o ser reemplazado por lógica dedicada o por otros microcontroladores.

El proyecto está pensado para avanzar en etapas pequeñas, medibles y documentadas.

## Contenido del repositorio

```text
z80-compu-basica/
├── README.md
├── LICENSE
├── .gitignore
├── docs/
│   ├── componentes.md
│   ├── pinout-z80.md
│   ├── notas-tecnicas.md
│   ├── mediciones-osciloscopio.md
│   └── referencias.md
├── projects/
│   └── 001-mega-z80-clock-reset-nop/
│       ├── README.md
│       └── mega_z80_clock_reset_nop/
│           └── mega_z80_clock_reset_nop.ino
└── assets/
    └── README.md
```

## Proyectos

|  Nº | Proyecto                                                           | Descripción                                                                                                                                                                    | Estado    |
| --: | ------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | --------- |
| 001 | [Mega Z80 Clock Reset NOP](projects/001_MEGA_Clock_Reset_NOP) | Primer experimento con Z80 real: Arduino Mega genera clock y reset, fuerza `00h` en el bus de datos para ejecutar `NOPs` y permite verificar señales básicas con osciloscopio. | Funcional |

## Componentes principales

Por ahora el repositorio documenta pruebas realizadas con:

* CPU Zilog Z0840006PSC Z80 CPU, encapsulado DIP-40.
* Arduino Mega 2560 Pro Embed o compatible.
* Osciloscopio OWON SDS1104 de 4 canales.
* Protoboard o placa de pruebas.
* Resistencias de 10 kΩ para pull-up y pull-down.
* Capacitor cerámico de 100 nF para desacople.
* Cables Dupont.
* Alimentación de 5 V tomada desde el Arduino Mega para las primeras pruebas.

Más adelante se podrán incorporar:

* SRAM estática.
* Lógica 74HC / 74HCT.
* Buffers triestado.
* Latches.
* Interfaz serie.
* Terminal basada en microcontrolador.
* Almacenamiento externo.
* Pantalla o salida de video experimental.

## Etapa actual

La etapa actual valida el funcionamiento básico del Z80 en una configuración mínima.

El Arduino Mega cumple tres funciones principales:

* genera la señal de clock para la CPU;
* controla la línea `/RESET`;
* mantiene el bus de datos del Z80 en `00h`.

En Z80, el opcode `00h` corresponde a la instrucción `NOP`. Por eso, al mantener todas las líneas de datos en bajo, la CPU lee y ejecuta instrucciones `NOP` continuamente. Esto permite comprobar que la CPU sale de reset, recibe clock y comienza a generar ciclos de lectura.

Esta etapa todavía no utiliza ROM, RAM ni periféricos reales.

## Entorno de desarrollo

Los proyectos están pensados para:

* Arduino IDE 2.
* Placa Arduino Mega 2560 o compatible.
* Monitor serie a `115200 baud`, salvo que el proyecto indique otra configuración.
* Instrumental de medición para verificar señales digitales, especialmente osciloscopio.

Cada proyecto Arduino debe respetar la estructura esperada por Arduino IDE: la carpeta del sketch y el archivo `.ino` deben tener el mismo nombre.

Por ejemplo:

```text
mega_z80_clock_reset_nop/
└── mega_z80_clock_reset_nop.ino
```

Esto permite abrir el proyecto directamente desde Arduino IDE usando:

```text
File → Open...
```

## Organización del código

Cada experimento vive dentro de `projects/` y tiene su propio `README.md`.

La idea es que cada etapa sea independiente, reproducible y documentada. Cada proyecto puede incluir:

* objetivo de la etapa;
* componentes utilizados;
* conexiones realizadas;
* código fuente;
* explicación del funcionamiento;
* mediciones esperadas;
* problemas encontrados;
* próximos pasos.


## Próximos proyectos posibles

* Medición detallada de `/MREQ`, `/RD`, `/M1` y `/RFSH`.
* Observación del bus de direcciones durante la ejecución de `NOPs`.
* Prueba de avance del contador de programa usando A0, A1, A2 y A3.
* Conexión de SRAM estática.
* Carga de un programa en RAM desde Arduino Mega.
* Liberación del Z80 para ejecutar desde RAM.
* Implementación de un puerto de salida simple.
* Salida de texto por consola serie.
* Monitor básico para cargar, examinar y ejecutar programas.
* Terminal externa usando otro microcontrolador.
* Reemplazo progresivo del Arduino por lógica dedicada.

## Licencia

Este repositorio se publica con fines educativos y personales bajo licencia MIT. Ver [`LICENSE`](LICENSE).
