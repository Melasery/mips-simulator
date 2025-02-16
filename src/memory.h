#ifndef MEMORY_H_
#define MEMORY_H_

#include <stdint.h>

#define MEMSIZE         4294967296              	/* 4GB addressable using 32 bits */

extern uint8_t *memory;                         	/* 4GB of flat memory */

uint8_t read_byte(uint32_t address);

void write_byte(uint32_t address, uint8_t data);

uint32_t read_word(uint32_t address);

void write_word(uint32_t address, uint32_t data);

#endif
