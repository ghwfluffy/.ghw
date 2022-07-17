// Alan Stern <stern@rowland.harvard.edu>
// https://marc.info/?l=linux-usb&m=121459435621262&w=2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

#include <linux/usbdevice_fs.h>

int main(int argc, const char *argv[])
{
    // Usage help
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <Bus> <Device>\n", argv[0]);
        return 1;
    }

    // Get arguments
    int bus = atoi(argv[1]);
    int dev = atoi(argv[2]);

    // Build device file path
    char filename[1024] = {0};
    snprintf(filename, sizeof(filename), "/dev/bus/usb/%d/%d", bus, dev);

    // Open device file
    int fd = open(filename, O_WRONLY);
    if (fd < 0)
    {
        fprintf(stderr, "Error opening USB device '%s'.", filename);
        return 2;
    }

    // Perform reset
    printf("Resetting USB device %s\n", filename);
    int rc = ioctl(fd, USBDEVFS_RESET, 0);
    if (rc < 0)
    {
        fprintf(stderr, "Failed to reset USB device with %d (errno %d).", rc, errno);
        close(fd);
        return 3;
    }

    printf("Reset successful\n");

    close(fd);
    return 0;
}
