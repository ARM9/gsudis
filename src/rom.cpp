#include "precompiled.hpp"
#include "rom.hpp"

ROM::ROM(std::string &fname, size_t begin, size_t length)
: m_rom_file(fname, std::ios::binary),
  m_data((std::istreambuf_iterator<char>(m_rom_file)),
         (std::istreambuf_iterator<char>())),
  m_begin(begin),
  m_length(length)
{
}

ROM::~ROM()
{
}

uint8 ROM::read(unsigned addr)
{
  return m_data[addr];
}

size_t ROM::size()
{
  return m_data.size();
}

