#ifndef REGIONS_AE722F80F55D5LS_M55_HE_H
#define REGIONS_AE722F80F55D5LS_M55_HE_H


//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <h>ROM Configuration
// =======================
// <h> MRAM
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x80000000
#define __ROM0_BASE 0x80000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00580000
#define __ROM0_SIZE 0x00580000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __ROM0_DEFAULT 1
//   <q>Startup
//   <i> Selects region to be used for startup code.
#define __ROM0_STARTUP 0
// </h>

// </h>

// <h>RAM Configuration
// =======================
// <h> SRAM0
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x02000000
#define __RAM0_BASE 0x02000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00400000
#define __RAM0_SIZE 0x00400000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM0_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM0_NOINIT 0
// </h>

// <h> SRAM1
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x08000000
#define __RAM1_BASE 0x08000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00280000
#define __RAM1_SIZE 0x00280000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM1_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM1_NOINIT 0
// </h>

// <h> SRAM2
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x50000000
#define __RAM2_BASE 0x50000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00040000
#define __RAM2_SIZE 0x00040000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM2_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM2_NOINIT 0
// </h>

// <h> SRAM3
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x50800000
#define __RAM3_BASE 0x50800000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00100000
#define __RAM3_SIZE 0x00100000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM3_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM3_NOINIT 0
// </h>

// <h> SRAM4
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x58000000
#define __RAM4_BASE 0x58000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00040000
#define __RAM4_SIZE 0x00040000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM4_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM4_NOINIT 0
// </h>

// <h> SRAM5
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x58800000
#define __RAM5_BASE 0x58800000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00040000
#define __RAM5_SIZE 0x00040000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM5_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM5_NOINIT 0
// </h>

// <h> SRAM6_A
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x62000000
#define __RAM6_BASE 0x62000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00100000
#define __RAM6_SIZE 0x00100000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM6_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM6_NOINIT 0
// </h>

// <h> SRAM6_B
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x62400000
#define __RAM7_BASE 0x62400000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00100000
#define __RAM7_SIZE 0x00100000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM7_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM7_NOINIT 0
// </h>

// <h> SRAM7
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x63000000
#define __RAM8_BASE 0x63000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00080000
#define __RAM8_SIZE 0x00080000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM8_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM8_NOINIT 0
// </h>

// <h> SRAM8
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x63200000
#define __RAM9_BASE 0x63200000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00200000
#define __RAM9_SIZE 0x00200000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM9_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM9_NOINIT 0
// </h>

// <h> SRAM9
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x60000000
#define __RAM10_BASE 0x60000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x000C0000
#define __RAM10_SIZE 0x000C0000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM10_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM10_NOINIT 0
// </h>

// </h>

// <h>Stack / Heap Configuration
//   <o0> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
//   <o1> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
#define __STACK_SIZE 0x00000200
#define __HEAP_SIZE 0x00000C00
// </h>


#endif /* REGIONS_AE722F80F55D5LS_M55_HE_H */
