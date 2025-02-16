#ifndef LOADER_H_
#define LOADER_H_

#include <stdint.h>
#include <stdio.h>

enum Segment{USER_TEXT, KERNEL_TEXT, USER_DATA, USER_STACK, KERNEL_DATA};

#define USER_TEXT_BASE 0x00400000
#define USER_TEXT_MASK 0x03FFFFFF

#define USER_DATA_BASE 0x10010000
#define USER_DATA_MASK 0x03FFFFFF

uint8_t read_file(FILE *);

#endif
