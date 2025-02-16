#include <stdint.h>
#include <stdio.h>

#include "memory.h"

uint8_t read_byte(uint32_t address)
{
   return (memory[address]);
}

void write_byte(uint32_t address, uint8_t data)
{
   memory[address] = data;
}

uint32_t read_word(uint32_t address)
{
   return ((uint32_t) memory[address]) | 
	  (((uint32_t) memory[address+1]) << 8) | 
 	  (((uint32_t) memory[address+2]) << 16) | 
 	  (((uint32_t) memory[address+3]) << 24);
}

void write_word(uint32_t address, uint32_t data)
{
   memory[address]   = (uint8_t) (data & 0x000000FF);
   memory[address+1] = (uint8_t) ((data & 0x0000FF00) >> 8);
   memory[address+2] = (uint8_t) ((data & 0x00FF0000) >> 16);
   memory[address+3] = (uint8_t) ((data & 0xFF000000) >> 24);
}
