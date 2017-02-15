#include "precompiled.hpp"
#include "superfx.hpp"

std::string usage = R"(
SuperFX disassembler
Usage: gsudis [OPTION]... [FILE]
When OPTION and FILE are empty, reads from stdin and -s is implicit
-h          Show this message
-b018000    Begin disassembling at address (hex) (optional, default: 0x8000)
-e01ffff    End disassembly at address (hex) (optional, default: end of bank)
-s          End disassembly at stop opcode (including pipeline)

Lorom mapping
)";

int main (int argc, char *argv[])
{
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
  
  unsigned last_flag = 0;
  int t_stop_at_stop = 0;
  int begin = 0x8000, end = -1;
  for (auto &it : args){
    std::string t_str;

    if (it.find("-b") != std::string::npos){
      last_flag++;
      t_str = it.substr(2);
      begin = std::stoul(t_str.c_str(), nullptr, 16);
    }
    else if (it.find("-e") != std::string::npos){
      last_flag++;
      t_str = it.substr(2);
      end = std::stoul(t_str.c_str(), nullptr, 16);
    }
    else if (it.find("-s") != std::string::npos){
      last_flag++;
      t_stop_at_stop = 1;
    }
  }

  if (argc > 1) {
    if (args.back() != "-" && end < 0) {
      end = (begin & 0xff0000) + 0x010000 - 1;
    }
  }
  std::string rom_name = (last_flag == args.size())
          ? "-"
          : args.back();
  SuperFX superfx(rom_name, begin, end, t_stop_at_stop);
  
  superfx.disassemble();

  return 0;
}

