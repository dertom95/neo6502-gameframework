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


clean:
	rm -Rf build-cmake