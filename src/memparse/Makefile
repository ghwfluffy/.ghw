CC=g++
STRIP=strip
DESTDIR=../../bin
CFLAGS=-W -Wall -Wextra -Wpedantic -Werror -O3
BIN=memparse
SRC=main.cpp

ALL_BIN=allmemparse
ALL_SRC=main.cpp

all:
	$(CC) $(SRC) $(CFLAGS) -o $(BIN)
	$(STRIP) $(BIN)
	$(CC) $(ALL_SRC) $(CFLAGS) -o $(ALL_BIN)
	$(STRIP) $(ALL_BIN)

install:
	cp $(BIN) $(ALL_BIN) $(DESTDIR)/

clean:
	rm -f $(BIN) $(ALL_BIN)
