CXX=g++
STRIP=strip
DESTDIR=../../bin
CFLAGS=-W -Wall -Wextra -Wpedantic -Werror -O3

all:
	$(CXX) base64enc.cpp $(CFLAGS) -o base64enc
	$(CXX) base64dec.cpp $(CFLAGS) -o base64dec
	$(STRIP) base64enc base64dec

install:
	cp base64enc $(DESTDIR)/
	cp base64dec $(DESTDIR)/

clean:
	rm -f base64enc base64dec
