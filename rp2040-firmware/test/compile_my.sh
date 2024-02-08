ca65 -g -l basic.lst --feature labels_without_colons my.s
ld65 -C cc65-mem.cfg -o prg1.bin my.o none.lib
bin2c -C gen_prg1.c -H gen_prg1.h prg1.bin
