#include <RTE_Components.h>
#include CMSIS_device_header
#include <cmsis_compiler.h>
#include <pinconf.h>
#include <clk.h>

#include <se_services_port.h>
#define SERVICES_check_response {if ((ret != 0) || (service_response != 0)) while(1) __WFI();}

#ifndef HW_REG32
#define HW_REG32(u,v) (*((volatile uint32_t *)(u + v)))
#endif

extern void low_power_sensor_sampling_demo();

int main()
{
    int32_t ret;
    uint32_t service_response;

    /* Initialize the SE services */
    se_services_port_init();

    bool is_enabled;
    ret = SERVICES_pll_xtal_is_started(se_services_s_handle, &is_enabled, &service_response);
    if (ret != 0) while(1) __WFI();

    if (!is_enabled) {
        /* service call to turn on HFXO */
        ret = SERVICES_pll_xtal_start(se_services_s_handle, 1, 1, 0xFFFFFFFF, &service_response);
        SERVICES_check_response;
    }

    /* refer to the "CGU Registers Guide" in the Ensemble Hardware Reference Manual (HWRM) */
    CGU->OSC_CTRL |= 1U | 1U << 4;      // switch sys_xtal_sel[0] and periph_xtal_sel[4] from HFRC to HFXO
    CGU->PLL_CLK_SEL = 0;               // switch all clocks to non-PLL clock sources
    CGU->ESCLK_SEL = (3U << 12) | (3U << 8) | (3U << 4) | (3U << 0);

    /* refer to the "CLKCTL_SYS Registers Guide" in the Ensemble Hardware Reference Manual (HWRM) */
    HW_REG32(CLKCTL_SYS_BASE, 0x820) = 1;   // ACLK source set to REFCLK
    HW_REG32(CLKCTL_SYS_BASE, 0x824) = 3;   // ACLK_DIV set to 4 (only effective when ACLK source is SYSPLL)

    ret = SERVICES_pll_clkpll_is_locked(se_services_s_handle, &is_enabled, &service_response);
    if (ret != 0) while(1) __WFI();

    if (is_enabled) {
        /* service call to switch Secure Enclave away from PLL */
        ret = SERVICES_clocks_select_pll_source(se_services_s_handle,
                                /*pll_source_t*/    PLL_SOURCE_OSC,
                                /*pll_target_t*/    PLL_TARGET_SECENC,
                                                    &service_response);
        SERVICES_check_response;

        /* service call to turn off PLL */
        ret = SERVICES_pll_clkpll_stop(se_services_s_handle, &service_response);
        SERVICES_check_response;
    }

    /* refer to the "AON Registers Guide" in the Ensemble Hardware Reference Manual (HWRM) */
    /* Bus Clock Divisor Register (0x20)
     * AXI bus clock depends on above OSC Control and PLL Select registers
     * AHB bus clock: 0: divide by 1, 1: divide by 2, 2: divide by 4, 3: divide by 4
     * APB bus clock: 0: divide by 1, 1: divide by 2, 2: divide by 4, 3: divide by 4
     */
    AON->SYSTOP_CLK_DIV = 0x001;

    /* MISC_REG1 (0x30)
     * Controls top level HFXO divider (div by 2^X)
     * Note: this does not impact the available HFXOx2 clock that is used by RTSS
     */
    uint32_t reg_data;
    reg_data = HW_REG32(AON_BASE, 0x30);
    reg_data &= ~(15U << 13);
    reg_data |=  (2U << 13);        // div by 4
    HW_REG32(AON_BASE, 0x30) = reg_data;

    /* Alif Ensemble Development Kit typically uses 38.4MHz HFXO */
    uint32_t current_clk = 9600000;     // HFXO is divided by 4
    SystemHFOSCClock = current_clk;     // HFOSC clock, used by some peripherals, is either HFRC/2 or HFXO depending on periph_xtal_sel[4]
    SystemREFClock = current_clk;       // SYST_REFCLK, when not using PLL, is either HFRC or HFXO depending on sys_xtal_sel[0]
    SystemAXIClock = current_clk;       // SYST_ACLK is either REFCLK or SYSPLL (only SYSPLL can be divided by 1-32)
    SystemAHBClock = current_clk;       // SYST_HCLK is ACLK div by 1, 2, or 4
    SystemAPBClock = current_clk>>1;    // SYST_PCLK is ACLK div by 1, 2, or 4

    SystemCoreClock = current_clk;      // RTSS_CORE_CLK is HFRC, HFRC/2, HFXOx2, or HFXO

    //CLKCTL_PER_MST->CAMERA_PIXCLK_CTRL = 100U << 16 | 1;    // output SYST_ACLK/100 on CAM_XVCLK pin
	//pinconf_set(PORT_0, PIN_3, PINMUX_ALTERNATE_FUNCTION_6, 0);                 // P0_3: CAM_XVCLK  (mux mode 6)
    //pinconf_set(PORT_10, PIN_3, PINMUX_ALTERNATE_FUNCTION_7, 0);                // P10_3:CAM_XVCLK  (mux mode 7)
#if defined (M55_HE)
    //M55HE_CFG->HE_CAMERA_PIXCLK = 100U << 16 | 1;           // output RTSS_HE_CLK/100 on LPCAM_XVCLK pin
    //pinconf_set(PORT_0, PIN_3, PINMUX_ALTERNATE_FUNCTION_5, 0);                 // P0_3: LPCAM_XVCLK(mux mode 5)
    //pinconf_set(PORT_1, PIN_3, PINMUX_ALTERNATE_FUNCTION_5, 0);                 // P1_3: LPCAM_XVCLK(mux mode 5)
#endif

    low_power_sensor_sampling_demo();

    return 0;
}
