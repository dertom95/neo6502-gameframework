set(BACKEND_SOURCE)
set(BACKEND PICO_NEO6502)

if(BACKEND STREQUAL "PICO_NEO6502")
	add_definitions(-D${BACKEND} -DCHIPS_IMPL -DINCLUDE_DATA)
	list(APPEND BACKEND_SOURCE
			 core/backend/neo6502/neo6502_gfx.c
			 core/backend/neo6502/neo6502_io.c
			 core/backend/neo6502/neo6502_utils.c
			 core/backend/neo6502/neo6502_cpu.c
	)
endif()	

message(BACKEND_SOURCE ${BACKEND_SOURCE})

add_executable(neo6502firmware
	${CMAKE_SOURCE_DIR}/3rd/pico-mod-player/lib/audio.c
	${CMAKE_SOURCE_DIR}/3rd/pico-mod-player/lib/mod_play.c

	core/memory.c
	core/backend/cpu/wdc65C02cpu.c
	core/roms.c
	

	ng_gfx.c
	ng_cpu.c
	ng_sound.c
	ng_utils.c
	ng_io.c
	ng_mem.c
	ng_assets.c

	#api/gen/memory_call_function.c

#	game/game.c

	${BACKEND_SOURCE}

	main.c	
)

if(BACKEND STREQUAL "PICO_NEO6502")
	add_definitions(-D${BACKEND} -DCHIPS_IMPL)
	list(APPEND BACKEND_SOURCE
			 core/backend/neo6502/neo6502_gfx.c
	)

	target_include_directories(neo6502firmware PUBLIC ${CMAKE_CURRENT_LIST_DIR}/core/backend/neo6502  )

endif()	



#target_compile_options(neo6502firmware PRIVATE -Wall -g)
target_compile_options(neo6502firmware PRIVATE -Wall -O2)

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

target_include_directories(neo6502firmware PUBLIC ${CMAKE_CURRENT_LIST_DIR}/core  )

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