import struct
from io import TextIOWrapper

# types:
# 'B' uint8_t 
# 'b' int8_t
# 'H' uint16_t
# 'h' int16_t
# 'I' uint32_t
# 'i' int32_t
def value_to_csv_le(value,type):
    byte_string = struct.pack(f'<{type}', value)

    # Convert the byte string to a comma-separated decimal string
    decimal_string = ','.join(str(byte) for byte in byte_string)   
    return decimal_string

def write_header(output:TextIOWrapper,type,size):
    result = f"{value_to_csv_le(type,'I')},{value_to_csv_le(size,'I')},"
    output.write(result)

def convert_to_rgb565(rgb):
    r, g, b = rgb
    r = (r >> 3) & 0x1F
    g = (g >> 2) & 0x3F
    b = (b >> 3) & 0x1F
    return (r << 11) | (g << 5) | b