import os
import struct
import glob

def pack_files(args):
    filepaths = []
    for path in args.filepaths:
        if '*' in path:
            filepaths.extend(glob.glob(path))
        else:
            filepaths.append(path)

    num_files = len(filepaths)

    asset_offsets = []
    file_data = []

    for filepath in filepaths:
        with open(filepath, 'rb') as infile:
            file_content = infile.read()
            file_data.append(file_content)

    with open(args.output, 'wb') as outfile:
        def allign():
            pos = outfile.tell()
            modulo = pos%4 # align to 4byte
            if modulo!=0:
                for i in range(4-modulo):
                    outfile.write(struct.pack('B', 0))

        # Write the number of files merged into the file
        outfile.write(struct.pack('B', num_files))

        # Write the file data
        for data in file_data:
            allign()

            asset_offsets.append((outfile.tell(),len(data)))
            outfile.write(data)


        allign()
        for pos,size in asset_offsets:
            outfile.write(struct.pack('I', pos))
        for pos,size in asset_offsets:
            outfile.write(struct.pack('I', size))

    write_header_file(args.output, filepaths, asset_offsets)

    print(f"Successfully packed {num_files} files into {args.output}:")
    for file in filepaths:
        print(f"\t \033[94m {file} \033[0m")
    print("\n")

def write_header_file(output_filename, filepaths, asset_offsets):
    base_name = os.path.splitext(os.path.basename(output_filename))[0]
    base_folder = os.path.dirname(output_filename)
    with open(base_folder+"/"+base_name + ".h", 'w') as header_file:
        header_file.write("#ifndef " + base_name.upper() + "_H\n")
        header_file.write("#define " + base_name.upper() + "_H\n")
        header_file.write("#include<stdint.h>\n\n")

        for i, filepath in enumerate(filepaths):
            define_name = "ASSET_" + os.path.splitext(os.path.basename(filepath))[0].upper()
            header_file.write(f"#define {define_name} {i}\n")

        header_file.write(f"\nextern const uint32_t {base_name}_offsets[];\n")
        header_file.write(f"extern const uint32_t {base_name}_sizes[];\n\n")

        header_file.write(f"#define {base_name.upper()}_AMOUNT {len(filepaths)}\n\n")

        header_file.write("#ifdef INCLUDE_DATA\n")
        header_file.write(f"extern const uint8_t {base_name}_data[];\n#endif\n")


        header_file.write("#endif // " + base_name.upper() + "_H\n")

    with open(base_folder+"/"+base_name + ".c", 'w') as offsets_file:
        offsets_file.write(f"#include \"{base_name}.h\"\n\n")
        offsets_file.write(f"const uint32_t {base_name}_offsets[] = {{\n")
        for offset,length in asset_offsets:
            offsets_file.write(f"    {offset},\n")
        offsets_file.write("};\n\n")

        offsets_file.write(f"const uint32_t {base_name}_sizes[] = {{\n")
        for offset,length in asset_offsets:
            offsets_file.write(f"    {length},\n")
        offsets_file.write("};\n\n")
        
        offsets_file.write("#ifdef INCLUDE_DATA\n")
        offsets_file.write(f"const uint8_t {base_name}_data[] = {{\n")
        counter = 0
        with open(output_filename,"rb") as gen_file:
            for b in gen_file.read():
                offsets_file.write(f"{hex(b)},")
                counter+=1
                if counter % 30 == 0:
                    offsets_file.write("\n")

        offsets_file.write("};\n\n#endif\n")