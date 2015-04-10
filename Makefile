# contact: David Pritchard

CC = clang++

# this is for CS 103 at USC specifically.
# bash doesn't expand aliases. patch student machine. prefer 'compile'.
ifeq ("$(wildcard /bin/compile)","")
ifneq ("$(wildcard ~/.compile.py)","")
PATCH = sudo cp ~/.compile.py /bin/compile; sudo chmod a+x /bin/compile
CC = compile
CFLAGS = 
endif
else
CC = compile
CFLAGS = 
endif

EXAMPLES = invert grayscale instagram separate tile zoom glass edges scale

all: picture.o $(EXAMPLES)

picture.o: picture.h picture.cpp jpgd.cpp jpgd.h
	$(PATCH)
	$(CC) -Wno-shadow -Wno-unused-const-variable picture.cpp -c

%: %.cpp picture.o picture.h
	$(CC) $@.cpp picture.o $(FLAGS) -o $@

clean:
	rm -f picture.o $(EXAMPLES)

zip:
	rm -f picture.zip
	zip -r picture.zip *.cpp picture.h jpgd.h Makefile fruit.bmp

