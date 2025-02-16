#include "interpreter.h"
#include "global.h"
#include "memory.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h> /* for isdigit */

/* read and return a non-negative integer from stdin */
int readInt(void)
{
    int x = 0; /* return value accumulator */
    int c;

    while ((c = getchar()) != EOF)
    {
        if (isdigit(c))
        {
            /* assumes '0'..'9' are contiguous */
            x = x * 10 + (c - '0');
        }
        else // if not a digit
        {
            /* put it back */
            ungetc(c, stdin);

            /* escapes from innermost loop */
            break;
        }
    }

    /* Flush stdin */
    while ((c = getchar()) != '\n' && c != EOF){

    }

    return x;
}

/* Convenience function to fetch instruction from address in PC */
uint32_t
fetch(uint32_t pc)
{
    return read_word(pc);
}

/*
   This is the main interpreter function, which decodes and executes MIPS instructions
*/
void run(uint32_t pc)
{
    /* Fetch - decode - execute loop */
    bool terminate = false;
    bool jump = false;

    do
    {
        /* Fetch instruction */
        uint32_t instr = fetch(pc);

        #ifdef DEBUG
        fprintf(stderr, "[0x%08X]        0x%08X    ", pc, instr);
        #endif

        /* Decode and execute instruction */
        if (instr == 0)
        {
            #ifdef DEBUG
            fprintf(stderr, "nop\n");
            #endif
        }
        else
        {
            uint32_t opcode = instr >> 26;

            switch (opcode)
            {
            case 0: /* R - format:RS, RT, RD, SHAMT, FUNC */
            {
                uint32_t rs = (instr & 0x3E00000) >> 21;
                uint32_t rt = (instr & 0x1F0000) >> 16;
                uint32_t rd = (instr & 0xF800) >> 11;
                uint32_t shamt = (instr & 0x7C0) >> 6;
                uint32_t func = instr & 0x3F;


                switch (func)
                {

                    case 0x21: //addu 
                    {   

                        registers[rd] = registers[rt] + registers[rs];
                        #ifdef DEBUG
                            fprintf(stderr, "addu $%u, $%u, $%u\n", rd, rs, rt);
                        #endif
                        break;
                    }
                    case 0x20: //add 
                    {   

                        int32_t result = (int32_t)registers[rt] + (int32_t)registers[rs];

                        registers[rd] = (uint32_t)result;
                        
                        #ifdef DEBUG
                            fprintf(stderr, "add $%u, $%u, $%u\n", rd, rs, rt);
                        #endif
                        break;
                    }
                    case 0x00: // sll instr
                    {
                        registers[rd] = registers[rt] << shamt;
                        
                        #ifdef DEBUG
                            fprintf(stderr, "sll $%u, $%u, %u\n", rd, rt, shamt);
                        #endif
                        
                        break; 
                    }
                    case 0x22: // sub
                    {
                        int32_t result = (int32_t)registers[rs] - (int32_t)registers[rt];
                        
                        registers[rd] = (uint32_t)result;
                        
                        #ifdef DEBUG
                        fprintf(stderr, "sub $%u, $%u, $%u\n", rd, rs, rt);
                        #endif
                        
                        break;
                    }
                    case 0X08: // jr instr
                    {
                        pc = registers[rs];
                        
                        #ifdef DEBUG
                            fprintf(stderr, "jr $%u\n", rs);
                        #endif

                        continue;
                        
                    }
                    case 0x25: // or
                    {
                        
                        registers[rd] = registers[rs] | registers[rt];
                        break;
                    }

                    case 0x2A: // slt 
                    {
                        if ((int32_t)registers[rs] < (int32_t)registers[rt]) {
                            registers[rd] = 1; 
                        } else {
                            registers[rd] = 0; 
                        }

                        #ifdef DEBUG
                            fprintf(stderr, "slt $%u, $%u, $%u (result = %d)\n", rd, rs, rt, registers[rd]);
                        #endif

                        break;
                    }

                    case 12: // SYSCALL
                    {
                        #ifdef DEBUG
                        fprintf(stderr, "syscall\n");
                        #endif
                        switch (registers[2])
                        {
                            case 1: // print_int
                            {
                                printf("%d", registers[4]);
                                break;
                            }
                            case 4: // print_string
                            {
                                const char *s = (const char *)&(memory[registers[4]]);
                                printf("%s", s);
                                break;
                            }
                            case 5: // read_int
                            {
                                int32_t input = readInt();
                                registers[2] = input;
                                break;
                            }
                            case 10: // exit
                            {
                                terminate = true;
                                break;
                            }
                            case 11: // print_character
                            {
                                printf("%c", (char)registers[4]);
                                break;
                            }
                            default:
                                fprintf(stderr, "Unimplemented syscall. Terminate.\n");
                                exit(1);
                        }
                        break;
                    } 	    
	                break;
                }


                break;
            } 

            case 0x23: // lw  instr
            {
                uint32_t rs = (instr >> 21) & 0x1F;  
                uint32_t rt = (instr >> 16) & 0x1F;  
                int16_t offset = (instr & 0xFFFF);   

                // Compute effective memory address
                uint32_t address = registers[rs] + offset;

                // Load the word from memory
                registers[rt] = read_word(address);

                #ifdef DEBUG
                    fprintf(stderr, "lw $%u, %d($%u)\n", rt, offset, rs);
                #endif

                break;
            } 
            case 0x2B: // sw instr
            {   
                uint32_t rs = (instr >> 21) & 0x1F;  
                uint32_t rt = (instr >> 16) & 0x1F;  
                int16_t offset = (instr & 0xFFFF);   

                uint32_t address = registers[rs] + offset;
                write_word(address, registers[rt]);

                    #ifdef DEBUG
                        fprintf(stderr, "sw $%u, %d($%u)\n", rt, offset, rs);
                    #endif

                break;
            
            }
            case 0x09: // addiu
            {
                uint32_t rs = (instr >> 21) & 0x1F;  
                uint32_t rt = (instr >> 16) & 0x1F;  
                int16_t immediate = (instr & 0xFFFF);  

                registers[rt] = registers[rs] + immediate;

                #ifdef DEBUG
                    fprintf(stderr, "addiu $%u, $%u, %d\n", rt, rs, immediate);
                #endif

                break;
            }
            case 0x08: // addi
            {
                uint32_t rs = (instr >> 21) & 0x1F;
                uint32_t rt = (instr >> 16) & 0x1F;
                int16_t immediate = (instr & 0xFFFF);

                registers[rt] = registers[rs] + immediate;

                #ifdef DEBUG
                fprintf(stderr, "addi $%u, $%u, %d\n", rt, rs, immediate);
                #endif

                break;
            }
            case 0x03: // jal instr 
            {
                uint32_t target = instr & 0x03FFFFFF;  
                
                registers[31] = pc + 4;
                uint32_t target_address = ((pc + 4) & 0xF0000000) | (target << 2);
                pc = target_address;


                #ifdef DEBUG
                    fprintf(stderr, "jal 0x%08X\n", target_address);
                #endif

                continue; 

            }

            case 0x0F: // lui
            {

                uint32_t rt = (instr >> 16) & 0x1F;  
                uint16_t immediate = (instr & 0xFFFF);  

                registers[rt] = immediate << 16;

                #ifdef DEBUG
                    fprintf(stderr, "lui $%u, %u\n", rt, immediate);
                #endif

                break;
            }

            case 0x0D: //ori
            {
                uint32_t rs = (instr >> 21) & 0x1F;  
                uint32_t rt = (instr >> 16) & 0x1F;  
                uint16_t immediate = (instr & 0xFFFF);  

                registers[rt] = registers[rs] | immediate;

                #ifdef DEBUG
                    fprintf(stderr, "ori $%u, $%u, %u\n", rt, rs, immediate);
                #endif

                break;
            }
            case 0x04: // beq 
            {
                uint32_t rs = (instr >> 21) & 0x1F; 
                uint32_t rt = (instr >> 16) & 0x1F; 
                int16_t immediate = (instr & 0xFFFF);
                
                if (registers[rs] == registers[rt]) {
                    pc = pc + 4 + ((int32_t)immediate << 2);
                 
                    #ifdef DEBUG
                        fprintf(stderr, "beq $%u, $%u, 0x%X (branch taken to 0x%08X)\n", 
                                rs, rt, immediate, pc);
                    #endif
                    continue;  
                } else {
                    #ifdef DEBUG
                        fprintf(stderr, "beq $%u, $%u, 0x%X (no branch)\n", rs, rt, immediate);
                    #endif
                }
                break;
            }

            case 0x05: // bne 
            {
                uint32_t rs = (instr >> 21) & 0x1F; 
                uint32_t rt = (instr >> 16) & 0x1F; 
                int16_t immediate = (instr & 0xFFFF);
                
                if (registers[rs] != registers[rt]) {
                    pc = pc + 4 + ((int32_t)immediate << 2);
                    
                    #ifdef DEBUG
                        fprintf(stderr, "bne $%u, $%u, 0x%X (branch taken to 0x%08X)\n", 
                                rs, rt, immediate, pc);
                    #endif
                    continue;  
                } else {
                    #ifdef DEBUG
                        fprintf(stderr, "bne $%u, $%u, 0x%X (no branch)\n", rs, rt, immediate);
                    #endif
                }
                break;
            }
            case 0x1C: // special2 cat
            {
                uint32_t rs = (instr >> 21) & 0x1F; 
                uint32_t rt = (instr >> 16) & 0x1F; 
                uint32_t rd = (instr >> 11) & 0x1F; 
                uint32_t funct = instr & 0x3F;      

                switch (funct) {
                    case 0x02: // mul 
                    {
                        int32_t result = (int32_t)registers[rs] * (int32_t)registers[rt];
                        registers[rd] = (uint32_t)result;

                        #ifdef DEBUG
                        fprintf(stderr, "mul $%u, $%u, $%u (result = %d)\n", rd, rs, rt, result);
                        #endif

                        break;
                    }
                }

                break;
            }

            case 0x02: // j 
            {
                uint32_t target = instr & 0x03FFFFFF;
                uint32_t target_address = ((pc + 4) & 0xF0000000) | (target << 2);
                pc = target_address;
                
                #ifdef DEBUG
                    fprintf(stderr, "j 0x%08X\n", target_address);
                #endif
                
                continue;  
            }

            case 0x0A: // slti
            {
                uint32_t rs = (instr >> 21) & 0x1F;
                uint32_t rt = (instr >> 16) & 0x1F;
                int16_t immediate = (instr & 0xFFFF);
                
                int32_t signExtImm = (int32_t)immediate;
                
                if (rt != 0) {
                    if ((int32_t)registers[rs] < signExtImm) {
                        registers[rt] = 1;
                    } else {
                        registers[rt] = 0;
                    }
                }
                
                #ifdef DEBUG
                    fprintf(stderr, "slti $%u, $%u, %d (rs=0x%08X, imm=%d, result=%d)\n",
                            rt, rs, signExtImm, registers[rs], signExtImm, registers[rt]);
                #endif
                
                break;
            }
            
            default: 
                // Bail out if unrecognised instruction is encountered
                printf("Boom: 0x%x\n", instr);
                exit(1);
            }
        }

        /* Increment pc */
        pc += 4;

    } while (!terminate);

    /* Dump register file */
void print_registers() {
    printf("\nRegisters:\n");
    for (int i = 0; i < 32; i++) {
        printf("$%-2d : %08X\n", i, registers[i]); 
    }
    }
    print_registers();
}
