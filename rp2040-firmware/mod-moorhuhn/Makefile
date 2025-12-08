MOD_DIR := $(CURDIR)
ASSET_FOLDER := $(MOD_DIR)/assets
EXPORT_FOLDER := $(MOD_DIR)/export
NG_TOOL := python3 $(MOD_DIR)/../tools/ngtool/ng_tool.py 
LLVM_MOS_SDK := /opt/llvm-mos
SDK_FOLDER := $(CURDIR)/..
PACK_FILES := temp/code.bin $(EXPORT_FOLDER)/*.bin $(ASSET_FOLDER)/audio/sfx/*.wav $(ASSET_FOLDER)/audio/mod/*.mod 
PACK_OUTPUT:= $(EXPORT_FOLDER)/assets.dat

initialize:
	mkdir -p $(MOD_DIR)/temp
	mkdir -p $(MOD_DIR)/export
	mkdir -p $(MOD_DIR)/source
	mkdir -p $(ASSET_FOLDER)/palette
	mkdir -p $(ASSET_FOLDER)/sprites
	mkdir -p $(ASSET_FOLDER)/tiles
	mkdir -p $(ASSET_FOLDER)/bitmaps
	mkdir -p $(ASSET_FOLDER)/fonts
	mkdir -p $(ASSET_FOLDER)/audio/mod
	mkdir -p $(ASSET_FOLDER)/audio/sfx 

source-compile:
	cd source && $(LLVM_MOS_SDK)/bin/mos-common-clang -D__NG6502__  -o ../temp/code.bin -Os -g -lexit-loop -linit-stack -I.. -I../../source/api \
		mod_init.c \
		moorhuhn.c \
		mh_globals.c \
		$(SDK_FOLDER)/source/api/gen/ng_api.c  \
		$(SDK_FOLDER)/source/api/ng_api_shared.c \
		&& $(LLVM_MOS_SDK)/bin/llvm-objdump -x ../temp/code.bin.elf > ../temp/code.bin.elf.map \
		&& $(LLVM_MOS_SDK)/bin/llvm-objdump -S -l ../temp/code.bin.elf > ../temp/code.S 
	
assets-create: initialize
	#TODO: make this into the python-tool
	$(SDK_FOLDER)/3rd/PicoDVI/software/scripts/packtiles --format r1 $(ASSET_FOLDER)/fonts/font_8x8.png $(EXPORT_FOLDER)/font8.bin 

	$(NG_TOOL) convert-palette --input $(ASSET_FOLDER)/palette/palette.png --output $(EXPORT_FOLDER)/color_palette.bin --binary 

	$(NG_TOOL) convert-tilesheet 	--tilesheet-file $(ASSET_FOLDER)/sprites/bird_16px_t.png \
									--tile-size 16x16 \
									--palette-file $(ASSET_FOLDER)/palette/palette.png \
									--array-name sprite_bird_16 \
									--output $(EXPORT_FOLDER)/sprite_bird_16.bin \
									--binary \
									--format 2 \
									--debug 

	$(NG_TOOL) convert-tilesheet 	--tilesheet-file $(ASSET_FOLDER)/sprites/bird_32px_t.png \
									--tile-size 32x32 \
									--palette-file $(ASSET_FOLDER)/palette/palette.png \
									--array-name sprite_bird_32 \
									--output $(EXPORT_FOLDER)/sprite_bird_32.bin \
									--binary \
									--format 2 \
									--debug 

	$(NG_TOOL) convert-tilesheet 	--tilesheet-file $(ASSET_FOLDER)/sprites/crosshair_16px_t.png \
									--tile-size 16x16 \
									--palette-file $(ASSET_FOLDER)/palette/palette.png \
									--array-name crosshair_16 \
									--output $(EXPORT_FOLDER)/sprite_crosshair_16.bin \
									--binary \
									--format 2 \
									--debug 	

	$(NG_TOOL) convert-tilesheet 	--tilesheet-file $(ASSET_FOLDER)/sprites/ui_misc_16px_t.png \
									--tile-size 16x16 \
									--palette-file $(ASSET_FOLDER)/palette/palette.png \
									--array-name ui_misc_16 \
									--output $(EXPORT_FOLDER)/ui_misc_16.bin \
									--binary \
									--format 2 \
									--debug 																								

	# $(NG_TOOL) convert-tilesheet 	--tilesheet-file $(ASSET_FOLDER)/sprites/pillar.png \
	# 								--tile-size 43x80 \
	# 								--palette-file $(ASSET_FOLDER)/palette/palette.png \
	# 								--array-name sprite_pillar \
	# 								--output $(EXPORT_FOLDER)/sprite_pillar.bin \
	# 								--binary \
	# 								--format 2 \
	# 								--debug \

	$(NG_TOOL) convert-tilesheet 	--tilesheet-file $(ASSET_FOLDER)/bitmaps/background1.png \
	 								--tile-size 160x120 \
	 								--palette-file $(ASSET_FOLDER)/palette/palette.png \
	 								--array-name bitmap_bg1 \
	 								--output $(EXPORT_FOLDER)/bitmap_bg1.bin \
	 								--binary \
	 								--format 2 \
	 								--debug \


check_code_bin:
	@if [ ! -f temp/code.bin ]; then \
		echo "Creating dummy code.bin..."; \
		touch temp/code.bin; \
		else \
		echo "code.bin already exists."; \
	fi

PACK_FILES = temp/code.bin $(EXPORT_FOLDER)/*.bin $(ASSET_FOLDER)/audio/sfx/*.wav $(ASSET_FOLDER)/audio/mod/*.mod --output $(EXPORT_FOLDER)/assets.dat

# Targets
assets-pack-no-code: check_code_bin assets-create
	$(NG_TOOL) pack-files $(PACK_FILES) --output $(PACK_OUTPUT)

assets-pack: assets-create source-compile
	$(NG_TOOL) pack-files $(PACK_FILES) --output $(PACK_OUTPUT)

assets-pack-only-compile: source-compile
	$(NG_TOOL) pack-files $(PACK_FILES) --output $(PACK_OUTPUT)
	
									
assets-clear: 
	-rm $(EXPORT_FOLDER)/*
	-rm $(MOD_DIR)/temp
#bin2c export/old_guy.bin -H export/old_guy.h
#bin2c export/sprites_misc.bin -H export/sprites_misc.h

code-switch-modnative: 
	cd .. && make kinc-modnative-build

code-switch-mod6502: 
	cd .. && make kinc-6502-build

run-modnative: 
	cd .. && make kinc-modnative-run

run-mod6502: 
	cd .. && make kinc-6502-run	

clean:
	rm -Rf $(ASSET_FOLDER)
	rm -Rf $(EXPORT_FOLDER)
	make initialize