MOD_DIR := $(CURDIR)
ASSET_FOLDER := $(MOD_DIR)/assets
EXPORT_FOLDER := $(MOD_DIR)/export
NG_TOOL := python3 $(MOD_DIR)/../tools/ngtool/ng_tool.py 
LLVM_MOS_SDK := /opt/llvm-mos

initialize:
	mkdir -p $(MOD_DIR)/temp
	mkdir -p $(MOD_DIR)/export
	mkdir -p $(MOD_DIR)/source
	mkdir -p $(ASSET_FOLDER)/fonts
	mkdir -p $(ASSET_FOLDER)/sprites
	mkdir -p $(ASSET_FOLDER)/tiles
	mkdir -p $(ASSET_FOLDER)/palette

source-compile:
	cd source && $(LLVM_MOS_SDK)/bin/mos-common-clang -D__NG6502__  -o ../temp/code.bin -Os -g -lexit-loop -linit-stack -I.. -I../../source/api \
		mod_pixelbuffer.c \
		../../source/api/ng_api_shared.c && $(LLVM_MOS_SDK)/bin/llvm-objdump -x ../temp/code.bin.elf > ../temp/code.bin.elf.map && $(LLVM_MOS_SDK)/bin/llvm-objdump -S -l ../temp/code.bin.elf > ../temp/code.S 
	
# source-compile-cc65:	
# 	cc65 -Osir source/mod.c --add-source -I../source/api -I. -o temp/main.s --standard c99
# 	ca65 -g -l basic.lst --feature labels_without_colons temp/main.s
# 	ld65 -C $(MOD_DIR)/../tools/cc65/cc65-mem-usercode.cfg -o temp/code.bin temp/main.o none.lib

assets-create: initialize
#	../3rd/PicoDVI/software/scripts/packtiles --format r1 $(ASSET_FOLDER)/fonts/font_8x8.png temp/font8.bin 
#	bin2c temp/font8.bin -H $(EXPORT_FOLDER)/font.h
	../3rd/PicoDVI/software/scripts/packtiles --format r1 $(ASSET_FOLDER)/fonts/font_8x8.png $(EXPORT_FOLDER)/font8.bin 

	$(NG_TOOL) convert-palette --input $(ASSET_FOLDER)/palette/palette.png --output $(EXPORT_FOLDER)/color_palette.bin --binary 
	$(NG_TOOL) convert-palette --input $(ASSET_FOLDER)/palette/palette.png --output $(EXPORT_FOLDER)/color_palette.h --array-name palette

	$(NG_TOOL) convert-palette --input $(ASSET_FOLDER)/palette/palette_small.png --output $(EXPORT_FOLDER)/color_palette_small.bin --binary

	$(NG_TOOL) convert-tilesheet 	--tilesheet-file $(ASSET_FOLDER)/sprites/sprites_misc.png \
									--tile-size 16x16 \
									--palette-file $(ASSET_FOLDER)/palette/palette.png \
									--array-name sprites_misc \
									--output $(EXPORT_FOLDER)/sprites_misc.bin \
									--binary \
									--format 2 \
									--debug \

	$(NG_TOOL) convert-tilesheet 	--tilesheet-file $(ASSET_FOLDER)/sprites/old_guy.png \
									--tile-size 16x16 \
									--palette-file $(ASSET_FOLDER)/palette/palette.png \
									--array-name gold_guy \
									--output $(EXPORT_FOLDER)/old_guy.bin \
									--binary \
									--format 2 \
									--debug \

	$(NG_TOOL) convert-tilesheet 	--tilesheet-file $(ASSET_FOLDER)/tilesheets/spritesheet.png \
									--tile-size 16x16 \
									--palette-file $(ASSET_FOLDER)/palette/palette.png \
									--array-name spritesheet \
									--output $(EXPORT_FOLDER)/spritesheet.bin \
									--binary \
									--format 1 \
									--debug \

	$(NG_TOOL) convert-tilemap      --input $(ASSET_FOLDER)/tilemaps/map.json \
									--format spritefusion \
									--output $(EXPORT_FOLDER)/tilemap.bin \


check_code_bin:
	@if [ ! -f temp/code.bin ]; then \
		echo "Creating code.bin..."; \
		touch temp/code.bin; \
		else \
		echo "code.bin already exists."; \
	fi

assets-pack-no-code: check_code_bin
	$(NG_TOOL) pack-files temp/code.bin $(EXPORT_FOLDER)/*.bin --output $(EXPORT_FOLDER)/assets.dat

assets-pack: assets-create  source-compile
	$(NG_TOOL) pack-files temp/code.bin $(EXPORT_FOLDER)/*.bin --output $(EXPORT_FOLDER)/assets.dat

assets-pack-only-compile: source-compile
	$(NG_TOOL) pack-files temp/code.bin $(EXPORT_FOLDER)/*.bin --output $(EXPORT_FOLDER)/assets.dat

									
assets-clear: 
	-rm $(EXPORT_FOLDER)/*
	-rm $(MOD_DIR)/temp
#bin2c export/old_guy.bin -H export/old_guy.h
#bin2c export/sprites_misc.bin -H export/sprites_misc.h


clean:
	rm -Rf $(ASSET_FOLDER)
	rm -Rf $(EXPORT_FOLDER)
	make initialize