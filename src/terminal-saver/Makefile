CXX=g++
STRIP=strip
DESTDIR=../../bin
CFLAGS=-W -Wall -Wextra -Wpedantic -Werror -O3

all:
	$(CXX) main.cpp $(CFLAGS) -o terminal-saver
	$(STRIP) terminal-saver

install:
	cp terminal-saver $(DESTDIR)/

clean:
	rm -f terminal-saver
