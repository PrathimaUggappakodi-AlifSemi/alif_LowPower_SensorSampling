#include <sys_clocks.h>
#include <soc.h>
#include <pm.h>
#include <pinconf.h>
#include <cmsis_compiler.h>
#include <se_services_port.h>

#ifndef HW_REG32
#define HW_REG32(u,v) (*((volatile uint32_t *)(u + v)))
#endif

#define SERVICES_check_response {if ((ret != 0) || (service_response != 0)) while(1) __WFI();}

#define DISABLE_SEMIHOST
#ifdef DISABLE_SEMIHOST
#ifndef RTE_Compiler_IO_STDOUT_User
#define printf(...) (0)
#endif
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6100100)
__ASM(".global __use_no_semihosting");
void _sys_exit(int ret) { while (1); }
void _ttywrch(int ch) { return; }
#elif defined ( __GNUC__ )
#define TRAP_RET_ZERO  {__BKPT(0); return 0;}
int _close(int val) TRAP_RET_ZERO
int _lseek(int val0, int   val1, int val2) TRAP_RET_ZERO
int _read (int val0, char* val1, int val2) TRAP_RET_ZERO
int _write(int val0, char* val1, int val2) TRAP_RET_ZERO
#endif
#endif

extern void low_power_sensor_sampling_demo();
volatile uint32_t HFRC_CLK;
volatile uint32_t HFXO_CLK;
volatile uint32_t RTSS_HE_CLK;
volatile uint32_t RTSS_HP_CLK;
int main()
{

    int32_t ret;
    uint32_t reg_data;
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
    CGU->ESCLK_SEL = (3U << 12) | (3U << 8) | (2U << 4) | (3U << 0);

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
    reg_data = HW_REG32(AON_BASE, 0x30);
    reg_data &= ~(15U << 13);
    reg_data |=  (2U << 13);        // div by 4
    HW_REG32(AON_BASE, 0x30) = reg_data;

    /* Alif Ensemble Development Kit typically uses 38.4MHz HFXO */
    HFXO_CLK = 9600000;        // top level HFXO clock
    HFRC_CLK = 76800000;        // top level HFRC clock

    /* Alif Ensemble Development Kit typically uses 38.4MHz HFXO */
    uint32_t current_clk = HFXO_CLK;
    SystemHFOSCClock = current_clk;     // HFOSC clock, used by some peripherals, is either HFRC/2 or HFXO depending on periph_xtal_sel[4]
    SystemREFClock = current_clk;       // SYST_REFCLK, when not using PLL, is either HFRC or HFXO depending on sys_xtal_sel[0]
    SystemAXIClock = current_clk;       // SYST_ACLK is either REFCLK or SYSPLL (only SYSPLL can be divided by 1-32)
    SystemAHBClock = current_clk>>1;    // SYST_HCLK is ACLK div by 1, 2, or 4
    SystemAPBClock = current_clk>>2;    // SYST_PCLK is ACLK div by 1, 2, or 4

   // SystemCoreClock = current_clk;      // RTSS_HE_CLK is HFRC, HFRC/2, HFXOx2, or HFXO
    /*uint32_t current_clk = get_HFXO_CLK();
    set_HFOSC_CLK(current_clk);     // HFOSC clock, used by some peripherals, is either HFRC/2 or HFXO depending on periph_xtal_sel[4]
    set_SOC_REFCLK(current_clk);    // SYST_REFCLK, when not using PLL, is either HFRC or HFXO depending on sys_xtal_sel[0]
    set_AXI_CLOCK(current_clk);     // SYST_ACLK is either REFCLK or SYSPLL (only SYSPLL can be divided by 1-32)
    set_AHB_CLOCK(current_clk);     // SYST_HCLK is ACLK div by 1, 2, or 4
    set_APB_CLOCK(current_clk>>1);  // SYST_PCLK is ACLK div by 1, 2, or 4*/

    RTSS_HE_CLK = current_clk;   // RTSS_HE_CLK is HFRC, HFRC/2, HFXOx2, or HFXO
    RTSS_HP_CLK = current_clk;   // RTSS_HP_CLK is HFRC, HFRC/2, HFXOx2, or HFXO


    CLKCTL_PER_MST->CAMERA_PIXCLK_CTRL = 100U << 16 | 1;    // output SYST_ACLK/100 on CAM_XVCLK pin
	pinconf_set(PORT_0, PIN_3, PINMUX_ALTERNATE_FUNCTION_6, 0);                 // P0_3: CAM_XVCLK  (mux mode 6)
    // pinconf_set(PORT_10, PIN_3, PINMUX_ALTERNATE_FUNCTION_7, 0);                // P10_3:CAM_XVCLK  (mux mode 7)
#if defined (CORE_M55_HE)
    M55HE_CFG->HE_CAMERA_PIXCLK = 100U << 16 | 1;           // output RTSS_HE_CLK/100 on LPCAM_XVCLK pin
    pinconf_set(PORT_0, PIN_3, PINMUX_ALTERNATE_FUNCTION_5, 0);                 // P0_3: LPCAM_XVCLK(mux mode 5)
    pinconf_set(PORT_1, PIN_3, PINMUX_ALTERNATE_FUNCTION_5, 0);                 // P1_3: LPCAM_XVCLK(mux mode 5)
#endif

    low_power_sensor_sampling_demo();

while(1);
    return 0;
}
