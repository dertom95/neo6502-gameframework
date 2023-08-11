from PIL import Image
import argparse

def convert_to_rgb565(rgb):
    r, g, b = rgb

    # if (r>0):
    #     r=r-1
    # if (g>0):
    #     g=g-1
    # if (b>0):
    #     b=b-1

    r = (r >> 3) & 0x1F
    g = (g >> 2) & 0x3F
    b = (b >> 3) & 0x1F
    return (r << 11) | (g << 5) | b

def generate_header_file(image_path, header_file_path, array_name):
    image = Image.open(image_path)
    pixels = image.convert("RGB")

    width, height = image.size

    with open(header_file_path, 'w') as header_file:
        header_file.write('#ifndef {0}_H\n'.format(array_name.upper()))
        header_file.write('#define {0}_H\n\n'.format(array_name.upper()))
        header_file.write('#include <stdint.h>\n\n')
        header_file.write('static uint16_t {0}[] = {{\n'.format(array_name))

        for y in range(height):
            for x in range(width):
                rgb = pixels.getpixel((x, y))
                rgb565 = convert_to_rgb565(rgb)
                header_file.write('0x{:04X}, '.format(rgb565))

            header_file.write('\n')

        header_file.write('};\n\n')
        header_file.write('#endif // {0}_H'.format(array_name.upper()))

    print('C header file generated successfully.')

def main():
    parser = argparse.ArgumentParser(description='Convert a PNG image to a C header file containing RGB565 pixel data.')
    parser.add_argument('input', help='path to the input PNG image')
    parser.add_argument('output', help='path to the output C header file')
    parser.add_argument('array_name', help='name of the C array variable')

    args = parser.parse_args()

    generate_header_file(args.input, args.output, args.array_name)

if __name__ == '__main__':
    main()
