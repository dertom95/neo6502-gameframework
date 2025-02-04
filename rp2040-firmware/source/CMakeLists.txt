set(BACKEND_SOURCE)
set(BACKEND PICO_NEO6502)

if(BACKEND STREQUAL "PICO_NEO6502")
add_definitions(-D${BACKEND} -DCHIPS_IMPL -DINCLUDE_DATA -DSOUND)
#add_definitions(-D${BACKEND} -DCHIPS_IMPL  -DSOUND)
list(APPEND BACKEND_SOURCE
			 core/backend/neo6502/neo6502_gfx.c
			 core/backend/neo6502/neo6502_io.c
			 core/backend/neo6502/neo6502_utils.c
			 core/backend/neo6502/neo6502_cpu.c
             core/backend/neo6502/neo6502_file.c
             core/backend/neo6502/neo6502_audio.c
             core/backend/neo6502/audio/audio.c
	)
endif()	

message(BACKEND_SOURCE ${BACKEND_SOURCE})

add_executable(neo6502firmware
    3rd/modplay/modplay.c

	core/memory.c
	core/backend/cpu/wdc65C02cpu.c
	core/roms.c
	
	ng_gfx.c
    ng_gfx_spritebuffer.c
    ng_gfx_pixelbuffer.c
    ng_gfx_tilemap.c
	ng_cpu.c
	ng_audio.c
    ng_sound_tsf.c
	ng_utils.c
	ng_io.c
	ng_mem.c
	ng_assets.c


	${BACKEND_SOURCE}

	main.c	
)

if(BACKEND STREQUAL "PICO_NEO6502")
	add_definitions(-D${BACKEND} -DCHIPS_IMPL)
	list(APPEND BACKEND_SOURCE
			 core/backend/neo6502/neo6502_gfx.c 
	)

	target_include_directories(neo6502firmware PUBLIC ${CMAKE_CURRENT_LIST_DIR}/core/backend/neo6502 ${CMAKE_SOURCE_DIR}/3rd/ff  )

endif()	


target_compile_options(neo6502firmware PRIVATE -Wall -g)

add_definitions(-DNDEBUG)
#target_compile_options(neo6502firmware PRIVATE -Wall -O3)

target_compile_definitions(neo6502firmware PRIVATE
#	DVI_DEFAULT_SERIAL_CONFIG=${DVI_DEFAULT_SERIAL_CONFIG}
	DVI_N_TMDS_BUFFERS=8
	)

add_library(fatfs INTERFACE)
target_sources(fatfs INTERFACE
    ${CMAKE_SOURCE_DIR}/3rd/ff/ff.c
    ${CMAKE_SOURCE_DIR}/3rd/ff/ffsystem.c
    ${CMAKE_SOURCE_DIR}/3rd/ff/ffunicode.c
    
)
target_include_directories(fatfs INTERFACE ${CMAKE_SOURCE_DIR}/3rd/ff/ /opt/pico-sdk/src/rp2_common/hardware_flash/include)

target_link_libraries(neo6502firmware
	pico_stdlib
	pico_multicore
	pico_util
	libdvi
	libsprite
    tinyusb_host
    tinyusb_device
    tinyusb_board
    fatfs
    hardware_flash    
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
