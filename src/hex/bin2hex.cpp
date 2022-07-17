#include <stdio.h>
#include <unistd.h>

/**
 * Convert binary data from stdin to hex and print to stdout
 *
 * @param argc Ignored
 * @param argv Ignore
 *
 * @return 0 (Success)
 */
int main(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;

    // Convert stdin to hex
    unsigned char data[2048] = {};
    ssize_t len = 0;
    while ((len = fread(data, 1, sizeof(data), stdin)) > 0)
    {
        for (size_t ui = 0; ui < static_cast<size_t>(len); ui++)
            printf("%02hhX", data[ui]);
    }

    // No newline for pipes
    if (isatty(fileno(stdout)))
        printf("\n");

    return 0;
}
