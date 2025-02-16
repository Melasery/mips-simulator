# MIPS Instruction Set Simulator

## Overview
This project is a MIPS instruction set simulator written in C. It implements a subset of the MIPS instruction set, allowing execution of MIPS machine code instruction-by-instruction, simulating the behavior of a real MIPS processor. The simulator follows the fetch-decode-execute cycle and supports system calls for basic I/O.

## Features
- Implements key MIPS instructions: arithmetic, logical, memory, and control flow.
- Supports **fetch-decode-execute cycle**.
- Handles system calls (printing, reading input, exit).
- Debugging support (`-DDEBUG` mode prints execution trace).
- Can load and run assembled MIPS programs.

## Fetch-Decode-Execute Cycle
1. **Fetch**: Read instruction from memory.
2. **Decode**: Extract opcode, operands, and immediate values.
3. **Execute**: Perform the corresponding operation.
4. **Update PC**: Move to the next instruction.

## Setup & Compilation

Clone the repository and build the project:
```bash
# Clone the repository
git clone https://github.com/Melasery/mips-simulator.git
cd mips-simulator

# Build the simulator
make
```

## Running the Simulator
Run an assembled MIPS program:
```bash
./assemble_and_run.sh examples/hello_world.s
```

## Example Debug Trace Output
When running in debug mode (`-DDEBUG`), the simulator prints an execution trace:
```
[0x00400000] 0x8FA40000 lw $4, 0($29)
[0x00400004] 0x27A50004 addiu $5, $29, 4
[0x00400008] 0x24A60004 addiu $6, $5, 4
[0x0040000C] 0x00041080 sll $2, $4, 2
[0x00400010] 0x00C23021 addu $6, $6, $2
[0x00400014] 0x0C100068 jal 0x4001a0
[0x004001A0] 0x24020004 addiu $2, $0, 4
[0x004001A4] 0x3C011001 lui $1, 4097
[0x004001A8] 0x34240000 ori $4, $1, 0
[0x004001AC] 0x0000000C syscall
Hello World
[0x004001B0] 0x3C011000 lui $1, 4096
[0x004001B4] 0x8C290000 lw $9, 0($1)
[0x004001B8] 0x03E00008 jr $31
[0x00400018] 0x00000000 nop
[0x0040001C] 0x2402000A addiu $2, $0, 10
[0x00400020] 0x0000000C syscall
```

## Example Register Dump
At the end of execution, the simulator prints register values:
```
$0  : 0x00000000
$1  : 0x10000000
$2  : 0x0000000a
$3  : 0x00000000
$4  : 0x10010000
$5  : 0x7ffff000
$6  : 0x7ffff004
$7  : 0x00000000
$8  : 0x00000000
$9  : 0x00000000
$10 : 0x00000000
$11 : 0x00000000
$12 : 0x00000000
$13 : 0x00000000
$14 : 0x00000000
$15 : 0x00000000
$16 : 0x00000000
$17 : 0x00000000
$18 : 0x00000000
$19 : 0x00000000
$20 : 0x00000000
$21 : 0x00000000
$22 : 0x00000000
$23 : 0x00000000
$24 : 0x00000000
$25 : 0x00000000
$26 : 0x00000000
$27 : 0x00000000
$28 : 0x10008000
$29 : 0x7fffeffc
$30 : 0x00000000
$31 : 0x004001b0
```

## Supported Instructions
| Instruction | Description |
|-------------|-------------|
| `add` | Addition |
| `sub` | Subtraction |
| `lw`  | Load word |
| `sw`  | Store word |
| `syscall` | System calls |
| `beq` | Branch if equal |
| `bne` | Branch if not equal |
| `jal` | Jump and link |
| `jr` | Jump register |
| `lui` | Load upper immediate |
| `ori` | OR immediate |
| `sll` | Shift left logical |

## File Structure
```
/mips-simulator
│── /src/                  # Source code
│    ├── sim.c             # Main program
│    ├── memory.c          # Memory handling
│    ├── memory.h          # Memory header
│    ├── loader.c          # File loader
│    ├── loader.h          # Loader header
│    ├── interpreter.c     # MIPS instruction execution
│    ├── interpreter.h     # Interpreter header
│    ├── global.h          # Global declarations
│── /examples/hello_world/  # MIPS test programs
│    ├── hello.s           # Example: prints "Hello World"
│    ├── libio.s           # I/O library
│    ├── startup.s         # Startup code
│── .gitignore              # Ignore compiled binaries
│── LICENSE                 # Open-source license (optional)
│── Mars4_5.jar             # MARS simulator
│── README.md               # Main documentation
│── assemble_and_run.sh      # Assemble & execute programs
│── run_mars.sh             # Run with MARS emulator
```

