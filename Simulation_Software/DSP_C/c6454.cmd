/****************************************************************************/
/*  lnk.cmd   v5.00                                                         */
/*  Copyright (c) 1996-2005  Texas Instruments Incorporated                 */
/*    Usage:  lnk6x <obj files...>    -o <out file> -m <map file> lnk.cmd   */
/*            cl6x  <src files...> -z -o <out file> -m <map file> lnk.cmd   */
/*                                                                          */
/*    Description: This file is a sample linker command file that can be    */
/*                 used for linking programs built with the C compiler and  */
/*                 running the resulting .out file on a C620x/C670x         */
/*                 simulator.  Use it as a guideline.  You will want to     */
/*                 change the memory layout to match your specific C6xxx    */
/*                 target system.  You may want to change the allocation    */
/*                 scheme according to the size of your program.            */
/*                                                                          */
/*    Notes: (1)   You must specivy a directory in which rts6x00.lib is     */
/*                 located.  either add a -i"<directory>" line to this      */
/*                 file or use the system environment variable C_DIR to     */
/*                 specify a search path for the libraries.                 */
/*                                                                          */
/*           (2)   If the run-time library you are using is not named       */
/*                 rts6200[e].lib, rts6400[e].lib, or rts6700[e].lib, be    */
/*                 sure to use the correct name here.                       */
/*                                                                          */
/****************************************************************************/
-c
-heap  0x2000
-stack 0xA000

/* Memory Map 1 - the default */
MEMORY
{
        L1D:     o = 00f00000h   l = 00008000h  /* 32 KByte */
        L1P:     o = 00e00000h   l = 00008000h  /* 32 KByte */
        L2:      o = 00800000h   l = 000F0000h  /*  1 Mbyte */
        DDR:     o = 0e0000000h   l = 10000000h  /* 256 MByte */
} 

SECTIONS
{
    .csl_vect   >       L2
    .text       >       L2
    .stack      >       L2
    .bss        >       L2
    .cinit      >       L2
    .cio        >       L2
    .const      >       L2
    .data       >       L2
    .switch     >       L2
    .sysmem     >       L2
    .far        >       L2
    .testMem    >       L2   
    .intvecs    >       L2
     .dataddr    >       DDR
}
