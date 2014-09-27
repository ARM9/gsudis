void disassemble();
void disassemble_opcode(char *output);
void dis_hex_listing(char *output);

bool is_alt1(uint8 opcode);
bool is_alt2(uint8 opcode);
bool is_alt3(uint8 opcode);

bool is_from(uint8 opcode);
bool is_to(uint8 opcode);

#define table16(id) \
  id, id+1, id+2, id+3, id+4, id+5, id+6, id+7, id+8, id+9, id+10, id+11, id+12, id+13, id+14, id+15
#define table15(id) \
  id, id+1, id+2, id+3, id+4, id+5, id+6, id+7, id+8, id+9, id+10, id+11, id+12, id+13, id+14
#define table12(id) \
  id, id+1, id+2, id+3, id+4, id+5, id+6, id+7, id+8, id+9, id+10, id+11
#define table6(id) \
  id, id+1, id+2, id+3, id+4, id+5

const std::vector<uint8> alt1_lut = {
  table12(0x30),  //stb
  table12(0x40),  //ldb
          0x4c,   //rpix
          0x4e,   //cmode
  table16(0x50),  //adc r
  table16(0x60),  //sbc r
  table15(0x71),  //bic r
  table16(0x80),  //umult r
          0x96,   //div2
  table6 (0x98),  //ljmp r
          0x9f,   //lmult
  table16(0xa0),  //lms r, ($yy<<1)
  table15(0xc1),  //xor r
          0xef,   //getbh
  table16(0xf0)   //lm r, ($xxxx)
};

const std::vector<uint8> alt2_lut = {
  table16(0x50),  //add #
  table16(0x60),  //sub #
  table15(0x71),  //and #
  table16(0x80),  //mult #
  table16(0xa0),  //sms r, ($yy<<1)
  table15(0xc1),  //or #
          0xdf,   //ramb
          0xef,   //getbl
  table16(0xf0)   //sm r, ($xxxx)
};

const std::vector<uint8> alt3_lut = {
  table16(0x50),  //adc #
  table16(0x60),  //cmp r
  table15(0x71),  //bic #
  table16(0x80),  //umult #
  table15(0xc1),  //xor #
          0xdf,   //romb
          0xef    //getbs
};

#undef table16
#undef table15
#undef table12
#undef table6

