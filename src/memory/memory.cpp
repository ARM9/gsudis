#include "../precompiled.hpp"
#include "../superfx.hpp"
#include "memory.hpp"

uint8 SuperFX::bus_read(unsigned addr) {
  return rom.read((((addr & 0x3f0000) >> 1) | (addr & 0x7fff)) % rom_mask);
}

uint8 SuperFX::op_read(uint16 addr) {
  return bus_read((regs.pbr << 16) + addr);
}

void SuperFX::memory_reset() {
  rom_mask = rom.size();
}

