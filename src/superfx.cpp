
#define SUPERFX_CPP

#include "memory/memory.cpp"
#include "disasm/disasm.cpp"

SuperFX::SuperFX(std::string &fname, int begin, int end, int p_stop)
: rom(fname, begin, end - begin),
  dis_end(end),
  dis_stop(p_stop),
  step_length(1)
{
  memory_reset();
  regs.reset();
  regs.r[15] = begin - 1;
  regs.pbr = (uint8)(begin >> 16);
}

SuperFX::~SuperFX()
{
}

#define case16(id) \
  case id+ 0: case id+ 1: case id+ 2: case id+ 3: case id+ 4: case id+ 5: case id+ 6: case id+ 7: \
  case id+ 8: case id+ 9: case id+10: case id+11: case id+12: case id+13: case id+14: case id+15
uint8 SuperFX::step()
{
  regs.r[15] += step_length;
  regs.pipeline = bus_read((regs.pbr << 16) + regs.r[15].data);

  regs.sfr = regs.sfr & ~(SFR_ALT3);
  switch (regs.pipeline){
    case (0x3d): regs.sfr = regs.sfr | SFR_ALT1; break;
    case (0x3e): regs.sfr = regs.sfr | SFR_ALT2; break;
    case (0x3f): regs.sfr = regs.sfr | SFR_ALT3; break;
    default:
      break;
  }

  return regs.pipeline;
}
#undef case16

