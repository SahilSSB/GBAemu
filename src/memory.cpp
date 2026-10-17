#include "../include/memory.hpp"
#include <iostream>
#include <fstream>

Memory::Memory():   bios(BIOS_SIZE),
                    ewram(EWRAM_SIZE),
                    iwram(IWRAM_SIZE)
{
  std::cout << "memory constructor called" << "\n";
}

bool Memory::loadROM(const std::string &filename) {
  std::cout << "loading rom: " << filename << "\n";
  
  std::ifstream file(filename, std::ios::binary);
  
  // file size
  file.seekg(0, std::ios::end);
  size_t fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  rom.resize(fileSize); 
  if (file.read(reinterpret_cast<char*>(rom.data()), fileSize)) return true;

  return false;
}
