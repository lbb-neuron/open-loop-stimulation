#include "led.h"
#include "time.h"

void led_init()
{
    WRITE_REGISTER(0x002C, 0x0700);
    led_status(LED_ON);
}

void led_status(unsigned int status)
{
    unsigned int reg = READ_REGISTER(0x002C);
    if(status)
        reg |= 0x0001;
    else
        reg &= 0xFFFE;
    WRITE_REGISTER(0x002C, reg);
}

void led_channel(unsigned int chan1, unsigned int chan2)
{
    unsigned int reg = READ_REGISTER(0x002C);
    if(chan1)
        reg |= 0x0004;
    else
        reg &= 0xFFFB;
    if(chan2)
        reg |= 0x0002;
    else
        reg &= 0xFFFD;
    WRITE_REGISTER(0x002C, reg);
}

void led_inform_dsp_ready()
{
    int i;
    for(i=0;i<2;i++)
    {
        led_status(LED_ON);
        led_channel(LED_OFF,LED_OFF);
        timer_delay_ms(333);
        led_status(LED_OFF);
        led_channel(LED_ON,LED_OFF);
        timer_delay_ms(333);
        led_channel(LED_OFF,LED_ON);
        timer_delay_ms(333);
    }
    led_status(LED_ON);
    led_channel(LED_OFF,LED_OFF);
}

