struct reg16_t {
  uint16 data;

  inline operator unsigned() const { return data; }
  inline uint16 assign(uint16 i) {
    data = i;
    return data;
  }

  inline unsigned operator++() { return assign(data + 1); }
  inline unsigned operator--() { return assign(data - 1); }
  inline unsigned operator++(int) { unsigned r = data; assign(data + 1); return r; }
  inline unsigned operator--(int) { unsigned r = data; assign(data - 1); return r; }
  inline unsigned operator   = (unsigned i) { return assign(i); }
  inline unsigned operator  |= (unsigned i) { return assign(data | i); }
  inline unsigned operator  ^= (unsigned i) { return assign(data ^ i); }
  inline unsigned operator  &= (unsigned i) { return assign(data & i); }
  inline unsigned operator <<= (unsigned i) { return assign(data << i); }
  inline unsigned operator >>= (unsigned i) { return assign(data >> i); }
  inline unsigned operator  += (unsigned i) { return assign(data + i); }
  inline unsigned operator  -= (unsigned i) { return assign(data - i); }
  inline unsigned operator  *= (unsigned i) { return assign(data * i); }
  inline unsigned operator  /= (unsigned i) { return assign(data / i); }
  inline unsigned operator  %= (unsigned i) { return assign(data % i); }

  inline unsigned operator   = (const reg16_t& i) { return assign(i); }

  reg16_t() : data(0) {}
  reg16_t(const reg16_t&) = delete;
};

struct sfr_t {
  bool irq;   //interrupt flag
  bool b;     //WITH flag
  bool ih;    //immediate higher 8-bit flag
  bool il;    //immediate lower 8-bit flag
  bool alt2;  //ALT2 mode
  bool alt1;  //ALT1 mode
  bool r;     //ROM r14 read flag
  bool g;     //GO flag
  bool ov;    //overflow flag
  bool s;     //sign flag
  bool cy;    //carry flag
  bool z;     //zero flag

  operator unsigned() const {
    return (irq << 15) | (b << 12) | (ih << 11) | (il << 10) | (alt2 << 9) | (alt1 << 8)
         | (r << 6) | (g << 5) | (ov << 4) | (s << 3) | (cy << 2) | (z << 1);
  }

  sfr_t& operator=(uint16_t data) {
    irq  = data & 0x8000;
    b    = data & 0x1000;
    ih   = data & 0x0800;
    il   = data & 0x0400;
    alt2 = data & SFR_ALT2; //0x0200;
    alt1 = data & SFR_ALT1; //0x0100;
    r    = data & 0x0040;
    g    = data & 0x0020;
    ov   = data & 0x0010;
    s    = data & 0x0008;
    cy   = data & 0x0004;
    z    = data & 0x0002;
    return *this;
  }
};


struct regs_t {
  uint8 pipeline;

  reg16_t r[16];    //general purpose registers
  sfr_t sfr;        //status flag register
  uint8 pbr;        //program bank register

  void reset() {
    sfr = 0x0000;
  }
} regs;

