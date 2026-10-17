#pragma once

#include "common.hpp"
#include "arm.hpp"

constexpr u32 BIOS_SIZE     = 16*1024;
constexpr u32 EWRAM_SIZE    = 256*1024;
constexpr u32 IWRAM_SIZE    = 32*1024;
constexpr u32 ROM_SRAM_SIZE = 64*1024; 

#define BIOS_START    0x00000000
#define BIOS_END      0x00003FFF

#define EWRAM_START   0x02000000
#define EWRAM_END     0x0203FFFF

#define IWRAM_START   0x03000000
#define IWRAM_END     0x03007FFF

#define IO_START      0x04000000
#define IO_END        0x040003FE

#define ROM_W0_START  0x08000000  // wait state 0
#define ROM_W0_END    0x09FFFFFF

#define ROM_W1_START  0X0A000000  // wait state 1
#define ROM_W1_END    0x0BFFFFFF

#define ROM_W2_START  0X0C000000  // wait state 2
#define ROM_W2_END    0x0DFFFFFF

constexpr int SP = 13, LR = 14, PC = 15;  

constexpr u32 EWRAM_MASK = EWRAM_SIZE - 1;  // 0x3FFFF
constexpr u32 IWRAM_MASK = IWRAM_SIZE -1;   // 0x7FFF

typedef struct Registers {
  // general purpose registers including sp(13), pc(15) and lr(14)
  u32 R[16] {};  
  u32 CPSR {0x1F};  // current program status
} Registers;

class Memory {
  private:
    std::vector<u8> bios    = std::vector<u8>(BIOS_SIZE);
    std::vector<u8> ewram   = std::vector<u8>(EWRAM_SIZE);
    std::vector<u8> iwram   = std::vector<u8>(IWRAM_SIZE);
    std::vector<u8> romSRAM = std::vector<u8>(ROM_SRAM_SIZE); 
    std::vector<u8> rom; 

  public:
    Memory();
    
    bool loadROM(const std::string &filename);
    u32 read32Bits(u32 addr);
    void write32Bits(u32 addr, u32 val);
};

class CPU {
  private:
    Memory    &mem;

  public:
    CPU(Memory &m);
    Registers regs;

    void step();
    std::vector<ARM::instructionEntry> buildARMTable();
};
