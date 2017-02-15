#include "precompiled.hpp"
#include "rom.hpp"

ROM::ROM(std::string &fname)
{
    std::ifstream f(fname, std::ios::binary);
    std::istream& rom_file = (fname == "-") ? std::cin : f;
    std::istreambuf_iterator<char> begin(rom_file), end;
    std::vector<uint8> data(begin, end);
    m_data = data;
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

