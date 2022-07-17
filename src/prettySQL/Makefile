CXX=g++
STRIP=strip
DESTDIR=../../bin
CFLAGS=-W -Wall -Wextra -Wpedantic -Werror -O3

all:
	$(CXX) main.cpp $(CFLAGS) -o prettySQL
	$(STRIP) prettySQL

install:
	cp prettySQL $(DESTDIR)/

clean:
	rm -f prettySQL
