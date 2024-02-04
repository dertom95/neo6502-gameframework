MOD_DIR := $(CURDIR)
ASSET_FOLDER := $(MOD_DIR)/assets
EXPORT_FOLDER := $(MOD_DIR)/export
NG_TOOL := python3 $(MOD_DIR)/../tools/ngtool/ng_tool.py 

initialize:
	mkdir -p $(MOD_DIR)/temp
	mkdir -p $(MOD_DIR)/export
	mkdir -p $(ASSET_FOLDER)/fonts
	mkdir -p $(ASSET_FOLDER)/sprites
	mkdir -p $(ASSET_FOLDER)/tiles
	mkdir -p $(ASSET_FOLDER)/palette

assets-create: initialize
	../3rd/PicoDVI/software/scripts/packtiles --format r1 $(ASSET_FOLDER)/fonts/font_8x8.png temp/font8.bin 
	bin2c temp/font8.bin -H $(EXPORT_FOLDER)/font.h

	$(NG_TOOL) convert-palette --input $(ASSET_FOLDER)/palette/palette.png --output $(EXPORT_FOLDER)/color_palette.h --array-name color_palette

	$(NG_TOOL) convert-palette --input $(ASSET_FOLDER)/palette/palette_small.png --output $(EXPORT_FOLDER)/color_palette_small.h --array-name color_palette_small

	$(NG_TOOL) convert-tilesheet 	--tilesheet-file $(ASSET_FOLDER)/sprites/sprites_misc.png \
									--tile-size 16x16 \
									--palette-file $(ASSET_FOLDER)/palette/palette.png \
									--array-name sprites_misc \
									--output $(EXPORT_FOLDER)/sprites_misc.h 

	$(NG_TOOL) convert-tilesheet 	--tilesheet-file $(ASSET_FOLDER)/sprites/old_guy.png \
									--tile-size 16x16 \
									--palette-file $(ASSET_FOLDER)/palette/palette.png \
									--array-name gold_guy \
									--output $(EXPORT_FOLDER)/old_guy.h 


clean:
	rm -Rf $(ASSET_FOLDER)
	rm -Rf $(EXPORT_FOLDER)
	make initialize