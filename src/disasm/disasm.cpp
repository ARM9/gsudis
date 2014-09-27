#include "../precompiled.hpp"
#include "../superfx.hpp"

void SuperFX::disassemble()
{
  char* out_buffer = new char[128];
  for (;((regs.pbr << 16) + regs.r[15]) <= dis_end;){
    step();
    if (regs.r[15] < 0x8000)
      break;
    if (dis_stop > 1){
      if (dis_stop++ > 2)
        break;
    }

    *out_buffer = 0;
    sprintf(out_buffer, "code_%06x:   ", (regs.pbr << 16) + regs.r[15].data);
    disassemble_opcode(out_buffer);
    dis_hex_listing(out_buffer);
    std::cout << out_buffer << std::endl;
  }

  delete[] out_buffer;
}

static void align_column(char* output, unsigned col)
{
  unsigned length = strlen(output);
  while (length++ < col) strcat(output, " ");
  strcat(output, ";");
}

void SuperFX::dis_hex_listing(char* output)
{
  align_column(output, 32);
  char h[8];

  int i = (regs.sfr & SFR_ALT3 ? -1 : 0);
  for (; i < step_length; i++){
    sprintf(h, " %.2x", (uint8)bus_read((regs.pbr << 16) + regs.r[15] + i));
    strcat(output, h);
  }
}

bool SuperFX::is_alt1(uint8 opcode)
{
  return std::binary_search(alt1_lut.begin(), alt1_lut.end(), opcode);
}

bool SuperFX::is_alt2(uint8 opcode)
{
  return std::binary_search(alt2_lut.begin(), alt2_lut.end(), opcode);
}

bool SuperFX::is_alt3(uint8 opcode)
{
  return std::binary_search(alt3_lut.begin(), alt3_lut.end(), opcode);
}

bool SuperFX::is_from(uint8 op)
{
  if ((op & 0xf0) ^ 0xb0)
    return false;
  else
    return true;
}

bool SuperFX::is_to(uint8 op)
{
  if ((op & 0xf0) ^ 0x10)
    return false;
  else
    return true;
}

void SuperFX::disassemble_opcode(char* output)
{

#define case4(id) \
  case id+ 0: case id+ 1: case id+ 2: case id+ 3
#define case6(id) \
  case id+ 0: case id+ 1: case id+ 2: case id+ 3: case id+ 4: case id+ 5
#define case12(id) \
  case id+ 0: case id+ 1: case id+ 2: case id+ 3: case id+ 4: case id+ 5: case id+ 6: case id+ 7: \
  case id+ 8: case id+ 9: case id+10: case id+11
#define case15(id) \
  case id+ 0: case id+ 1: case id+ 2: case id+ 3: case id+ 4: case id+ 5: case id+ 6: case id+ 7: \
  case id+ 8: case id+ 9: case id+10: case id+11: case id+12: case id+13: case id+14
#define case16(id) \
  case id+ 0: case id+ 1: case id+ 2: case id+ 3: case id+ 4: case id+ 5: case id+ 6: case id+ 7: \
  case id+ 8: case id+ 9: case id+10: case id+11: case id+12: case id+13: case id+14: case id+15

#define op0 regs.pipeline
#define op1 bus_read((regs.pbr << 16) + regs.r[15] + 1)
#define op2 bus_read((regs.pbr << 16) + regs.r[15] + 2)

  char t[256] = "";
  
  step_length = 1;
  switch (op0) {
    case  (0x00): sprintf(t, "stop"); if (dis_stop){dis_stop++;} break;
    case  (0x01): sprintf(t, "nop"); break;
    case  (0x02): sprintf(t, "cache"); break;
    case  (0x03): sprintf(t, "lsr"); break;
    case  (0x04): sprintf(t, "rol"); break;
    case  (0x05): sprintf(t, "bra code_%06x",
                      ((regs.pbr << 16) + regs.r[15] + 2 + (int8)op1));
      step_length = 2;
      break;
    case  (0x06): sprintf(t, "bge code_%06x",
                      ((regs.pbr << 16) + regs.r[15] + 2 + (int8)op1));
      step_length = 2;
      break;
    case  (0x07): sprintf(t, "blt code_%06x",
                      ((regs.pbr << 16) + regs.r[15] + 2 + (int8)op1));
      step_length = 2;
      break;
    case  (0x08): sprintf(t, "bne code_%06x",
                      ((regs.pbr << 16) + regs.r[15] + 2 + (int8)op1));
      step_length = 2;
      break;
    case  (0x09): sprintf(t, "beq code_%06x",
                      ((regs.pbr << 16) + regs.r[15] + 2 + (int8)op1));
      step_length = 2;
      break;
    case  (0x0a): sprintf(t, "bpl code_%06x",
                      ((regs.pbr << 16) + regs.r[15] + 2 + (int8)op1));
      step_length = 2;
      break;
    case  (0x0b): sprintf(t, "bmi code_%06x",
                      ((regs.pbr << 16) + regs.r[15] + 2 + (int8)op1));
      step_length = 2;
      break;
    case  (0x0c): sprintf(t, "bcc code_%06x",
                      ((regs.pbr << 16) + regs.r[15] + 2 + (int8)op1));
      step_length = 2;
      break;
    case  (0x0d): sprintf(t, "bcs code_%06x",
                      ((regs.pbr << 16) + regs.r[15] + 2 + (int8)op1));
      step_length = 2;
      break;
    case  (0x0e): sprintf(t, "bvc code_%06x",
                      ((regs.pbr << 16) + regs.r[15] + 2 + (int8)op1));
      step_length = 2;
      break;
    case  (0x0f): sprintf(t, "bvs code_%06x",
                      ((regs.pbr << 16) + regs.r[15] + 2 + (int8)op1));
      step_length = 2;
      break;
    case16(0x10): sprintf(t, "to r%u", op0 & 15); break;
    case16(0x20): 
      if (is_from(op1)){
        sprintf(t, "moves r%u, r%u", op0 & 15, op1 & 15);
        step_length = 2;
      }else if (is_to(op1)){
        sprintf(t, "move r%u, r%u", op1 & 15, op0 & 15);
        step_length = 2;
      }else{
        sprintf(t, "with r%u", op0 & 15);
      }
      break;
    case12(0x30):
      switch (regs.sfr & SFR_ALT3){
        case SFR_ALT0:
        case SFR_ALT2:
          sprintf(t, "stw (r%u)", op0 & 15);
          break;
        case SFR_ALT1:
        case SFR_ALT3:
          sprintf(t, "stb (r%u)", op0 & 15);
          break;
      }
      break;
    case  (0x3c): sprintf(t, "loop"); break;
    case  (0x3d):
      if (is_alt1(op1)){
        step();
        regs.sfr = regs.sfr | SFR_ALT1;
        disassemble_opcode(output);
      }else{
        sprintf(t, "alt1");
      }
      break;
    case  (0x3e):
      if (is_alt2(op1)){
        step();
        regs.sfr = regs.sfr | SFR_ALT2;
        disassemble_opcode(output);
      }else{
        sprintf(t, "alt2");
      }
      break;
    case  (0x3f):
      if (is_alt3(op1)){
        step();
        regs.sfr = regs.sfr | SFR_ALT3;
        disassemble_opcode(output);
      }else{
        sprintf(t, "alt3");
      }
      break;
    case12(0x40):
      switch (regs.sfr & SFR_ALT3){
        case SFR_ALT0:
        case SFR_ALT2:
          sprintf(t, "ldw (r%u)", op0 & 15);
          break;
        case SFR_ALT1:
        case SFR_ALT3:
          //step_length = 2;
          sprintf(t, "ldb (r%u)", op0 & 15);
          break;
      }
      break;
    case  (0x4c):
      switch (regs.sfr & SFR_ALT3){
        case SFR_ALT0:
        case SFR_ALT2:
          sprintf(t, "plot");
          break;
        case SFR_ALT1:
        case SFR_ALT3:
          //step_length = 2;
          sprintf(t, "rpix");
          break;
      }
      break;
    case  (0x4d): sprintf(t, "swap"); break;
    case  (0x4e):
      switch (regs.sfr & SFR_ALT3){
        case SFR_ALT0:
        case SFR_ALT2:
          sprintf(t, "color");
          break;
        case SFR_ALT1:
        case SFR_ALT3:
          //step_length = 2;
          sprintf(t, "cmode");
          break;
      }
      break;
    case  (0x4f): sprintf(t, "not"); break;
    case16(0x50):
      switch (regs.sfr & SFR_ALT3){
        case SFR_ALT0:
          sprintf(t, "add r%u", op0 & 15);
          break;
        case SFR_ALT1:
          //step_length = 2;
          sprintf(t, "adc r%u", op0 & 15);
          break;
        case SFR_ALT2:
          //step_length = 2;
          sprintf(t, "add #$%x", op0 & 15);
          break;
        case SFR_ALT3:
          //step_length = 2;
          sprintf(t, "adc #$%x", op0 & 15);
          break;
      }
      break;
    case16(0x60):
      switch (regs.sfr & SFR_ALT3){
        case SFR_ALT0:
          sprintf(t, "sub r%u", op0 & 15);
          break;
        case SFR_ALT1:
          //step_length = 2;
          sprintf(t, "sbc r%u", op0 & 15);
          break;
        case SFR_ALT2:
          //step_length = 2;
          sprintf(t, "sub #$%x", op0 & 15);
          break;
        case SFR_ALT3:
          //step_length = 2;
          sprintf(t, "cmp r%u", op0 & 15);
          break;
      }
      break;
    case  (0x70): sprintf(t, "merge"); break;
    case15(0x71):
      switch (regs.sfr & SFR_ALT3){
        case SFR_ALT0:
          sprintf(t, "and r%u", op0 & 15);
          break;
        case SFR_ALT1:
          //step_length = 2;
          sprintf(t, "bic r%u", op0 & 15);
          break;
        case SFR_ALT2:
          //step_length = 2;
          sprintf(t, "and #$%x", op0 & 15);
          break;
        case SFR_ALT3:
          //step_length = 2;
          sprintf(t, "bic #$%x", op0 & 15);
          break;
      }
      break;
    case16(0x80):
      switch (regs.sfr & SFR_ALT3){
        case SFR_ALT0:
          sprintf(t, "mult r%u", op0 & 15);
          break;
        case SFR_ALT1:
          //step_length = 2;
          sprintf(t, "umult r%u", op0 & 15);
          break;
        case SFR_ALT2:
          //step_length = 2;
          sprintf(t, "mult #$%x", op0 & 15);
          break;
        case SFR_ALT3:
          //step_length = 2;
          sprintf(t, "umult #$%x", op0 & 15);
          break;
      }
      break;
    case  (0x90): sprintf(t, "sbk"); break;
    case4 (0x91): sprintf(t, "link #%u", op0 & 15); break;
    case  (0x95): sprintf(t, "sex"); break;
    case  (0x96):
      switch (regs.sfr & SFR_ALT3){
        case SFR_ALT0:
        case SFR_ALT2:
          sprintf(t, "asr");
          break;
        case SFR_ALT1:
        case SFR_ALT3:
          //step_length = 2;
          sprintf(t, "div2");
          break;
      }
      break;
    case  (0x97): sprintf(t, "ror"); break;
    case6 (0x98):
      switch (regs.sfr & SFR_ALT3){
        case SFR_ALT0:
        case SFR_ALT2:
          sprintf(t, "jmp r%u", op0 & 15);
          break;
        case SFR_ALT1:
        case SFR_ALT3:
          //step_length = 2;
          sprintf(t, "ljmp r%u", op0 & 15);
          break;
      }
      break;
    case  (0x9e): sprintf(t, "lob"); break;
    case  (0x9f):
      switch (regs.sfr & SFR_ALT3){
        case SFR_ALT0:
        case SFR_ALT2:
          sprintf(t, "fmult");
          break;
        case SFR_ALT1:
        case SFR_ALT3:
          //step_length = 2;
          sprintf(t, "lmult");
          break;
      }
      break;
    case16(0xa0):
      switch (regs.sfr & SFR_ALT3){
        case SFR_ALT0:
          step_length = 2;
          sprintf(t, "ibt r%u, #$%.2x", op0 & 15, op1);
          break;
        case SFR_ALT2:
          step_length = 2;
          sprintf(t, "sms ($%.4x), r%u", op1 << 1, op0 & 15);
          break;
        case SFR_ALT1:
        case SFR_ALT3:
          step_length = 2;
          sprintf(t, "lms r%u, ($%.4x)", op0 & 15, op1 << 1);
          break;
      }
      break;
    case16(0xb0): sprintf(t, "from r%u", op0 & 15); break;
    case  (0xc0): sprintf(t, "hib"); break;
    case15(0xc1):
      switch (regs.sfr & SFR_ALT3){
        case SFR_ALT0:
          sprintf(t, "or r%u", op0 & 15);
          break;
        case SFR_ALT1:
          //step_length = 2;
          sprintf(t, "xor r%u", op0 & 15);
          break;
        case SFR_ALT2:
          //step_length = 2;
          sprintf(t, "or #$%x", op0 & 15);
          break;
        case SFR_ALT3:
          //step_length = 2;
          sprintf(t, "xor #$%x", op0 & 15);
          break;
      }
      break;
    case15(0xd0): sprintf(t, "inc r%u", op0 & 15); break;
    case  (0xdf):
      switch (regs.sfr & SFR_ALT3){
        case SFR_ALT0:
        case SFR_ALT1:
          sprintf(t, "getc");
          break;
        case SFR_ALT2:
          //step_length = 2;
          sprintf(t, "ramb");
          break;
        case SFR_ALT3:
          //step_length = 2;
          sprintf(t, "romb");
          break;
      }
      break;
    case15(0xe0): sprintf(t, "dec r%u", op0 & 15); break;
    case  (0xef):
      switch (regs.sfr & SFR_ALT3){
        case SFR_ALT0:
          sprintf(t, "getb");
          break;
        case SFR_ALT1:
          //step_length = 2;
          sprintf(t, "getbh");
          break;
        case SFR_ALT2:
          //step_length = 2;
          sprintf(t, "getbl");
          break;
        case SFR_ALT3:
          //step_length = 2;
          sprintf(t, "getbs");
          break;
      }
      break;
    case16(0xf0):
      switch (regs.sfr & SFR_ALT3){
        case SFR_ALT0:
          step_length = 3;
          sprintf(t, "iwt r%u, #$%.2x%.2x", op0 & 15, op2, op1);
          break;
        case SFR_ALT2:
          step_length = 3;
          sprintf(t, "sm ($%.2x%.2x), r%u", op2, op1, op0 & 15);
          break;
        case SFR_ALT1:
        case SFR_ALT3:
          step_length = 3;
          sprintf(t, "lm r%u, ($%.2x%.2x)", op0 & 15, op2, op1);
          break;
      }
      break;
  }

  strcat(output, t);
#undef case4
#undef case6
#undef case12
#undef case15
#undef case16
#undef op0
#undef op1
#undef op2
}

