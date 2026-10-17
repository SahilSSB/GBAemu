#include "../include/arm.hpp"
#include "../include/memory.hpp"
#include <cstdlib>
#include <iomanip>
#include <iostream>

std::vector<ARM::instructionEntry> CPU::buildARMTable() {
  return {
    {0x0E000000, 0x0A000000, ARM::armBranch, "B/BL"},
  };
}

void CPU::step() {
  u32 addr = regs.R[PC];
  u32 instruction = mem.read32Bits(regs.R[15]);
  regs.R[15] += 4;

  if ((instruction >> 28) != 0xE) { // temporarily for these instructions
    std::cout << std::hex << addr << " condition" << (instruction >> 28) 
              << " not implemented\n";
    exit(1);
  }

  std::cout << std::hex << std::setw(8) << std::setfill('0') << addr
          << ": " << std::setw(8) << instruction << "\n";

  for (auto e: buildARMTable()) {
    if ((instruction & e.mask) == e.pattern) {
      e.handler(this, &mem, instruction);
      return;
    }
  }

  std::cout << std::hex << std::setw(8) << std::setfill('0') << addr <<
               ": " << std::setw(8) << instruction << " <unimplemented>\n";
  exit(1);
}

u32 Memory::read32Bits(uint32_t addr) {
  u32 offset = addr & 0x01FFFFFF;
  u32 result = 0;
  for (int i = 0; i < 4; i++) {
    result |= (u32)rom[offset + i] << (8 * i);
  }
  return result;
}

void ARM::armBranch(CPU *cpu, Memory *memory, u32 instr) {
  u32 offset = instr & 0x00FFFFFF;  // 24 bit offset

  if (offset & 0x00800000)          // if bit 23 is -ve  
    offset |= 0xFF000000;           // make the preceeding bits as 1

  offset <<= 2;                     
  
  if(CHECK_BIT(instr, 24))          
    cpu->regs.R[LR] = cpu->regs.R[PC];

  cpu->regs.R[PC] += 4 + offset;
}

void ARM::armBlockDataTransfer(CPU *cpu, Memory *memory, u32 instr) {
  u32 offset = instr & 0x00FFFFFF;

}
