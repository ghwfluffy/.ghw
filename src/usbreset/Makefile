CC=gcc
STRIP=strip
DESTDIR=../../bin
CFLAGS=-W -Wall -Wextra -Wpedantic -Werror -O3

all:
	$(CC) main.c $(CFLAGS) -o usbreset
	$(STRIP) usbreset

install:
	cp usbreset $(DESTDIR)/

clean:
	rm -f usbreset
