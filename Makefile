# contact: David Pritchard

CC = compile
# if needed, install from http://bits.usc.edu/cs103/compile/ or use g++ or clang++

EXAMPLES = invert grayscale instagram separate tile zoom glass edges scale

all: picture.o $(EXAMPLES)

picture.o: picture.h picture.cpp jpgd.cpp jpgd.h
	$(CC) -Wno-shadow -Wno-unused-const-variable -Wno-unreachable-code picture.cpp -c

%: %.cpp picture.o picture.h
	$(CC) $@.cpp picture.o -o $@

clean:
	rm -f picture.o $(EXAMPLES)

zip:
	rm -f picture.zip
	zip -r picture.zip *.cpp picture.h jpgd.h Makefile fruit.bmp

