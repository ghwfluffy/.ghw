#include <stdio.h>
#include <unistd.h>

namespace
{

constexpr const char encoding[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
constexpr const char webEncoding[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

static_assert((sizeof(encoding) - sizeof('\0')) == 64, "Encoding requires 64 characters.");
static_assert((sizeof(webEncoding) - sizeof('\0')) == 64, "Web encoding requires 64 characters.");

}

/**
 * Convert binary data to Base64 encoding.
 * Optionally use URL (web) encoding.
 *
 * @param argc stdargs argument count
 * @param argv stdargs
 *
 * @return 0 (Success) if encoded, 1 if stdargs help message displayed
 */
int main(int argc, const char *argv[])
{
    const char *convert = encoding;

    // Optional argument 1 can specify "web" or "url" encoding
    if (argc > 1)
    {
        // Lazy parse
        char option = argv[1][0];
        if (option == 'w' || option == 'W' || option == 'u' || option == 'U')
            convert = webEncoding;
        else
        {
            fprintf(stderr, "Base64 encode stdin data\n");
            fprintf(stderr, "Optional web/url encoding\n");
            fprintf(stderr, "Usage: %s [web/url]\n", argv[0]);
            return 1;
        }
    }

    // Convert stdin to base64
    char data[2048] = {};
    ssize_t len = 0;
    unsigned char bits = 0;
    unsigned char curVal = 0;
    unsigned long long total = 0;
    while ((len = fread(data, 1, sizeof(data), stdin)) > 0)
    {
        for (size_t ui = 0; ui < static_cast<size_t>(len); ui++)
        {
            unsigned char uc = static_cast<unsigned char>(data[ui]);
            // Take 6
            // Keep 2
            if (bits == 0)
            {
                unsigned char index = uc >> 2;
                printf("%c", convert[index]);
                ++total;
                curVal = uc & 0x03;
                bits = 2;
            }
            // Take 4
            // Keep 4
            else if (bits == 2)
            {
                curVal <<= 4;
                unsigned char index = curVal | (uc >> 4);
                printf("%c", convert[index]);
                ++total;
                curVal = uc & 0x0F;
                bits = 4;
            }
            // Take 2
            // Take 6
            // Keep 0
            else if (bits == 4)
            {
                // Take 2
                curVal <<= 2;
                unsigned char index = curVal | (uc >> 6);
                printf("%c", convert[index]);
                ++total;
                // Take 6
                index = uc & 0x3F;
                printf("%c", convert[index]);
                ++total;
                bits = 0;
            }
        }
    }

    // Even it out
    while (total & 3)
    {
        printf("=");
        ++total;
    }

    // No newline for pipes
    if (isatty(fileno(stdout)))
        printf("\n");

    return 0;
}
