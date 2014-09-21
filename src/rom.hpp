#ifndef ROM_HPP
#define ROM_HPP

class ROM
{
public:
  ROM (std::string &fname, size_t begin, size_t length);
  ~ROM ();
  
  virtual uint8 read(unsigned addr);
  virtual size_t size();
private:
  std::ifstream m_rom_file;

  std::vector<uint8> m_data;
  size_t m_begin, m_length;
};

#endif //ROM_HPP
