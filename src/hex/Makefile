CXX=g++
STRIP=strip
DESTDIR=../../bin
CFLAGS=-W -Wall -Wextra -Wpedantic -Werror -O3

all:
	$(CXX) bin2hex.cpp $(CFLAGS) -o bin2hex
	$(CXX) hex2bin.cpp $(CFLAGS) -o hex2bin
	$(STRIP) bin2hex hex2bin

install:
	cp bin2hex $(DESTDIR)/
	cp hex2bin $(DESTDIR)/

clean:
	rm -f bin2hex hex2bin
