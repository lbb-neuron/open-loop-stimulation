#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#define DAC8555_CHANNELS 4

typedef Uint16 uint16;
typedef Uint32 uint32;
typedef Uint8 uint8;

typedef Int32 int32;
typedef Int16 int16;
typedef Int8  int8;

typedef struct dac_command
{
	Uint16 value;
	Uint16 command;
} dac_command;

typedef struct dac8555_data
{
	dac_command dac[DAC8555_CHANNELS];
} dac8555_data;

#endif
