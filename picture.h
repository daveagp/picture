#ifndef PICTURE_H
#define PICTURE_H

#include <string>

struct Color; // see end of this file

class Picture {

public: 
// construct an all-black picture of the given height and width
Picture(int height, int width);
// create a deep copy of a picture
Picture(const Picture& p);
// open a .BMP file and copy it into a new Picture object
explicit Picture(const char filename[]);
// get dimensions of a picture
int height();
int width();
// get color at a particular location. needs 0<=row<height, 0<=col<width
Color get(int row, int col);
// set color at a particulat location. needs 0<=row<height, 0<=col<width
void set(int row, int col, Color new_color);
// show this picture. should it open in a new window? (default, no)
void show(bool new_window = false);
// save this picture to a .BMP file
void save(const char filename[]);
// for technical reasons: destructor
~Picture();
// for technical reasons: assignment operator
Picture& operator=(const Picture& p);


private:
// data members and implementation details
int _width;
int _height;
typedef unsigned char uint8;
uint8* data;
int _rowlen;

uint8* bgr(int row, int col);

static const int RGB = 3;
static const int BMP_HEADER_SIZE = 0x36;
static const int HEADER_DIMENSIONS_OFFSET = 0x12;
static const int DIB_SUBHEADER_SIZE = 0x28;

static int shows;

static std::string itoa(int i);
static int rowlen(int width);
typedef unsigned int uint32;
static uint8* header_append(uint8* hdr, int bytes, uint32 value);
static std::string nice(const char cstring[]);
public:
 static bool headless; // default, no. yes: show() -> save("lastshow.bmp")
};

#ifndef CS103_COLOR
#define CS103_COLOR
struct Color {
   int red;
   int green;
   int blue;
   Color() {} // uninitialized color
   Color(int r, int g, int b) {red = r; green = g; blue = b;}
   bool same_as(Color c) {return red==c.red && green==c.green && blue==c.blue;}
};
#endif

#endif
