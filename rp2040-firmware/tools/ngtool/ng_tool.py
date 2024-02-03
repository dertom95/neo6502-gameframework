from PIL import Image
import argparse,os

from ng_tool_tilesheet import convert_tilesheet

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

    output_folder = os.path.dirname(header_file_path)
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    if height > 1:
        raise ValueError("Only palettes with height 1 are supported")
    if width > 256 or width <= 0:
        raise ValueError(f"Palette only supports with between [1..255] current-value:{width}")

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
    generate_header_file(args.input, args.output, args.array_name)


  

def main():
    # Create the main parser
    parser = argparse.ArgumentParser(description='Convert a PNG image to a C header file containing RGB565 pixel data.')

    # Create subparsers for the main commands
    subparsers = parser.add_subparsers(title='Main commands', dest='command')

    # Create a parser for create-palette command
    parser_create_palette = subparsers.add_parser('convert-palette', help='Create a color palette')
    parser_create_palette.add_argument('--input', help='Path to the input PNG image')
    parser_create_palette.add_argument('--output', help='Path to the output C header file')
    parser_create_palette.add_argument('--array-name', help='Name of the C array variable')

    parser_convert_tilesheet = subparsers.add_parser('convert-tilesheet', help='Convert tilesheet to header file')
    parser_convert_tilesheet.add_argument('--tile-size', help='Size of each tile in the format [width]x[height]')
    parser_convert_tilesheet.add_argument('--tilesheet-file', help='Path to the tilesheet image file')
    parser_convert_tilesheet.add_argument('--palette-file', help='Path to the palette image file')
    parser_convert_tilesheet.add_argument('--output', help='Path to the output header file')
    parser_convert_tilesheet.add_argument('--array-name', help='Name of the C array variable')
    parser_convert_tilesheet.add_argument('--transparent-idx', help="idx to be used for transparent! (default:255)", default=255)

    args = None
    # Parse the arguments
    try:
        args = parser.parse_args()
    except (argparse.ArgumentError, SystemExit):
        print("\033[91mERROR:\033[0m Unknown Command!\n")

    # Check if no command is provided
    if not args or not hasattr(args, 'command') or not args.command:
        parser.print_help()
        exit()

    # Call the appropriate function based on the command
    if args.command == 'convert-palette':
        convert_palette(args)
    elif args.command == 'convert-tilesheet':
        convert_tilesheet(args)


if __name__ == '__main__':
    main()
