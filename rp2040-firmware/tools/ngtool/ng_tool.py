
import argparse,os

from ng_config import DATA_TYPE

from ng_tool_tilesheet import convert_tilesheet
from ng_tool_packfiles import pack_files
from ng_tool_palette import convert_palette

from utils import write_header

  

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
    parser_create_palette.add_argument('--binary',action='store_true', help='Export as binary')

    parser_convert_tilesheet = subparsers.add_parser('convert-tilesheet', help='Convert tilesheet to header file')
    parser_convert_tilesheet.add_argument('--tile-size', help='Size of each tile in the format [width]x[height]')
    parser_convert_tilesheet.add_argument('--tilesheet-file', help='Path to the tilesheet image file')
    parser_convert_tilesheet.add_argument('--palette-file', help='Path to the palette image file')
    parser_convert_tilesheet.add_argument('--output', help='Output File-Path')
    parser_convert_tilesheet.add_argument('--array-name', help='Name of the C array variable')
    parser_convert_tilesheet.add_argument('--transparent-idx', help="idx to be used for transparent! (default:255)", default=255)
    parser_convert_tilesheet.add_argument('--binary',action='store_true', help='Export as binary')
    
    packfiles_parser = subparsers.add_parser("pack-files",description='Pack multiple files into one binary file, including h and c support files.')
    packfiles_parser.add_argument('filepaths', nargs='+', help='Filepaths to pack')
    packfiles_parser.add_argument('--output', help='Output file path')

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
    elif args.command == 'pack-files':
        pack_files(args)


if __name__ == '__main__':
    main()
