.PHONY: all clean run build-pre build-post

all: build

build: so-cpp.o
	gcc so-cpp.o -o so-cpp

so-cpp.o: so-cpp.c
	gcc -c -Wall so-cpp.c

clean:
	rm so-cpp so-cpp.o
