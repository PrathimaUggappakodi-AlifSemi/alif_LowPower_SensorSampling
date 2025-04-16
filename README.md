## Low Power Demo: Sensor Sampling

This is a use-case based low power demo for Alif Ensemble processors.

In this Sensor Sampling use-case, we demonstrate how to continuously capture sensor data from an external SPI ADC at low power. We maximize CPU sleep time by using the DMA peripheral and hardware event routing to drive SPI xfers without CPU interaction. We also demonstrate a dynamic clock scaling scheme which optimizes the power drawing based on application need by switching system clocks between low frequency HFXO and high frequency PLL sources. At low clocks, the DMA can continuously capture sensor data from the ADC and place it into SRAM without ever needing to wake the CPU. Only after enough data is captured into SRAM, the CPU is woken up to process the data. To maximize operating efficiency, the data processing step is done at high frequency with the PLL enabled.

Note: This demo is supported on Gen 2 Ensemble devices only. The demo may be run on the RTSS-HP or RTSS-HE. Please refer to the [Getting started ](doc/getting_started.md)  guide for more details.