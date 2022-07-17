CC=gcc
STRIP=strip
DESTDIR=$(shell pwd)/../bin

all:
	cd oid; $(CC) oid.c -o oid; $(STRIP) oid

clean:
	cd oid; rm -f oid

install:
	cd oid; cp oid $(DESTDIR)/oid
