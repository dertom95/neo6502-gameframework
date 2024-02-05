import os
import numpy as np
from math import sqrt
from PIL import Image
from utils import value_to_csv_le,write_header
from ng_config import DATA_TYPE

def convert_tilesheet(args):
    # Your conversion logic here
    print("Converting tilesheet with the following arguments:")
    print(f"Tile size: {args.tile_size}")
    print(f"Tilesheet file: {args.tilesheet_file}")
    print(f"Palette file: {args.palette_file}")
    print(f"Output file: {args.output}")
    print(f"Array name: {args.array_name}") 
    print(f"Transparent-Idx: {args.transparent_idx}");

    if args.tile_size:
        if 'x' in args.tile_size:
            args.tile_width, args.tile_height = map(int, args.tile_size.split('x'))
        else:
            # Handle the case when the input format is incorrect
            print("Invalid tile size format. Please use the format [width]x[height].")
            exit(1)
    
    palette_colors = read_palette_colors(args.palette_file)
    convert_image_to_array(args.tilesheet_file, args.tile_width, args.tile_height, args.array_name, palette_colors, args.output)
    
def closest_color(rgb, colors):
    # Calculate the Euclidean distance between the RGB color and each color in the list
    distances = [np.linalg.norm(np.array(rgb) - np.array(color)) for color in colors]

    # Find the index of the closest color
    closest_index = distances.index(min(distances))

    return closest_index

 

def convert_image_to_array(image_filename, tile_width, tile_height, array_name, colors, output_filename, transparaent_idx=255):
    # Load the image
    image = Image.open(image_filename)

    # Convert the image to RGB if it's RGBA
    # if image.mode == 'RGBA':
    #     image = image.convert('RGB')

    # Get the image size
    width, height = image.size

    # Calculate the number of tiles in each dimension
    num_tiles_x = width // tile_width
    num_tiles_y = height // tile_height

    # Create the output file
    output_file = open(output_filename, 'w')

    # Write the array declaration to the output file

    output_file.write(f'#ifndef {array_name.upper()}_H\n')
    output_file.write(f'#define {array_name.upper()}_H\n\n')
    output_file.write(f'#include <stdint.h>\n\n')
    output_file.write(f"const uint8_t {array_name}[] = {{\n")
    
    #write_header(output_file,DATA_TYPE.DT_TILESHEET,num_tiles_x * num_tiles_y * tile_width * tile_height)
    tilesheet_size = num_tiles_x * num_tiles_y * tile_width * tile_height
    if tilesheet_size>65535:
        raise ValueError(f"Tilesheet {array_name} too huge!({tilesheet_size} bytes) only 65535 allowed!")
    
    output_file.write("0,0, 0,0, 0,0,0,0, ") # ng_mem_block_t (u16[start], u16[size], u32[ptr])
    output_file.write(f'{value_to_csv_le(tilesheet_size,"H")},{tile_width},{tile_height},{num_tiles_x},{num_tiles_x*num_tiles_y},')
    output_file.write('0,0,1,0, ') # 16bit flags, format and one pad
    output_file.write('0,0,0,0, ') # tilesheet_data_ram pointer - data
    output_file.write('0,0,0,0, ') # tilesheet_data_flah pointer - data
    array_size = 0
    # Iterate over each tile
    for y in range(num_tiles_y):
        for x in range(num_tiles_x):
            # Get the tile pixels
            tile_pixels = image.crop((x * tile_width, y * tile_height, (x + 1) * tile_width, (y + 1) * tile_height))
            
            # Iterate over each pixel in the tile
            for pixel in tile_pixels.getdata():
                array_size += 1
                
                if array_size % 16 == 0:
                    output_file.write(f"\n")

                if (len(pixel)==4 and pixel[3]<128):
                    color_index = transparaent_idx
                else:
                    # Convert the RGB value to the closest color index
                    color_index = closest_color(pixel[:3], colors)

                # Write the color index to the output file
                output_file.write(f"{color_index}, ")
            

    # Write the closing brace to the output file
    output_file.write("\n};")

    output_file.write(f"""
        const uint8_t {array_name}_tiles = {num_tiles_x*num_tiles_y};
        const uint8_t {array_name}_tiles_x = {num_tiles_x};
        const uint8_t {array_name}_tiles_y = {num_tiles_y};
        const uint8_t {array_name}_tile_width = {tile_width};
        const uint8_t {array_name}_tile_height = {tile_height};
        const uint16_t {array_name}_size = {array_size};
#endif    
    """)

    # Close the output file
    output_file.close()

def read_palette_colors(image):
    # If the image parameter is a filename, open it as an image
    if isinstance(image, str):
        image = Image.open(image)

    # Get the pixel data of the image
    pixels = image.load()

    # Get the width of the image
    width = image.width

    # Read the RGB values from left to right
    colors = []
    for x in range(width):
        r, g, b, _ = pixels[x, 0]
        colors.append((r, g, b))

    return tuple(colors)

def convert_rgba_to_rgb(source_filename, alpha_pixel=(255,0,255),outfile=None):
    # Load the RGBA image
    rgba_image = Image.open(source_filename).convert('RGBA')

    # Create a new RGB image with the same size
    rgb_image = Image.new('RGB', rgba_image.size)

    # Get the pixel data of the images
    rgba_pixels = rgba_image.load()
    rgb_pixels = rgb_image.load()

    # Iterate through each pixel in the image
    for y in range(rgba_image.height):
        for x in range(rgba_image.width):
            r, g, b, a = rgba_pixels[x, y]

            if a == 0:
                rgb_pixels[x, y] = alpha_pixel
            else:
                rgb_pixels[x, y] = (r, g, b)
       # Save the converted image
    rgb_image.save(outfile)
    return rgb_image




