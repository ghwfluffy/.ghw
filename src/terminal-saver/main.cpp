#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

namespace
{

enum class Color
{
    None        = 0x00,
    Black       = 0x30,
    Red         = 0x31,
    Green       = 0x32,
    Yellow      = 0x33,
    Blue        = 0x34,
    Pink        = 0x35,
    LightBlue   = 0x36,
    White       = 0x37,
};

/**
 * Print the characters to change terminal text color
 *
 * @param color The text color
 */
void startColor(Color color)
{
    char seq[] = {0x1B, 0x5B, 0x30, 0x3B, 0x33, (char)color, 0x6D};
    for (size_t ui = 0; ui < sizeof(seq); ui++)
        printf("%c", seq[ui]);
}

/**
 * Print the characters to reset terminal text color
 */
void endColor()
{
    char seq[] = {0x1B, 0x5B, 0x6D};
    for (size_t ui = 0; ui < sizeof(seq); ui++)
        printf("%c", seq[ui]);
}

}

/**
 * Just prints nonsense
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

    srand(time(nullptr));
    unsigned int sleep = 1;
    unsigned long long printed = 0;
    unsigned long long lineLength = 0;
    bool nosleep = false;
    while (true)
    {
        unsigned int entropy = static_cast<unsigned int>(rand());

        const bool enoughPrinted = printed++ > (500 * 100);
        if (enoughPrinted && (entropy >= 0x00001337 && entropy <= 0x01337000))
        {
            if (entropy & 0x1)
                printf("ghw");
            else if (entropy & 0x2)
                printf("Ghw");
            else if (entropy & 0x4)
                printf("GHW");
        }

        nosleep = lineLength < 80 || (entropy & 0xA) != 0;

        unsigned char u = entropy & 0xFF;
        if (u < 0x3)
        {
            if (enoughPrinted)
                printf("\r");
        }
        else if (u < 0x20)
        {
            if (lineLength > 80 && (entropy & 0xAA10) == 0xAA10)
            {
                printf("\n");
                lineLength = 0;
            }
        }
        else if (u >= 0x20 && u < 0x7F)
        {
            unsigned int chars = entropy % 8;
            for (unsigned int ui = 0; ui < chars; ui++)
            {
                char c = (char)(rand() % (0x7F-0x20)) + 0x20;
                if ((c >= 0x20 && c <= 0x2F) || (c >= 0x3A && c <= 0x40) || (c >= 0x5B && c <= 0x60))
                    printf("%c", c);
            }
            printf("%c", static_cast<char>(u));
            lineLength++;
        }
        else if (u < 0xA0)
        {
            endColor();
        }
        else if (u < 0xC0)
        {
            startColor(Color::Green);
        }
        else if (u < 0xC8)
        {
            startColor(Color::Red);
        }
        else if (u < 0xD0)
        {
            startColor(Color::Yellow);
        }
        else if (u < 0xD8)
        {
            startColor(Color::Blue);
        }
        else if (u < 0xE0)
        {
            startColor(Color::Pink);
        }
        else if (u < 0xE8)
        {
            startColor(Color::LightBlue);
        }
        else if (u < 0xF0)
        {
            startColor(Color::White);
        }
        else
        {
            endColor();
            printf(".");
            lineLength++;
        }

        if (enoughPrinted && !nosleep)
        {
            fflush(stdout);
            usleep(sleep);
            sleep += rand() % 500;
            if (sleep > 10000)
            {
                usleep((100 * 1000) * (rand() % 3));
                sleep = 100;
            }
        }
    }

    return 0;
}
