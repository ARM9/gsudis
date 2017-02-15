#ifndef ROM_HPP
#define ROM_HPP

class ROM
{
public:
  ROM (std::string &fname);
  ~ROM ();
  
  virtual uint8 read(unsigned addr);
  virtual size_t size();
private:
  std::vector<uint8> m_data;
};

#endif //ROM_HPP
