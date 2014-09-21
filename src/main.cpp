#include "precompiled.hpp"
#include "superfx.hpp"

std::string usage = R"(
SuperFX disassembler
Usage: gsudis -[bes] infile
-h          Show this message
-b018000    Begin disassembling at address (hex) (optional, default: 0x8000)
-e01ffff    End disassembly at address (hex) (optional, default: end of bank)
-s          End disassembly at stop opcode (including pipeline)

Address format is mapped lorom, eg address 0x8000 is 0 in the binary.
)";

int main (int argc, char *argv[])
{
  if (argc < 2){
    std::cout << usage;
    return 0;
  }
  
  std::vector<std::string> args;
  
  for (int i = 1; i < argc; i++){
    args.emplace_back(argv[i]);
    if (args.back() == "-h"){
      std::cout << usage;
      return 0;
    }
#ifdef _DEBUG
    std::cout << argv[i] << "\t" << args.back() << std::endl;
#endif
  }
  
  int t_stop_at_stop = 0;
  int t_begin = 0x8000, t_end = -1;
  for (auto &it : args){
    std::string t_str;

    if (it.find("-b") != std::string::npos){
      t_str = it.substr(2);
      t_begin = std::stoul(t_str.c_str(), nullptr, 16);
#ifdef _DEBUG
      std::cout << "found -b flag: 0x" << std::hex << t_begin << std::endl;
#endif
    }
    else if (it.find("-e") != std::string::npos){
      t_str = it.substr(2);
      t_end = std::stoul(t_str.c_str(), nullptr, 16);
#ifdef _DEBUG
      std::cout << "found -e flag: 0x" << std::hex << t_end << std::endl;
#endif
    }
    else if (it.find("-s") != std::string::npos){
      t_stop_at_stop = 1;
#ifdef _DEBUG
      std::cout << "found -s flag" << std::endl;
#endif
    }
  }

  if (t_end < 0){
    t_end = (t_begin & 0xff0000) + 0x010000 - 1;
  }
  std::string rom_name(args.back());
  SuperFX superfx(rom_name, t_begin, t_end, t_stop_at_stop);
  
  superfx.disassemble();

  return 0;
}

