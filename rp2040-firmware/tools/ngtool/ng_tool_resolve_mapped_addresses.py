import re

# Define a mapping for sizeof to simulate C data type sizes
SIZEOF_MAP = {
    "uint8_t": 1,
    "int8_t": 1,
    "uint16_t": 2,
    "int16_t": 2,
    "uint32_t": 4,
    "int32_t": 4,
    "gamepad_t": 11,  # Example size for a custom struct
    "gamepad_state_t": 2,  # Example size for another custom struct
}

# Define the allowed prefixes for output
ALLOWED_PREFIXES = ["MM_", "FUNCTION_"]

def sizeof(data_type):
    """
    Simulate the sizeof operator for C data types.
    """
    if data_type not in SIZEOF_MAP:
        raise ValueError(f"Unknown data type for sizeof: {data_type}")
    return SIZEOF_MAP[data_type]

def extract_defines(header_file):
    """
    Extract #define macros from the given header file.
    Returns a dictionary of macro names and their expressions.
    """
    defines = {}
    define_pattern = re.compile(r"#define\s+(\w+)\s+(.+)")
    
    with open(header_file, "r") as file:
        for line in file:
            # Remove comments
            line = re.sub(r"//.*", "", line)
            line = re.sub(r"/\*.*?\*/", "", line)
            line = line.strip()
            
            # Match #define lines
            match = define_pattern.match(line)
            if match:
                name, expression = match.groups()
                defines[name] = expression.strip()
    return defines

def resolve_macro(name, defines, resolved):
    """
    Recursively resolve the value of a macro.
    """
    if name in resolved:
        return resolved[name]  # Return already resolved value
    
    if name not in defines:
        raise ValueError(f"Undefined macro: {name}")
    
    expression = defines[name]
    
    # Replace references to other macros in the expression
    for ref_name in defines:
        # Replace only full macro names, not parts of other names
        pattern = r"\b" + re.escape(ref_name) + r"\b"
        if re.search(pattern, expression):
            ref_value = resolve_macro(ref_name, defines, resolved)
            expression = re.sub(pattern, str(ref_value), expression)
    
    # Replace sizeof(type) with the corresponding size
    sizeof_pattern = re.compile(r"sizeof\((\w+)\)")
    expression = sizeof_pattern.sub(lambda match: str(sizeof(match.group(1))), expression)
    
    # Evaluate the final expression
    try:
        resolved_value = eval(expression)
    except Exception as e:
        raise ValueError(f"Error evaluating macro {name}: {e}")
    
    resolved[name] = resolved_value
    return resolved_value

def resolve_all_macros(defines):
    """
    Resolve all macros in the given dictionary.
    Returns a dictionary of resolved macro values.
    """
    resolved = {}
    for name in defines:
        resolve_macro(name, defines, resolved)
    return resolved

def write_results_to_file(resolved, defines, output_file):
    """
    Write the resolved macros to a file in ascending order with hexadecimal values and memory size.
    """
    # Filter macros for output based on allowed prefixes
    filtered_resolved = {
        name: value for name, value in resolved.items()
        if any(name.startswith(prefix) for prefix in ALLOWED_PREFIXES)
    }
    
    # Sort the filtered macros by their resolved value (address)
    sorted_resolved = sorted(filtered_resolved.items(), key=lambda x: x[1])
    
    with open(output_file, "w") as file:
        file.write("<memorymap>\n")
        for name, value in sorted_resolved:
            hex_value = f"[0x{value:04X}]"
            # Calculate the size in bytes (default to 1 byte if not specified)
            size_in_bytes = 1  # Default size
            if "sizeof" in defines.get(name, ""):
                size_in_bytes = sizeof(re.search(r"sizeof\((\w+)\)", defines[name]).group(1))
            file.write(f"  <entry name=\"{name}\" address=\"{value}\" hex=\"{hex_value}\" bytes=\"{size_in_bytes}\" />\n")
        file.write("</memorymap>\n")

def main(header_files, output_file):
    """
    Main function to parse, resolve, and write macro results.
    """
    all_defines = {}
    
    # Parse all header files
    for header_file in header_files:
        defines = extract_defines(header_file)
        all_defines.update(defines)
    
    # Resolve all macros
    resolved_macros = resolve_all_macros(all_defines)
    
    # Write results to the output file
    write_results_to_file(resolved_macros, all_defines, output_file)
    print(f"Resolved macros written to {output_file}")


if __name__ == "__main__":
    # Specify the header files to process
    header_files = ["api/ng_memory_map.h"]  # Replace with your header files
    output_file = "resolved_macros.xml"
    
    main(header_files, output_file)
