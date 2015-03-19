#ifndef PICTURE_H
#define PICTURE_H

#include <string>

class Picture {

public: // use Picture::Color to contain color data
struct Color {
   int red;
   int green;
   int blue;
};

// construct an all-black picture of the given height and width
Picture(int height, int width);
// create a deep copy of a picture
Picture(Picture& p);
// open a .BMP file and copy it into a new Picture object
Picture(const char filename[]);
// get dimensions of a picture
int height();
int width();
// get color at a particular location. needs 0<=row<height, 0<=col<width
Picture::Color get(int row, int col);
// set color at a particulat location. needs 0<=row<height, 0<=col<width
void set(int row, int col, Picture::Color new_color);
// show this picture. should it open in a new window? (default, no)
void show(bool new_window = false);
// save this picture to a .BMP file
void save(const char filename[]);
// destructor
~Picture();


private:
// data members
int _width;
int _height;
typedef unsigned char uint8;
uint8* data;
int _rowlen;
uint8* bgr(int row, int col);

// implementation details
typedef unsigned short int uint16;
typedef unsigned int uint32;

static std::string itoa(int i);
static int rowlen(int width);
static int shows;

uint8* header_append(uint8* hdr, int bytes, uint32 value);

static const int RGB = 3;
static const int BMP_HEADER_SIZE = 0x36;
static const int HEADER_DIMENSIONS_OFFSET = 0x12;
static const int DIB_SUBHEADER_SIZE = 0x28;

};

#endif
