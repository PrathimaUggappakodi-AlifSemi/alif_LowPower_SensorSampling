#include <stdint.h>
#include <stdio.h>

#include <RTE_Components.h>
#include CMSIS_device_header

#include <pinconf.h>
#include <Driver_GPIO.h>
#include <Driver_LPTIMER_Private.h>

#include "LPTIMER_config.h"

#ifndef HW_REG32
#define HW_REG32(u,v) (*((volatile uint32_t *)(u + v)))
#endif

extern ARM_DRIVER_LPTIMER DRIVER_LPTIMER0;
static ARM_DRIVER_LPTIMER *ptrLPTIMER = &DRIVER_LPTIMER0;

volatile uint32_t lptimer_cb_status = 0;

static void LPTIMER_cb_func(uint8_t event)
{
    /* Note: LPTIMER IRQ is disabled at the NVIC (not needed for the demo) */
    if (event == ARM_LPTIMER_EVENT_UNDERFLOW)
    {
        lptimer_cb_status |= event;
    }
}

void LPTIMER_config()
{
    int32_t ret;
    uint32_t count = 15;  /* 32768 Hz div-by (15+1) = 2048 Hz */
    uint8_t channel = LPTIMER_CHANNEL;

    /* Initialize LPTIMER */
    ret = ptrLPTIMER->Initialize(channel, LPTIMER_cb_func);
    while(ret != ARM_DRIVER_OK);

    ret = ptrLPTIMER->PowerControl(channel, ARM_POWER_FULL);
    while(ret != ARM_DRIVER_OK);

    ret = ptrLPTIMER->Control(channel, ARM_LPTIMER_SET_COUNT1, &count);
    while(ret != ARM_DRIVER_OK);

    /* Port 15 pinmux (via LPGPIO peripheral) */
    HW_REG32(LPGPIO_BASE, 0x08) = 1U << channel;

    /* Port 15 padconfig */
    HW_REG32(LPGPIO_CTRL_BASE, (channel << 2)) = PADCTRL_OUTPUT_DRIVE_STRENGTH_4MA;

    lptimer_cb_status = 0;
    ret = ptrLPTIMER->Start(channel);
    while(ret != ARM_DRIVER_OK);

    /* LPTIMER will drive a pin on port 15, its IRQ is not used */
    NVIC_DisableIRQ(LPTIMER_CHANNEL_IRQ(channel));
}
