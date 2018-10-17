#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

void cpu_ram_read()
{

}

void cpu_ram_write()
{

}
void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp;
  char line[1024];
  int address = ADDR_PROGRAM_ENTRY;

  // Opens the source file
  if ((fp = fopen(filename, "r")) == NULL)
  {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(2);
  }

  // Reads each line of the file and stores it in RAM
  while (fgets(line, sizeof line, fp) != NULL)
  {
    // Convert string to a number
    char *endchar;
    unsigned char byte = strtol(line, &endchar, 2);

    // Ignores line where there are no numbers to read
    if (endchar == line)
    {
      continue;
    }


    cpu->ram[address++] = byte;
  }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->reg[regA] *= cpu->reg[regB];
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    unsigned char IR = cpu->ram[cpu->PC];

    unsigned char operandA = cpu->ram[(cpu->PC + 1) & 0xff];
    unsigned char operandB = cpu->ram[(cpu->PC + 2) & 0xff];

    int instruction_set_pc = (IR >> 4) & 1;

    switch(IR)
    {
      case LDI:
        cpu->reg[operandA] = operandB;
        break;

      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        break;

      case HLT:
        running = 0;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      default:
        fprintf(stderr, "PC %02x: unknown instruction %02x\n", cpu->PC, IR);
        break;
    }
    if (!instruction_set_pc)
    {
      cpu->PC += ((IR >> 6) & 0x3) + 1;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  // Zero registers and RAM
  memset(cpu->reg, 0, sizeof cpu->reg);
  memset(cpu->ram, 0, sizeof cpu->ram);
}
