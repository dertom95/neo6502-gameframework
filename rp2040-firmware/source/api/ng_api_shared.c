#include "ng_api_shared.h"


int snprintf_int(char* str, uint8_t size, int num)
{
    // Handle negative numbers
    bool isNegative = false;
    if (num < 0)
    {
        isNegative = true;
        num = -num;
    }

    // Convert the integer to a string
    int written = 0;
    do
    {
        // Check if buffer overflow has occurred
        if (written >= size - 1)
            break;

        // Calculate the digit and add it to the string
        str[written++] = '0' + num % 10;
        num /= 10;
    } while (num > 0);

    // Add the negative sign if necessary
    if (isNegative && written < size - 1)
        str[written++] = '-';

    // Reverse the string
    for (int i = 0, j = written - 1; i < j; ++i, --j)
    {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }

    // Null-terminate the string
    str[written] = '\0';

    // Return the number of characters written
    return written;
}

int snprintf_str(char* str, uint8_t size, const char* s)
{
    // Copy the string to the buffer
    int written = 0;
    for (; *s != '\0' && written < size - 1; ++s, ++written)
        str[written] = *s;

    // Null-terminate the string
    str[written] = '\0';

    // Return the number of characters written
    return written;
}


int ng_snprintf(char* str, uint8_t size, const char* format, ...)
{
    int written = 0;
    bool isOverflow = false;

    // Check if buffer size is zero
    if (size == 0)
        return 0;

    // Start variadic arguments
    va_list args;
    va_start(args, format);

    // Iterate through the format string
    for (const char* p = format; *p != '\0'; ++p)
    {
        // Check if buffer overflow has occurred
        if (written >= size - 1)
        {
            isOverflow = true;
            break;
        }

        // Handle format specifier
        if (*p == '%')
        {
            // Handle the next character after '%'
            ++p;

            // Handle format specifiers
            switch (*p)
            {
                case 'd':
                {
                    // Handle integer format specifier
                    int num = va_arg(args, int);
                    written += snprintf_int(str + written, size - written, num);
                    break;
                }
                case 's':
                {
                    // Handle string format specifier
                    const char* s = va_arg(args, const char*);
                    written += snprintf_str(str + written, size - written, s);
                    break;
                }
                // Add more format specifiers as needed
                default:
                    // Handle unsupported format specifier
                    str[written++] = *p;
                    break;
            }
        }
        else
        {
            // Copy non-format characters to the buffer
            str[written++] = *p;
        }
    }

    // Null-terminate the string
    str[written] = '\0';

    // Cleanup variadic arguments
    va_end(args);

    // Return the number of characters that would have been written
    return isOverflow ? -1 : written;
}

