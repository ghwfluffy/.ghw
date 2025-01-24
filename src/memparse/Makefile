CC=g++
STRIP=strip
DESTDIR=../../bin
CFLAGS=-W -Wall -Wextra -Wpedantic -Werror -O3
BIN=memparse
SRC=main.cpp

all:
	$(CC) $(SRC) $(CFLAGS) -o $(BIN)
	$(STRIP) $(BIN)

install:
	cp $(BIN) $(DESTDIR)/

clean:
	rm -f $(BIN)
