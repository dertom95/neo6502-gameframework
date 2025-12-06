#include "ng_api_shared.h"

// Convert float to Q8.8
q8_8_t float_to_q8_8(float f) {
    if (f < 0.0f) return 0;
    int32_t val = (int)(f * 256.0f + 0.5f);
    if (val > 65535) val = 65535;
    return (q8_8_t)val;
}

// Convert Q8.8 to float
float q8_8_to_float(q8_8_t x) {
    return ((float)x) / 256.0f;
}

// Fast multiplication: (a * b) >> 8
q8_8_t q8_8_mul(q8_8_t a, q8_8_t b) {
    return (q8_8_t)(((uint32_t)a * (uint32_t)b) >> 8);
}

// Helper function to handle unsigned integers
int snprintf_uint8(char* str, uint8_t size, uint8_t num) {
    int written = 0;
    char buffer[4]; // Enough to hold max uint8_t value (255) as a string
    int index = 0;

    // Convert number to string in reverse order
    do {
        buffer[index++] = '0' + (num % 10);
        num /= 10;
    } while (num > 0 && index < sizeof(buffer));

    // Reverse the string and copy to the output buffer
    while (index > 0 && written < size - 1) {
        str[written++] = buffer[--index];
    }

    // Null-terminate the string
    str[written] = '\0';

    return written;
}

// Helper function to handle signed integers
int snprintf_int(char* str, uint8_t size, int num) {
    int written = 0;

    // Handle negative numbers
    if (num < 0) {
        if (written < size - 1) {
            str[written++] = '-';
        }
        num = -num; // Convert to positive for further processing
    }

    // Use the unsigned integer function to handle the absolute value
    written += snprintf_uint8(str + written, size - written, (uint8_t)num);

    return written;
}

// Helper function to handle strings
int snprintf_str(char* str, uint8_t size, const char* s) {
    int written = 0;

    // Copy the string to the buffer
    for (; *s != '\0' && written < size - 1; ++s, ++written) {
        str[written] = *s;
    }

    // Null-terminate the string
    str[written] = '\0';

    return written;
}

// Main ng_snprintf function
int ng_snprintf(char* str, uint8_t size, const char* format, ...) {
    int written = 0;
    bool isOverflow = false;

    // Check if buffer size is zero
    if (size == 0) {
        return 0;
    }

    // Start variadic arguments
    va_list args;
    va_start(args, format);

    // Iterate through the format string
    for (const char* p = format; *p != '\0'; ++p) {
        // Check if buffer overflow has occurred
        if (written >= size - 1) {
            isOverflow = true;
            break;
        }

        // Handle format specifier
        if (*p == '%') {
            // Handle the next character after '%'
            ++p;

            // Handle format specifiers
            switch (*p) {
                case 'b': {
                    // Handle unsigned integer format specifier
                    uint8_t num = va_arg(args, uint8_t);
                    written += snprintf_uint8(str + written, size - written, num);
                    break;
                }
                case 'd': {
                    // Handle signed integer format specifier
                    int num = va_arg(args, int);
                    written += snprintf_int(str + written, size - written, num);
                    break;
                }
                case 's': {
                    // Handle string format specifier
                    const char* s = va_arg(args, const char*);
                    written += snprintf_str(str + written, size - written, s);
                    break;
                }
                // Add more format specifiers as needed
                default:
                    // Handle unsupported format specifier
                    if (written < size - 1) {
                        str[written++] = *p;
                    }
                    break;
            }
        } else {
            // Copy non-format characters to the buffer
            if (written < size - 1) {
                str[written++] = *p;
            }
        }
    }

    // Null-terminate the string
    if (written < size) {
        str[written] = '\0';
    } else if (size > 0) {
        str[size - 1] = '\0';
    }

    // Cleanup variadic arguments
    va_end(args);

    // Return the number of characters that would have been written
    return isOverflow ? -1 : written;
}

