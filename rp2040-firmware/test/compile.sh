cc65 -Osir 6502_tilepaint.c
ca65 -g -l basic.lst --feature labels_without_colons 6502_tilepaint.s
ld65 -C cc65-mem.cfg -o prg1.bin 6502_tilepaint.o none.lib
bin2c -C gen_prg1.c -H gen_prg1.h prg1.bin
