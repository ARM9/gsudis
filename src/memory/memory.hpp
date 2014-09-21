unsigned rom_mask;  //rom_size - 1
uint8 bus_read(unsigned addr);
uint8 op_read(uint16 addr);

void memory_reset();
