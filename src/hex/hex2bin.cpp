#include <stdio.h>
#include <unistd.h>

/**
 * Convert hex data (even number of bytes) from stdin to binary stdout
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

    // Convert stdin to hex
    char data[2048] = {};
    ssize_t len = 0;
    bool bEven = false;
    unsigned char val = 0;
    bool hasNewline = false;
    while ((len = fread(data, 1, sizeof(data), stdin)) > 0)
    {
        for (size_t ui = 0; ui < static_cast<size_t>(len); ui++)
        {
            unsigned char intVal = 0;
            char c = data[ui];
            if (c >= '0' && c <= '9')
                intVal = c - '0';
            else if (c >= 'a' && c <= 'f')
                intVal = c - 'a' + 10;
            else if (c >= 'A' && c <= 'F')
                intVal = c - 'A' + 10;
            else
                continue;

            val |= intVal << (bEven ? 0 : 4);
            if (bEven)
            {
                printf("%c", static_cast<char>(val));
                hasNewline = val == '\n';
                val = 0;
            }
            bEven = !bEven;
        }
    }

    // No newline for pipes
    if (!hasNewline && isatty(fileno(stdout)))
        printf("\n");

    return 0;
}
