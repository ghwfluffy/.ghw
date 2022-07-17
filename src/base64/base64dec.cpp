#include <stdio.h>
#include <unistd.h>

namespace
{

/**
 * Convert a Base64 character to the associated integer value.
 * Supports all Base64 encoding schemes.
 *
 * @param curChar Base64 character
 *
 * @return Associated integer value, -1 if not a valid Base64 character
 */
char charToVal(char curChar)
{
    char ret = -1;
    if (curChar >= 'A' && curChar <= 'Z')
        ret = curChar - 'A';
    else if (curChar >= 'a' && curChar <= 'z')
        ret = curChar - 'a' + 26;
    else if (curChar >= '0' && curChar <= '9')
        ret = curChar - '0' + 52;
    else if (curChar == '+' || curChar == '-')
        ret = 62;
    else if (curChar == '/' || curChar == '_' || curChar == ',')
        ret = 63;
    return ret;
}

}

/**
 * Convert base64 data from stdin to binary data to stdout.
 * If outputting to a terminal, the text will always end in a newline.
 *
 * @param argc Ignored
 * @param argv Ignored
 *
 * @return 0 (Success)
 */
int main(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;

    // Convert stdin to binary
    char data[2048] = {};
    ssize_t len = 0;
    unsigned char bits = 0;
    unsigned char curVal = 0;
    bool hasNewline = false;
    while ((len = fread(data, 1, sizeof(data), stdin)) > 0)
    {
        for (size_t ui = 0; ui < static_cast<size_t>(len); ui++)
        {
            char c = data[ui];
            char intVal = charToVal(c);
            if (intVal < 0)
                continue;

            // Keep 6 bits
            if (bits == 0)
            {
                curVal = intVal;
                bits = 6;
            }
            // Take all 6 bits
            else if (bits == 2)
            {
                curVal <<= 6;
                curVal |= intVal;
                printf("%c", curVal);
                hasNewline = curVal == '\n';
                bits = 0;
            }
            // Take 4 bits
            // Keep 2 bits
            else if (bits == 4)
            {
                curVal <<= 4;
                curVal |= (intVal >> 2);
                printf("%c", curVal);
                hasNewline = curVal == '\n';
                curVal = intVal & 0x03;
                bits = 2;
            }
            // Take 2 bits
            // Keep 4 bits
            else if (bits == 6)
            {
                curVal <<= 2;
                curVal |= (intVal >> 4);
                printf("%c", curVal);
                hasNewline = curVal == '\n';
                curVal = intVal & 0x0F;
                bits = 4;
            }
        }
    }

    // No newline for pipes
    if (!hasNewline && isatty(fileno(stdout)))
        printf("\n");

    return 0;
}
