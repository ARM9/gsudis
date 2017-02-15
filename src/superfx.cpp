
#define SUPERFX_CPP

#include "memory/memory.cpp"
#include "disasm/disasm.cpp"

SuperFX::SuperFX(std::string &fname, int begin, int end, int p_stop)
: rom(fname),
  dis_end(end),
  dis_stop(p_stop),
  step_length(1)
{
  if(end == -1) dis_end = 0x8000+rom.size()-2;
  memory_reset();
  regs.reset();
  regs.r[15] = begin - 1;
  regs.pbr = (uint8)(begin >> 16);
}

SuperFX::~SuperFX()
{
}

uint8 SuperFX::step()
{
  regs.r[15] += step_length;
  regs.pipeline = bus_read((regs.pbr << 16) + regs.r[15].data);
  regs.sfr = regs.sfr & ~(SFR_ALT3);

  return regs.pipeline;
}

