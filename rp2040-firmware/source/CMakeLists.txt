add_executable(neo6502firmware
	${CMAKE_SOURCE_DIR}/3rd/pico-mod-player/lib/audio.c
	${CMAKE_SOURCE_DIR}/3rd/pico-mod-player/lib/mod_play.c



	core/memory.c
	core/wdc65C02cpu.c
	core/roms.c
	../test/gen_prg1.c

	ng_gfx.c
	ng_sound.c
	ng_utils.c
	ng_io.c
	ng_mem.c

	game/game.c

	main.c	
)

add_definitions(-DPICO_NEO6502=1 -DCHIPS_IMPL)

target_compile_options(neo6502firmware PRIVATE -Wall -g )
#target_compile_options(neo6502firmware PRIVATE -Wall -O3)

target_compile_definitions(neo6502firmware PRIVATE
#	DVI_DEFAULT_SERIAL_CONFIG=${DVI_DEFAULT_SERIAL_CONFIG}
#	DVI_N_TMDS_BUFFERS=5
	)

target_link_libraries(neo6502firmware
	pico_stdlib
	pico_multicore
	pico_util
	libdvi
	libsprite
	tinyusb_host tinyusb_board
#	pico_pio_usb
)

target_include_directories(neo6502firmware PUBLIC ${CMAKE_CURRENT_LIST_DIR}/core )

#delay programstart
#target_compile_definitions(neo6502firmware PRIVATE PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS=3000)

pico_enable_stdio_uart(neo6502firmware 1)
pico_enable_stdio_usb(neo6502firmware 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(neo6502firmware)


# add_custom_command(TARGET tiles_and_sprites POST_BUILD
#     COMMAND picotool load -F tiles_and_sprites.uf2 && picotool reboot
#     COMMENT "Loading ${PROJECT_NAME}.uf2"
# )