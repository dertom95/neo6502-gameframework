import os
from PIL import Image
from utils import convert_to_rgb565
import struct

def generate_header_file(image_path, header_file_path, array_name, binary=False):
    image = Image.open(image_path)
    pixels = image.convert("RGB")

    width, height = image.size

    output_folder = os.path.dirname(header_file_path)
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    if height > 1:
        raise ValueError("Only palettes with height 1 are supported")
    if width > 256 or width <= 0:
        raise ValueError(f"Palette only supports with between [1..255] current-value:{width}")

    if binary:
        with open(header_file_path, 'wb') as binary_file:
            binary_file.write(struct.pack('<H',width))
            for y in range(height):
                for x in range(width):
                    rgb = pixels.getpixel((x, y))
                    rgb565 = convert_to_rgb565(rgb)
                    binary_file.write(struct.pack('<H',rgb565))
        print('palette binary generated successfully.')
    else:
        with open(header_file_path, 'w') as header_file:
            header_file.write('#ifndef {0}_H\n'.format(array_name.upper()))
            header_file.write('#define {0}_H\n\n'.format(array_name.upper()))
            header_file.write('#include <stdint.h>\n\n')
            header_file.write('const uint16_t {0}[] = {{\n'.format(array_name))
            
            header_file.write(f'{width},')
            for y in range(height):
                for x in range(width):
                    rgb = pixels.getpixel((x, y))
                    rgb565 = convert_to_rgb565(rgb)
                    header_file.write('0x{:04X}, '.format(rgb565))

                header_file.write('\n')

            header_file.write('};\n\n')
            header_file.write('#endif // {0}_H'.format(array_name.upper()))

        print('C header file generated successfully.')


def convert_palette(args):
    # Logic for create-palette command
    print("Running create-palette command")
    print("Input:", args.input)
    print("Output:", args.output)
    print("Array Name:", args.array_name)
    if not args.binary and not args.array_name:
        raise ValueError("You need to name the array wiht --array-name if using header-output")
    generate_header_file(args.input, args.output, args.array_name,args.binary)