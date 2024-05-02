from ng_tool_tilemap_spritefusion import read_spritefusion_to_tilemap
from ng_tool_tilemap_formats import Tilemap,Layer

def serialize_tilemap(tilemap):
    layer_amount = len(tilemap.layers)
    width = tilemap.width
    height = tilemap.height
    flags = tilemap.flags  # Placeholder value, you can modify this as needed

    # Calculate the size of the layer offsets section
    layer_offsets_size = layer_amount * 2  # Each offset is 2 bytes (uint16_t)

    # Calculate the size of the entire byte array
    byte_array_size = 4 + layer_offsets_size + (5 + width * height) * layer_amount

    # Create the byte array with padding
    byte_array = bytearray(byte_array_size)

    # Serialize the tilemap header
    byte_array[0] = layer_amount
    byte_array[1] = width
    byte_array[2] = height
    byte_array[3] = flags

    # Calculate and serialize the layer offsets
    offset = 4 + layer_offsets_size
    store_offset = offset
    for i, layer in enumerate(tilemap.layers):
        offset_bytes = offset.to_bytes(2, 'little')
        byte_array[4 + 2 * i] = offset_bytes[0]
        byte_array[5 + 2 * i] = offset_bytes[1]
        offset += 5 + width * height
    
    offset = store_offset
    # Serialize the layer data
    for i, layer in enumerate(tilemap.layers):
        layer_id = i  # Assuming layer IDs are 0-indexed and sequential
        layer_flags = 0  # Placeholder value, you can modify this as needed

        # Serialize the layer header
        byte_array[offset] = layer_id
        byte_array[offset + 1] = layer_flags

        # Serialize the tile indices
        tile_indices = layer.tiles
        byte_array[offset + 2:offset + 2 + width * height] = tile_indices

        # Calculate and serialize the checksum
        checksum = 95  # Placeholder value, you can modify this as needed
        byte_array[offset + 2 + width * height] = checksum

        # Update the offset for the next layer
        offset += 5 + width * height

    return byte_array


def create_tilemap(args):
    if args.format=="spritefusion":
        tilemap = read_spritefusion_to_tilemap(args.input)
        bytes = serialize_tilemap(tilemap)
        with open(args.output,"wb") as file:
            file.write(bytes)
    else:
        raise ValueError(f"{args.fileformat} not supported, yet")
    