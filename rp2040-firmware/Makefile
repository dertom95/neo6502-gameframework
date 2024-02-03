# helper calls

build:
	cmake -B build-cmake -G "Unix Makefiles"
	cmake --build build-cmake -j 4

pico-install: build
	picotool load -f build-cmake/source/neo6502firmware.uf2

pico-reboot:
	picotool reboot

monitor-install-deps:
	sudo apt-get install minicom

assets-create:
	mkdir -p temp
	3rd/PicoDVI/software/scripts/packtiles --format r1 3rd/PicoDVI/software/assets/font_8x8.png temp/font8.bin 
	bin2c temp/font8.bin -H source/gen/font_8.h

	python3 tools/create_palette.py resources/palette_pico-8_duel-1x.png source/gen/color_palette.h color_palette

	python3 tools/ng_tool.py --convert-palette --input 
clean:
	rm -Rf build-cmake
	rm -Rf temp