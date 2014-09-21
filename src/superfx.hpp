#ifndef SUPERFX_HPP
#define SUPERFX_HPP

#include "rom.hpp"

#define SFR_ALT0 0x0000
#define SFR_ALT1 0x0100
#define SFR_ALT2 0x0200
#define SFR_ALT3 0x0300

struct SuperFX
{
  SuperFX (std::string &fname, int begin, int end, int p_stop);
  ~SuperFX ();
  ROM rom;

  #include "memory/memory.hpp"
  #include "disasm/disasm.hpp"

  #include "registers.hpp"

  uint8 step();

  unsigned dis_end;
  unsigned dis_stop; //stop after STOP opcode
  unsigned instruction_counter;
  int step_length;
};

#endif //SUPERFX_HPP
