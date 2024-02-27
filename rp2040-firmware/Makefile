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

test-ng-mem:
	mkdir -p temp && gcc -DTESTING -o temp/test-ng-mem source/ng_mem.c && temp/test-ng-mem


kinc_get:
	@if [ -d "build-scripts/kinc/tmp/Kinc" ]; then \
		echo "Found-Kinc"; \
    else \
		mkdir -p build-scripts/kinc/tmp && cd build-scripts/kinc/tmp && git clone https://github.com/Kode/Kinc.git; \
	fi

kinc-build: kinc_get
	cd build-scripts/kinc && tmp/Kinc/make  --to ../../build-kinc --target linux --graphics opengl --compile		

mod-assets-create:
	# todo: get rid of intermediate step to pack assets twice (once without code and once with code compiled)
	cd mod && make assets-pack

mod-assets-create-only-code:
	cd mod && make assets-pack-only-compile

mod-source-compile:
	cd mod && make source-compile

gluecode-create:
	python3 tools/ngtool/ng_tool.py create-gluecode source/ng_gfx.h source/ng_io.h --output tools/generator/input/glue.xml
	python3 tools/ngtool/gen.py --config-file tools/generator/generate_gluecode.json

gluecode-test-compile: 
	mkdir -p temp
	cc65 --cpu 65c02 -Osir -o temp/ng_api.s source/api/gen/ng_api.c
	ca65 --cpu 65c02 -g -l basic.lst --feature labels_without_colons temp/ng_api.s
	ld65 -C tools/cc65/cc65-mem-kernel.cfg -o temp/ng_api.bin temp/ng_api.o none.lib
	@echo "Compilation OK"
clean:
	rm -Rf build-cmake
	rm -Rf temp