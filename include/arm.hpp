#pragma once

#include "common.hpp"

#define EXTRACT_BITS(value, start, count) \
  (((value) >> (start)) & ((1 << (count)) - 1))
#define CHECK_BIT(value, bit) (((value) >> (bit)) & 1)

class CPU;
class Memory;

namespace ARM {
  typedef void (*instructionHandler)(CPU* cpu, Memory* memory, u32 instr);

  struct instructionEntry {
    u32 mask;
    u32 pattern;
    instructionHandler handler;
    std::string name; 
  };
  
  void armBranch(CPU* cpu, Memory* memory, u32 instr);
  void armBlockDataTransfer(CPU* cpu, Memory* memory, u32 instr);
}
