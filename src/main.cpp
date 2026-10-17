#include "../include/memory.hpp"
#include <SDL3/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


CPU::CPU(Memory& m) : mem(m) {regs.R[PC] = 0x08000000;} 

int main(int argc, char** argv) {
  Memory mem;
  CPU cpu(mem);
  // check if rom loaded
  if (!mem.loadROM(argv[1])) {
    throw std::runtime_error("error loading rom");
  }

  //testing 
  for (int i = 0; i < 5; i++) cpu.step();
  
  // sdl3 init which i will use later when i write the graphics part of the emulator
  // if (!SDL_Init(SDL_INIT_VIDEO)) {
  //   SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
  //                            "Error initializing SDL3", nullptr);
  //   exit(EXIT_FAILURE);
  // }
  //
  // SDL_Window *window = SDL_CreateWindow("GBCpp", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  //
  // if (!window) {
  //   SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
  //                            "Error creating window", nullptr);
  //   exit(EXIT_FAILURE);
  // }
  //
  // bool isRunning {true};
  // while (isRunning) {
  //   SDL_Event event {0};
  //   while (SDL_PollEvent(&event)) 
  //     if (event.type == SDL_EVENT_QUIT)
  //       isRunning = false;
  // }
  //
  // SDL_DestroyWindow(window);
  //
  // SDL_Quit();
  return 0;
}
