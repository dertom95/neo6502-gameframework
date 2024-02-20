cc65 -Osir 6502_func_call_api.c --add-source
ca65 -g -l basic.lst --feature labels_without_colons 6502_func_call_api.s
ld65 -C cc65-mem.cfg -o prg1.bin 6502_func_call_api.o none.lib
bin2c -C gen_prg1.c -H gen_prg1.h prg1.bin
