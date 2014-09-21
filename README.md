### SuperFX disassembler

```
Usage: gsudis -[bes] infile
-h          Show this message
-b0x018000  Begin disassembling at address (hex) (optional, default: 0x8000)
-e0x01ffff  End disassembly at address (hex) (optional, default: end of bank)
-s          End disassembly at stop opcode (including pipeline)

Address format is mapped lorom, eg address 0x8000 is 0 in the binary.
```
