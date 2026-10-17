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

 
static u32 readLE32(const std::vector<u8>& v, u32 offset) {
  u32 result = 0;
  for (int i = 0; i < 4; i++)
    result |= (u32)v[offset + i] << (8 * i);
  return result;
}

u32 Memory::read32Bits(u32 addr) {
  switch (addr >> 24) {
    case 0x03: {  // for iwram
                 return readLE32(iwram, addr & IWRAM_MASK);
               }
    case 0x02: {  // for ewram
                 return readLE32(ewram, addr & EWRAM_MASK); 
               }
    case 0x08: case 0x09:
    case 0x0A: case 0x0B:
    case 0x0C: case 0x0D: {
                            u32 offset = addr & 0x01FFFFFF;
                            if (offset + 3 >= rom.size()) {
                              std::cout << "rom read out of range: " << std::hex << addr << "\n";
                              exit(1);
                            }
                            return readLE32(rom, offset);
                          }
    default:
               std::cout << "unmapped read: " << std::hex << addr << "\n";
               exit(1);
  }
}

void Memory::write32Bits(u32 addr, u32 val) {
  u32 bytes = addr >> 24;
  switch (bytes) {
    case 0x03: {  // for iwram
                 u32 offset = addr & IWRAM_MASK;
                 for (int i = 0; i < 4; i++) 
                   iwram[offset + i] = (val >> (8 * i)) & 0xFF;
                 return;
               }
    case 0x02: { // for ewram 
                 u32 offset = addr & EWRAM_MASK;
                 for (int i = 0; i < 4; i++)
                   ewram[offset + i] = (val >> (8 * i)) & 0xFF;
                 return;
               }
    default:
               std::cout << "unmapped write: " << std::hex << addr << "\n";
               exit(1);
  }
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

// void ARM::armBlockDataTransfer(CPU *cpu, Memory *memory, u32 instr) {
// }
