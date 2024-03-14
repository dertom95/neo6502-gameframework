cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(picodvi C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_compile_options(-Wall)

#set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)

# To change the default serial config for all apps, pass e.g.
#   cmake -DDVI_DEFAULT_SERIAL_CONFIG=pimoroni_demo_hdmi_cfg ..
# then rebuild. This is applied via target_compile_definitions on each app
# individually.

include_directories(
	3rd/PicoDVI/software/assets
	3rd/PicoDVI/software/include
    3rd/PicoDVI/software/libdvi
    3rd/PicoDVI/software/libsprite
	3rd/pico-mod-player/lib
	3rd/pico-mod-player/data
#	3rd/Pico-PIO-USB/src
)


add_subdirectory(source)

add_subdirectory(3rd/PicoDVI/software/libdvi)
add_subdirectory(3rd/PicoDVI/software/libsprite)
#add_subdirectory(3rd/Pico-PIO-USB)

#add_subdirectory(3rd/PicoDVI/software/apps)