 // use sprc234 (Version: 03.00.10.02) for the CSL include files

#include <stdio.h>
#include <string.h>

#include <cslr_pllc.h>
#include <cslr_gpio.h>
#include <cslr_emifa.h>
#include <cslr_ddr2.h>
#include <cslr_dev.h>
#include <cslr_intc.h>
#include <cslr_chip.h>
#include <cslr_edma3cc.h>
#include <cslr_cache.h>
#include <cslr_tmr.h>
#include <soc.h>

#include <c6x.h>

#include "Device_config.h"
#include "Device_lib.h"

typedef volatile CSL_DevRegs             *CSL_DevRegsOvly;


extern void intcVectorTable(void);  

void init_cache();
void init_gpio();
void init_pll1();
void init_pll2();
void init_emifa();
void init_ddr2();
void init_irq();
void init_timer();
void init_dma(int indata_channels);
void init_qdma(int indata_channels);


Uint32 MeaData[MAX_DATAPOINTS_PER_FRAME];
#pragma DATA_ALIGN(MeaData, 8);
Uint32 MonitorData[MONITOR_ARRAY];
Uint32 ChannelsPerSweepConfigured;


/* Globals */

CSL_DevRegsOvly devRegs = (CSL_DevRegsOvly)CSL_DEV_REGS;

void MEA21_init()
{
    init_gpio();
   	init_pll1();
    init_ddr2();
	init_emifa();
	init_cache();

	WRITE_REGISTER(DSP_INDATA_CTRL0, DSPINDATACTRL0_CLEAR_FIFO | DSPINDATACTRL0_RESET_FIFO);    // Disable all Data Channels and Clear Fifo
	WRITE_REGISTER(MAILBOX_CTRL, 0x100);                  // enable DSP Mailbox interrupts

	init_irq();
}

void MEA21_enableData()
{
    int i;

    WRITE_REGISTER(DSP_INDATA_CONFIG0, INDATA_CONFIG0_VALUE); // Enable Data Sources
    WRITE_REGISTER(DSP_INDATA_CONFIG1, INDATA_CONFIG1_VALUE);
    WRITE_REGISTER(DSP_INDATA_CONFIG2, INDATA_CONFIG2_VALUE);
    WRITE_REGISTER(DSP_INDATA_CONFIG3, INDATA_CONFIG3_VALUE);
    WRITE_REGISTER(DSP_INDATA_CONFIG4, INDATA_CONFIG4_VALUE);
    WRITE_REGISTER(DSP_INDATA_CONFIG5, INDATA_CONFIG5_VALUE);
    WRITE_REGISTER(DSP_INDATA_CONFIG6, INDATA_CONFIG6_VALUE);
    WRITE_REGISTER(DSP_INDATA_CONFIG7, INDATA_CONFIG7_VALUE);
    WRITE_REGISTER(DSP_INDATA_CONFIG8, INDATA_CONFIG8_VALUE);
    WRITE_REGISTER(DSP_INDATA_CONFIG9, INDATA_CONFIG9_VALUE);
    WRITE_REGISTER(DSP_INDATA_CONFIGa, INDATA_CONFIGa_VALUE);
    WRITE_REGISTER(DSP_INDATA_CONFIGb, INDATA_CONFIGb_VALUE);
    WRITE_REGISTER(DSP_INDATA_CONFIGc, INDATA_CONFIGc_VALUE);
    WRITE_REGISTER(DSP_INDATA_CONFIGd, INDATA_CONFIGd_VALUE);
    WRITE_REGISTER(DSP_INDATA_CONFIGe, INDATA_CONFIGe_VALUE);
    WRITE_REGISTER(DSP_INDATA_CONFIGf, INDATA_CONFIGf_VALUE);

    for (i = 0; i < 5000; i++)
    {
        ChannelsPerSweepConfigured = READ_REGISTER(DSP_INDATA_CHANNELS);  // should be the same number as CHANNELS_PER_FRAME
        if (ChannelsPerSweepConfigured) break;                            // a non-zero value indicated the FPGA has finished configuration
    }

    init_timer();
    init_dma(ChannelsPerSweepConfigured);
    init_qdma(ChannelsPerSweepConfigured);

    WRITE_REGISTER(DSP_INDATA_CTRL0, DSPINDATACTRL0_INT_ENABLE | DSPINDATACTRL0_CLEAR_FIFO | DSPINDATACTRL0_RESET_FIFO);    // Enable Irq

    SetMonitorSize(MONITOR_ARRAY);
}

void init_timer()
{
    Bool timerRdy;

    // enable timer 0
    // Unlock the control register
    CSL_FINST(((CSL_DevRegs *)CSL_DEV_REGS)->PERLOCK, DEV_PERLOCK_LOCKVAL, UNLOCK);
    CSL_FINST(((CSL_DevRegs *)CSL_DEV_REGS)->PERCFG0, DEV_PERCFG0_TIMER0CTL, ENABLE);
    do {
        timerRdy = (Bool) CSL_FEXT(((CSL_DevRegs *)CSL_DEV_REGS)->PERSTAT0, DEV_PERSTAT0_TIMER0STAT);
    } while (!timerRdy);

    // enable timer 1
    // Unlock the control register
    CSL_FINST(((CSL_DevRegs *)CSL_DEV_REGS)->PERLOCK, DEV_PERLOCK_LOCKVAL, UNLOCK);
    CSL_FINST(((CSL_DevRegs *)CSL_DEV_REGS)->PERCFG0, DEV_PERCFG0_TIMER1CTL, ENABLE);
    do {
        timerRdy = (Bool) CSL_FEXT(((CSL_DevRegs *)CSL_DEV_REGS)->PERSTAT0, DEV_PERSTAT0_TIMER1STAT);
    } while (!timerRdy);


    CSL_TmrRegsOvly tmr1Regs = (CSL_TmrRegsOvly)CSL_TMR_1_REGS;

    // clear TIM12 register
    CSL_FINST(tmr1Regs->TIMLO,TMR_TIMLO_TIMLO,RESETVAL);

    CSL_FINS(tmr1Regs->TCR, TMR_TCR_CLKSRC_LO, 0);

    // select 32 bit unchained mode and take the timer out of reset
    CSL_FINS(tmr1Regs->TGCR, TMR_TGCR_TIMMODE, 1);  // 32bit unchained
    CSL_FINST(tmr1Regs->TGCR, TMR_TGCR_TIMLORS, RESET_OFF);
}

void init_gpio()
{
    CSL_GpioRegsOvly gpioRegs = (CSL_GpioRegsOvly)CSL_GPIO_0_REGS;
    Bool  gpioEn;

    /* Unlock the control register */
    CSL_FINST(((CSL_DevRegs*)CSL_DEV_REGS)->PERLOCK, DEV_PERLOCK_LOCKVAL, 
              UNLOCK);
               
    /* Enable the GPIO */
    CSL_FINST(((CSL_DevRegs*)CSL_DEV_REGS)->PERCFG0, DEV_PERCFG0_GPIOCTL, 
              ENABLE);

    do {
        gpioEn = (Bool) CSL_FEXT(((CSL_DevRegs*)CSL_DEV_REGS)->PERSTAT0, 
                                   DEV_PERSTAT0_GPIOSTAT);
    } 
    while (gpioEn != CSL_DEV_PERSTAT0_GPIOSTAT_ENABLE);


	CSL_FINS(gpioRegs->DIR, GPIO_DIR_DIR2 , CSL_GPIO_DIR_DIR_OUT);	// LED
	CSL_FINS(gpioRegs->DIR, GPIO_DIR_DIR15, CSL_GPIO_DIR_DIR_OUT);	// LED

	CSL_FINS(gpioRegs->OUT_DATA, GPIO_OUT_DATA_OUT2, 1); // LED
	CSL_FINS(gpioRegs->OUT_DATA, GPIO_OUT_DATA_OUT15, 0); // LED


	CSL_FINS(gpioRegs->DIR, GPIO_DIR_DIR4 , CSL_GPIO_DIR_DIR_IN);	// FPGA Data available
	CSL_FINS(gpioRegs->DIR, GPIO_DIR_DIR6 , CSL_GPIO_DIR_DIR_IN);	// Mailbox IRQ
	
	/* Enable Interrupts for GP[4] */
	CSL_FINS(gpioRegs->SET_RIS_TRIG, GPIO_SET_RIS_TRIG_SETRIS4, CSL_GPIO_SET_RIS_TRIG_SETRIS_ENABLE);
	/* Enable Interrupts for GP[6] */
	CSL_FINS(gpioRegs->SET_RIS_TRIG, GPIO_SET_RIS_TRIG_SETRIS6, CSL_GPIO_SET_RIS_TRIG_SETRIS_ENABLE);
	CSL_FINST(gpioRegs->BINTEN, GPIO_BINTEN_EN, ENABLE);
}

void init_cache()
{
    ((CSL_CacheRegsOvly)CSL_CACHE_0_REGS)->L1PCFG = CSL_CACHE_L1PCFG_MODE_32K;
    ((CSL_CacheRegsOvly)CSL_CACHE_0_REGS)->L1DCFG = CSL_CACHE_L1DCFG_MODE_32K;
}

void init_pll1()
{
	// input Clock is 51.2 MHz
	// max device clock is 850 MHz
	volatile int i;
	
	CSL_PllcRegsOvly pllRegs = (CSL_PllcRegsOvly)CSL_PLLC_1_REGS;
	
	CSL_FINST(pllRegs->PLLCTL, PLLC_PLLCTL_PLLENSRC, REGBIT);
	CSL_FINST(pllRegs->PLLCTL, PLLC_PLLCTL_PLLEN, BYPASS);

  	// wait 4 cycles
  	for (i = 0; i < 4; i++);

 	CSL_FINST(pllRegs->PLLCTL, PLLC_PLLCTL_PLLRST, YES);
	
	CSL_FINS(pllRegs->PREDIV,  PLLC_PREDIV_RATIO,   0); // Prediv by 1
	CSL_FINS(pllRegs->PLLM,    PLLC_PLLM_PLLM,     15); // Multiply by 16 -> 51.2 * 16 = 819.2 MHz

	CSL_FINS(pllRegs->PLLDIV4, PLLC_PLLDIV4_RATIO,  7); // 7: divide by 16 ->  51.2 MHz (Max: 166 MHz) 
	CSL_FINS(pllRegs->PLLDIV5, PLLC_PLLDIV5_RATIO,  2); // 2: divide by 3  -> 273.06 MHz (Max: 333 MHz)
//	CSL_FINST(pllRegs->ALNCTL, PLLC_ALNCTL_ALN4, YES); // Align SYSCLK4, default, do not change
//	CSL_FINST(pllRegs->ALNCTL, PLLC_ALNCTL_ALN5, YES); // Align SYSCLK5, default, do not change
	CSL_FINST(pllRegs->PLLCMD, PLLC_PLLCMD_GOSET, SET); //
	while (CSL_FEXT(pllRegs->PLLSTAT, PLLC_PLLSTAT_GOSTAT) == CSL_PLLC_PLLSTAT_GOSTAT_INPROG);

	// wait for PLL reset (128 * CLKIN1 cycle time) -> 2.5 us
	for (i = 0; i <  2000; i++);
	
 	CSL_FINST(pllRegs->PLLCTL, PLLC_PLLCTL_PLLRST, NO);
		
	// wait for PLL lock time (2000 * CLKIN1 cycle time) -> 96.9 us
	for (i = 0; i <  20000; i++);
	
	CSL_FINST(pllRegs->PLLCTL, PLLC_PLLCTL_PLLEN, PLL);
}

void init_pll2()
{
	// input Clock is 25.6 MHz
	// PLLOUT is 250 .. 533 MHz
	// DDR2 clock is fixed to PLLOUT/2
	// PLLM is fixed to x20 -> 25.6 MHz -> 512 MHz PLLOUT -> 256 MHz DDR2 Clock
	// SYSLK1 is  50 or 125 MHz when EMAC is used
}

// we want ARDY pin is active-low (strobe period extended when ARDY is low)
// Requirement for the Xilinx async Interface:
// T_SETUP:  >= 3x 38.4 MHz cycles (> 78.125 ns) (2 cycles should be ok?)
// T_STROBE:  > 2x 38.4 MHz cycles (> 52.083 ns) (+ ARDY; ARDY must be asserted and deasserted for 2 cycles)
// T_HOLD:   >= 1x 38.4 MHz cycle  (> 26.042 ns)

// Clock for EMIFA is AECLKIN, which is 38.4 MHz

void init_emifa()
{
	CSL_EmifaRegsOvly emifaRegs = (CSL_EmifaRegsOvly)CSL_EMIFA_0_REGS;

	CSL_FINST(devRegs->PERCFG1, DEV_PERCFG1_EMIFACTL, ENABLE);
	CSL_FINS(emifaRegs->BPRIO, EMIFA_BPRIO_PRIO_RAISE, 0xfe);	// see SPRZ234N Silicon Errata

  	CSL_FINST(emifaRegs->CE2CFG, EMIFA_CE2CFG_SSEL, ASYNCMEM);	// CE2 (0xa0000000) uses ASYNC interface
  	CSL_FINST(emifaRegs->CE2CFG, EMIFA_CE2CFG_SS, SSMDIS);		// Disable Select Strobe Mode
  	CSL_FINST(emifaRegs->CE2CFG, EMIFA_CE2CFG_BWEM, WESDIS);	// Disable WE Strobe Mode
   	CSL_FINST(emifaRegs->CE2CFG, EMIFA_CE2CFG_AE, ARDYEN);		// enable ARDY Pin
   	CSL_FINS(emifaRegs->CE2CFG, EMIFA_CE2CFG_W_SETUP, 2);		// 3 cycles @38.4 MHz
   	CSL_FINS(emifaRegs->CE2CFG, EMIFA_CE2CFG_W_STROBE, 1);		// 2 cycles @38.4 MHz
   	CSL_FINS(emifaRegs->CE2CFG, EMIFA_CE2CFG_W_HOLD, 0);		// 1 cycle  @38.4 MHz
   	CSL_FINS(emifaRegs->CE2CFG, EMIFA_CE2CFG_R_SETUP, 2);		// 3 cycles @38.4 MHz
   	CSL_FINS(emifaRegs->CE2CFG, EMIFA_CE2CFG_R_STROBE, 1);		// 2 cycles @38.4 MHz
   	CSL_FINS(emifaRegs->CE2CFG, EMIFA_CE2CFG_R_HOLD, 0);		// 1 cycle  @38.4 MHz
   	CSL_FINST(emifaRegs->CE2CFG, EMIFA_CE2CFG_ASIZE, 32BIT);	// enable ARDY Pin

  	CSL_FINST(emifaRegs->CE3CFG, EMIFA_CE3CFG_SSEL, SYNCMEM);	// CE3 (0xb0000000) uses SYNC interface
  	CSL_FINST(emifaRegs->CE3CFG, EMIFA_CE3CFG_RD_BE_EN, SET);
  	CSL_FINST(emifaRegs->CE3CFG, EMIFA_CE3CFG_CE_EXT, CLR);
  	CSL_FINST(emifaRegs->CE3CFG, EMIFA_CE3CFG_R_ENABLE, SET);
  	CSL_FINST(emifaRegs->CE3CFG, EMIFA_CE3CFG_W_LTNCY, ONECYCLE);
  	CSL_FINST(emifaRegs->CE3CFG, EMIFA_CE3CFG_R_LTNCY, THREECYCLE);
  	CSL_FINST(emifaRegs->CE3CFG, EMIFA_CE3CFG_SBSIZE, 32BIT);

  	CSL_FINST(emifaRegs->CE4CFG, EMIFA_CE4CFG_SSEL, SYNCMEM);	// CE4 (0xc0000000) uses SYNC interface to stream Stimulus data
  	CSL_FINST(emifaRegs->CE4CFG, EMIFA_CE4CFG_RD_BE_EN, SET);
  	CSL_FINST(emifaRegs->CE4CFG, EMIFA_CE4CFG_CE_EXT, CLR);
  	CSL_FINST(emifaRegs->CE4CFG, EMIFA_CE4CFG_R_ENABLE, SET);
  	CSL_FINST(emifaRegs->CE4CFG, EMIFA_CE4CFG_W_LTNCY, ONECYCLE);
  	CSL_FINST(emifaRegs->CE4CFG, EMIFA_CE4CFG_R_LTNCY, THREECYCLE);
  	CSL_FINST(emifaRegs->CE4CFG, EMIFA_CE4CFG_SBSIZE, 32BIT);

  	CSL_FINS(emifaRegs->AWCC, EMIFA_AWCC_AP, 0);  				// define polarity of ARDY pin: active low
  	CSL_FINS(emifaRegs->AWCC, EMIFA_AWCC_TA, 7);  				// Turn Around cycles
  	CSL_FINS(emifaRegs->AWCC, EMIFA_AWCC_MAX_EXT_WAIT, 0xff);	// Maximum Extended Wait cycles; 106.25 us @ AECLK of 38.4 MHz
}

void init_ddr2()
{
	CSL_Ddr2RegsOvly ddr2Regs = (CSL_Ddr2RegsOvly)CSL_DDR2_0_REGS;

	CSL_FINST(devRegs->PERCFG1, DEV_PERCFG1_DDR2CTL, ENABLE);

	CSL_FINS(ddr2Regs->SDRFC, DDR2_SDRFC_REFRESH_RATE, 3200); // 256 MHz * 200 us / 16 = 3200
	CSL_FINST(ddr2Regs->DMCCTL, DDR2_DMCCTL_IFRESET, RELEASE);

	CSL_FINS(ddr2Regs->BPRIO, DDR2_BPRIO_PRIO_RAISE, 0xfe);	// see SPRZ234N Silicon Errata
	CSL_FINS(ddr2Regs->SDRFC, DDR2_SDRFC_REFRESH_RATE, 3200); // 256 MHz * 200 us / 16 = 3200

	CSL_FINST(ddr2Regs->SDCFG, DDR2_SDCFG_BOOT_UNLOCK, UNLOCKED);
	CSL_FINST(ddr2Regs->SDCFG, DDR2_SDCFG_DDR_DRIVE, NORMAL); // default is NORMAL
	CSL_FINST(ddr2Regs->SDCFG, DDR2_SDCFG_BOOT_UNLOCK, LOCKED);
	
	CSL_FINST(ddr2Regs->SDCFG, DDR2_SDCFG_TIMUNLOCK, SET);
	CSL_FINST(ddr2Regs->SDCFG, DDR2_SDCFG_NM, 16BIT);
	CSL_FINST(ddr2Regs->SDCFG, DDR2_SDCFG_CL, FIVE);
	CSL_FINST(ddr2Regs->SDCFG, DDR2_SDCFG_IBANK, EIGHT);
	CSL_FINST(ddr2Regs->SDCFG, DDR2_SDCFG_PAGESIZE, 1024W_PAGE);
	
	CSL_FINS(ddr2Regs->SDTIM1, DDR2_SDTIM1_T_RFC, 49); //  195 ns * 256 MHz = 49.92
	CSL_FINS(ddr2Regs->SDTIM1, DDR2_SDTIM1_T_RP,   3); // 12.5 ns * 256 MHz = 3.2
	CSL_FINS(ddr2Regs->SDTIM1, DDR2_SDTIM1_T_RCD,  3); // 12.5 ns * 256 MHz = 3.2
	CSL_FINS(ddr2Regs->SDTIM1, DDR2_SDTIM1_T_WR,   3); // 15.0 ns * 256 MHz = 3.84
	CSL_FINS(ddr2Regs->SDTIM1, DDR2_SDTIM1_T_RAS, 11); // 45.0 ns * 256 MHz = 11.52
	CSL_FINS(ddr2Regs->SDTIM1, DDR2_SDTIM1_T_RC,  14); // 57.5 ns * 256 MHz = 14.72
	CSL_FINS(ddr2Regs->SDTIM1, DDR2_SDTIM1_T_RRD,  3); // 11.5 ns * 256 MHz = 3.06
	CSL_FINS(ddr2Regs->SDTIM1, DDR2_SDTIM1_T_WTR,  3); //  7.5 ns * 256 MHz = 1.92 
	
	CSL_FINS(ddr2Regs->SDTIM2, DDR2_SDTIM2_T_ODT,   2); //  2 cycles
	CSL_FINS(ddr2Regs->SDTIM2, DDR2_SDTIM2_T_SXNR, 52); // 205 ns * 256 MHz = 52.48
	CSL_FINS(ddr2Regs->SDTIM2, DDR2_SDTIM2_T_SXRD,199); // 200 cycles
	CSL_FINS(ddr2Regs->SDTIM2, DDR2_SDTIM2_T_RTP,   1); //  7.5 ns * 256 MHz = 1.92
	CSL_FINS(ddr2Regs->SDTIM2, DDR2_SDTIM2_T_CKE,   2); //  3 cycles

	CSL_FINS(ddr2Regs->SDRFC, DDR2_SDRFC_REFRESH_RATE, 1997); // 256 MHz * 7.8 us = 1996.8
	
	CSL_FINS(ddr2Regs->DMCCTL, DDR2_DMCCTL_RL, 6);      // CAS Latency + 1

	CSL_FINST(ddr2Regs->SDCFG, DDR2_SDCFG_TIMUNLOCK, CLEAR);
}

void init_irq()
{
	CSL_IntcRegsOvly intcRegs = (CSL_IntcRegsOvly)CSL_INTC_0_REGS;

	// map GP[6] event (Mailbox write) to cpu int 8
	CSL_FINS(intcRegs->INTMUX2, INTC_INTMUX2_INTSEL8, CSL_INTC_EVENTID_GPINT6);

	// set ISTP to point to the vector table address
	ISTP = (unsigned int)intcVectorTable;
  
	// clear all interrupts, bits 4 thru 15
	ICR = 0xFFF0; 

	// enable the bits for non maskable interrupt and CPUINT6 and CPUINT8 */
	IER |= 0x02;
	IER |= 0x40;  // enable CPUINT6 (DMA completion)
	IER |= 0x100; // enable CPUINT8 (Mailbox write)
   
	// enable interrupts, set GIE bit 
	_enable_interrupts();
}

void init_dma(int indata_channels)
{
	CSL_IntcRegsOvly intcRegs = (CSL_IntcRegsOvly)CSL_INTC_0_REGS;

	CSL_Edma3ccRegsOvly edma3ccRegs = (CSL_Edma3ccRegsOvly)CSL_EDMA3CC_0_REGS;
	volatile CSL_Edma3ccParamsetRegs *ParmsetGPINT4RLD = &edma3ccRegs->PARAMSET[CSL_EDMA3_CHA_GPINT3]; // inbound data reload set
	volatile CSL_Edma3ccParamsetRegs *ParmsetGPINT4 = &edma3ccRegs->PARAMSET[CSL_EDMA3_CHA_GPINT4]; // inbound data
	volatile CSL_Edma3ccParamsetRegs *ParmsetGPINT5 = &edma3ccRegs->PARAMSET[CSL_EDMA3_CHA_GPINT5]; // outbound data

	CSL_FINS(edma3ccRegs->DCHMAP[52], EDMA3CC_DCHMAP_PAENTRY, 52); // Map event 52 to parameter set 52
	CSL_FINS(edma3ccRegs->DMAQNUM[6], EDMA3CC_DMAQNUM_E4, 0);      // Use Q0 for event 52

	CSL_FINS(edma3ccRegs->DCHMAP[53], EDMA3CC_DCHMAP_PAENTRY, 53); // Map event 53 to parameter set 53
	CSL_FINS(edma3ccRegs->DMAQNUM[6], EDMA3CC_DMAQNUM_E5, 0);      // Use Q0 for event 53
	
	memset((void *)ParmsetGPINT4RLD, 0, sizeof(CSL_Edma3ccParamsetRegs));
	memset((void *)ParmsetGPINT5, 0, sizeof(CSL_Edma3ccParamsetRegs));
	
	CSL_FINST(ParmsetGPINT4RLD->OPT, EDMA3CC_OPT_TCINTEN, ENABLE);
	CSL_FINST(ParmsetGPINT4RLD->OPT, EDMA3CC_OPT_STATIC, NORMAL);
	CSL_FINS(ParmsetGPINT4RLD->OPT, EDMA3CC_OPT_TCC, 52);			  // Use a TCC of 52 for event 52
	CSL_FINS(ParmsetGPINT4RLD->SRC, EDMA3CC_SRC_SRC, 0xb0000000);
	CSL_FINS(ParmsetGPINT4RLD->OPT, EDMA3CC_OPT_FWID, 2);
	CSL_FINST(ParmsetGPINT4RLD->OPT, EDMA3CC_OPT_SAM, CONST);
	CSL_FINS(ParmsetGPINT4RLD->A_B_CNT, EDMA3CC_A_B_CNT_ACNT, 4*indata_channels);
	CSL_FINS(ParmsetGPINT4RLD->A_B_CNT, EDMA3CC_A_B_CNT_BCNT, FRAMES_PER_LOOP);
	CSL_FINS(ParmsetGPINT4RLD->SRC_DST_BIDX, EDMA3CC_SRC_DST_BIDX_DSTBIDX, 4*indata_channels);
	CSL_FINS(ParmsetGPINT4RLD->DST, EDMA3CC_DST_DST, (Uint32)&MeaData);
	CSL_FINS(ParmsetGPINT4RLD->LINK_BCNTRLD, EDMA3CC_LINK_BCNTRLD_LINK, CSL_EDMA3_CHA_GPINT3 << 5);
//	CSL_FINS(ParmsetGPINT4RLD->LINK_BCNTRLD, EDMA3CC_LINK_BCNTRLD_BCNTRLD, FRAMES_PER_LOOP);
	CSL_FINS(ParmsetGPINT4RLD->CCNT, EDMA3CC_CCNT_CCNT, 1);

  	memcpy((void *)ParmsetGPINT4, (void *)ParmsetGPINT4RLD, sizeof(CSL_Edma3ccParamsetRegs));

	CSL_FINST(ParmsetGPINT5->OPT, EDMA3CC_OPT_TCINTEN, ENABLE);
	CSL_FINST(ParmsetGPINT5->OPT, EDMA3CC_OPT_STATIC, STATIC);
	CSL_FINS(ParmsetGPINT5->OPT, EDMA3CC_OPT_TCC, 53);			  // Use a TCC of 53 for event 53
	CSL_FINS(ParmsetGPINT5->SRC, EDMA3CC_SRC_SRC, (Uint32)&MonitorData);
	CSL_FINS(ParmsetGPINT5->A_B_CNT, EDMA3CC_A_B_CNT_ACNT, 0);
	CSL_FINS(ParmsetGPINT5->A_B_CNT, EDMA3CC_A_B_CNT_BCNT, 1);
	CSL_FINS(ParmsetGPINT5->DST, EDMA3CC_DST_DST, 0xb0000000);
	CSL_FINS(ParmsetGPINT5->LINK_BCNTRLD, EDMA3CC_LINK_BCNTRLD_LINK, 0xffff);
	CSL_FINS(ParmsetGPINT5->CCNT, EDMA3CC_CCNT_CCNT, 1);

  	CSL_FINST(edma3ccRegs->EMCRH, EDMA3CC_EMCRH_E52, CLEAR);	// clear missed events)
  	CSL_FINST(edma3ccRegs->EMCRH, EDMA3CC_EMCRH_E53, CLEAR);	// clear missed events)

  	CSL_FINST(edma3ccRegs->SECRH, EDMA3CC_SECRH_E52, CLEAR);	// clear secondary events)
  	CSL_FINST(edma3ccRegs->SECRH, EDMA3CC_SECRH_E53, CLEAR);	// clear secondary events)

//	CSL_FINST(edma3ccRegs->ESRH, EDMA3CC_ESRH_E52, SET);	// Manual Trigger
	CSL_FINST(edma3ccRegs->EESRH, EDMA3CC_EESRH_E52, SET);	// enable event
	CSL_FINST(edma3ccRegs->ICRH,  EDMA3CC_ICRH_I52, CLEAR);	// clear pending interrupts
	CSL_FINST(edma3ccRegs->IESRH, EDMA3CC_IESRH_I52, SET);	// Enable interrupt for event 52
	
		// map DMA global completion interrupt to cpu int6
	CSL_FINS(intcRegs->INTMUX1, INTC_INTMUX1_INTSEL6, CSL_INTC_EVENTID_EDMA3CC_GINT);
}

void init_qdma(int indata_channels)
{
	CSL_Edma3ccRegsOvly edma3ccRegs = (CSL_Edma3ccRegsOvly)CSL_EDMA3CC_0_REGS;

	volatile CSL_Edma3ccParamsetRegs *ParmsetQDMA = &edma3ccRegs->PARAMSET[10]; // QDMA

	CSL_FINS(edma3ccRegs->QCHMAP[0], EDMA3CC_QCHMAP_PAENTRY, 10); // Map QDMA 0 to parameter set 10
	CSL_FINS(edma3ccRegs->QCHMAP[0], EDMA3CC_QCHMAP_TRWORD, 3);   // Define Trigger Word for QDMA 10, 3: DST
	CSL_FINS(edma3ccRegs->QDMAQNUM, EDMA3CC_QDMAQNUM_E0, 0);      // Use Q0 for QDMA 0

	CSL_FINST(edma3ccRegs->QEECR, EDMA3CC_QEECR_E0, CLEAR);	      // disable QDMA 0 event

	memset((void *)ParmsetQDMA, 0, sizeof(CSL_Edma3ccParamsetRegs));
	CSL_FINST(ParmsetQDMA->OPT, EDMA3CC_OPT_TCINTEN, ENABLE);
	CSL_FINST(ParmsetQDMA->OPT, EDMA3CC_OPT_STATIC, STATIC);
	CSL_FINST(ParmsetQDMA->OPT, EDMA3CC_OPT_SYNCDIM, ABSYNC);
	CSL_FINS(ParmsetQDMA->OPT, EDMA3CC_OPT_TCC, 10);			      // Use a TCC of 10 for QDMA 10
	CSL_FINS(ParmsetQDMA->SRC, EDMA3CC_SRC_SRC, (Uint32)&MeaData);
	CSL_FINS(ParmsetQDMA->A_B_CNT, EDMA3CC_A_B_CNT_ACNT, 4*indata_channels);
	CSL_FINS(ParmsetQDMA->A_B_CNT, EDMA3CC_A_B_CNT_BCNT, FRAMES_PER_LOOP/DOWNSAMPLE);
	CSL_FINS(ParmsetQDMA->SRC_DST_BIDX, EDMA3CC_SRC_DST_BIDX_DSTBIDX, 4*indata_channels*DOWNSAMPLE); // use only every DOWNSAMPLE. Frame from Source
	CSL_FINS(ParmsetQDMA->SRC_DST_BIDX, EDMA3CC_SRC_DST_BIDX_DSTBIDX, 4*indata_channels);	 // Destination is continous
	CSL_FINS(ParmsetQDMA->LINK_BCNTRLD, EDMA3CC_LINK_BCNTRLD_LINK, 0xffff);
	CSL_FINS(ParmsetQDMA->CCNT, EDMA3CC_CCNT_CCNT, 1);

  	CSL_FINST(edma3ccRegs->QEMCR, EDMA3CC_QEMCR_E0, CLEAR);  // clear missed events
  	CSL_FINST(edma3ccRegs->QSECR, EDMA3CC_QSECR_E0, CLEAR);  // clear secondary events

	CSL_FINST(edma3ccRegs->ICR,   EDMA3CC_ICR_I10, CLEAR);   // clear pending interrupts
	CSL_FINST(edma3ccRegs->QEESR, EDMA3CC_QEESR_E0, SET);	 // enable QDMA event

//	CSL_FINS(ParmsetQDMA->DST, EDMA3CC_DST_DST, (Uint32)MatlabPointer); // write trigger word, start transfer
//	while (CSL_FEXT(edma3ccRegs->IPR, EDMA3CC_IPR_I10) == 0); // wait for completion
//	CSL_FINST(edma3ccRegs->ICR,  EDMA3CC_ICR_I10, CLEAR);	  // clear pending interrupts
}

void timer_setperiod(int period)
{
    CSL_TmrRegsOvly tmr1Regs = (CSL_TmrRegsOvly)CSL_TMR_1_REGS;

    CSL_FINST(tmr1Regs->TCR, TMR_TCR_ENAMODE_LO, DISABLE);
    CSL_FINS(tmr1Regs->PRDLO,TMR_PRDLO_PRDLO, period);
    CSL_FINST(tmr1Regs->TIMLO,TMR_TIMLO_TIMLO,RESETVAL);
    CSL_FINS(tmr1Regs->TCR, TMR_TCR_ENAMODE_LO, 2);  // continous mode
}



void SetMonitorSize(int datapoints)
{
	CSL_Edma3ccRegsOvly edma3ccRegs = (CSL_Edma3ccRegsOvly)CSL_EDMA3CC_0_REGS;
	volatile CSL_Edma3ccParamsetRegs *ParmsetGPINT5 = &edma3ccRegs->PARAMSET[CSL_EDMA3_CHA_GPINT5]; // outbound data

	CSL_FINS(ParmsetGPINT5->A_B_CNT, EDMA3CC_A_B_CNT_ACNT, 0);
	WRITE_REGISTER(DSP_OUTDATA_THR, datapoints);
	WRITE_REGISTER(DSP_OUTDATA_CTRL, 3);				  // Disable all Data Channels and Clear Fifo
	CSL_FINS(ParmsetGPINT5->A_B_CNT, EDMA3CC_A_B_CNT_ACNT, 4*datapoints);
}
