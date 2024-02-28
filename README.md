# Low Power Demo: Sensor Sampling
This is a use-case based low power demo for Alif Ensemble E1 and E3 processors.

In this Sensor Sampling use-case, we demonstrate how to continuously capture
sensor data from an external SPI ADC at low power. We maximize CPU sleep time by
using the DMA peripheral and hardware event routing to drive SPI xfers without
CPU interaction. We also demonstrate a dynamic clock scaling scheme which
optimizes the power draw based on application need by switching system clocks
between low frequency HFXO and high frequency PLL sources. At low clocks, the
DMA can continuously capture sensor data from the ADC and place it into SRAM
without ever needing to wake the CPU. Only after enough data is captured into
SRAM, the CPU is woken up to process the data. To maximize operating efficiency,
the data processing step is done at high frequency with the PLL enabled.

Note: This demo is supported on Gen 2 Ensemble devices only. The demo may be run
on the RTSS-HP or RTSS-HE.

## Project Setup
### Hardware Setup
1. Alif Ensemble DevKit (Gen 2)
   - please attach a jumper wire connected between P4_0 and P15_0
2. Logic Analyzer to monitor the below MCU signals
   - P15_0  LPTIMER0  (loopback to P4_0)
   - P0_0   UTIMER0_A
   - P0_1   UTIMER0_B
   - P0_2   UTIMER1_A
   - P5_0   SPI0_DATA (loopback to P5_1)
   - P5_2   SPI0_SELECT
   - P5_3   SPI0_SCLK
   - P5_4   software debug (optional)
   - P5_6   software debug (optional)
   - P0_3   SYST_ACLK clock debug (optional)
   - P1_3   RTSS_HE_CLK clock debug (optional)
3. Power Analyzer connected to DevKit J5 (for current measurement on MCU_3V3)
4. SEGGER JLink Debug Probe (optional)

**Software Debug Signal Sequence**
1. P5_4 low-to-high when the CPU wakes after 20 x 8 SPI transfers
2. P5_6 low-to-high high during sync barrier start
3. P5_6 high-to-low after sync barrier end, dynamic clock switching done here
4. P5_4 high-to-low after dynamic clock switching is done, CPU processing starts
5. P5_4 low-to-high after CPU processing ends
6. P5_6 low-to-high high during sync barrier start
7. P5_6 high-to-low after sync barrier end, dynamic clock switching done here
8. P5_4 high-to-low after dynamic clock switching is done, CPU goes to sleep

### Software Setup
The following software must be installed on your system (tested with Ubuntu 22.04 LTS)
1. Setup the VSCode environment as outlined in [Getting Started with VSCode](https://alifsemi.com/download/AUGD0012).
   - Ubuntu 22.04 LTS users, please download Arm GCC 10.3-2021.10 (Python compatibility issues)
2. Modify the Alif Ensemble CMSIS DFP v1.0.0 with the files provided in this project
   - Extract the archive in the patch directory and overwrite the files at cmsis-packs/AlifSemiconductor/Ensemble/1.0.0/
3. Install the latest SEGGER J-Link software (optional) [SEGGER J-Link](https://www.segger.com/downloads/jlink)

After setting up the development environment
1. Clone this repository and open the folder as a project in VSCode.
2. Make sure you select the configuration by pressing F1 and typing "select a configuration" and picking the core - HE or HP
3. Press CTRL-SHIFT-B to build the project. Default configuration is for executing-in-place (XIP) from MRAM.
4. Press F1 -> "Run Tasks" -> "Install Debug Stubs with Security Toolkit"
5. Press F1 -> "Run Tasks" -> "Program with Security Toolkit"
6. Press 5 to start debugging, **OR** *skip to the next step*

Note: To clean the project, press F1 -> "Run Tasks" -> "Clean Project with cbuild". Otherwise, if you use the "Clean All" option instead, then it will delete the RTE_Device.h file which was modified to make #define RTE_SPI0_DMA_ENABLE set to 1. If the DMA_ENABLE is set to 0 then the demo will not work properly.

**Troubleshooting**
* [ERROR] Target did not respond
   1. Place the MCU in hard maintenance mode before programming
   2. In a terminal, navigate to the app-release-exec directory
   3. Run ./maintenance and then enter the below options
      * option 1 - Device Control
      * option 1 - Hard maintenance mode
   4. Press the RESET button on the Ensemble DevKit
   5. Hit enter until you exit the maintenance tool to release the serial port
   6. Re-try the "Program with Security Toolkit" step
* SPI transactions only occur once, and are not continuous
   1. DMA might not be enabled for this project
   2. Open the file at RTE/Device/AExxx/RTE_Device.h
   3. Make sure #define RTE_SPI0_DMA_ENABLE is set to 1
