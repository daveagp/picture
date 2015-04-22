#include "picture.h"
// 'unity build' - include cpp so we just have one object
#include "jpgd.cpp"

#include <stdio.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <cstring>
#include <cctype>

using namespace std;

int Picture::width() {
   return _width;
}

int Picture::height() {
   return _height;
}

Picture::Picture(int h, int w) {
   _width = w;
   _height = h;
   _rowlen = rowlen(_width);
   data = new uint8[_height*_rowlen];
   for (int i=0; i<_height*_rowlen; i++)
      data[i] = 0;
}

Picture::Picture(const Picture& p) {
   _width = p._width;
   _height = p._height;
   _rowlen = rowlen(_width);
   data = new uint8[_height*_rowlen];
   for (int i=0; i<_height*_rowlen; i++)
      data[i] = p.data[i];
}

// destructor followed by copy constructor and return self
Picture& Picture::operator=(const Picture& p) {
   delete[] data;
   _width = p._width;
   _height = p._height;
   _rowlen = rowlen(_width);
   data = new uint8[_height*_rowlen];
   for (int i=0; i<_height*_rowlen; i++)
      data[i] = p.data[i];
   return *this;
}

Picture::~Picture() {
   delete[] data;
}

Color Picture::get(int row, int col) {
   if (row < 0 || row >= _height)
      throw runtime_error("Row out of bounds: " + itoa(row));
   if (col < 0 || col >= _width)
      throw runtime_error("Column out of bounds: " + itoa(col));
   Color result;
   uint8* dataloc = bgr(row, col);
   result.blue = dataloc[0];
   result.green = dataloc[1];
   result.red = dataloc[2];
   return result;
}

void Picture::set(int row, int col, Color new_color) {
   if (row < 0 || row >= _height)
      throw runtime_error("Row out of bounds: " + itoa(row));
   if (col < 0 || col >= _width)
      throw runtime_error("Column out of bounds: " + itoa(col));
   if (new_color.red < 0 || new_color.red > 255)
      throw runtime_error("Red out of bounds: " + itoa(new_color.red));
   if (new_color.green < 0 || new_color.green > 255)
      throw runtime_error("Green out of bounds: " + itoa(new_color.green));
   if (new_color.blue < 0 || new_color.blue > 255)
      throw runtime_error("Blue out of bounds: " + itoa(new_color.blue));
   uint8* dataloc = bgr(row, col);
   dataloc[0] = (uint8) new_color.blue;
   dataloc[1] = (uint8) new_color.green;
   dataloc[2] = (uint8) new_color.red;
}

Picture::Picture(const char filename[]) {
   const char* last4 = filename + strlen(filename) - 4;
   if (strlen(filename) < 4)
      last4 = "----";
   
   if (last4[0]=='.' && toupper(last4[1])=='B' && toupper(last4[2])=='M'
       && toupper(last4[3])=='P') {

      FILE* file = fopen(filename, "rb");
      if (!file)
         throw runtime_error("Cannot open file: " + nice(filename));
      
      char initialbytes[2];
      fread(initialbytes, sizeof(char), 2, file);
      if (initialbytes[0] != 'B' || initialbytes[1] != 'M')
         throw runtime_error("Not a BMP file");
      
      fseek(file, HEADER_DIMENSIONS_OFFSET, SEEK_SET);   
      uint32 dimensions[2];
      fread(dimensions, sizeof(uint32), 2, file);
      _width = dimensions[0];
      _height = dimensions[1];
      _rowlen = rowlen(_width);
      
      fseek(file, BMP_HEADER_SIZE, SEEK_SET);   
      data = new uint8[_height*_rowlen];
      fread(data, sizeof(uint8), _height*_rowlen, file);
      fclose(file);
   }
   else if (last4[0]=='.' && toupper(last4[1])=='J' && toupper(last4[2])=='P'
            && toupper(last4[3])=='G') {
      int planes;
      unsigned char* packed_data = jpgd::decompress_jpeg_image_from_file
         (filename, &_width, &_height, &planes, RGB);
      if (planes != RGB)
         throw runtime_error("Need RGB .jpg: " + nice(filename));
      _rowlen = rowlen(_width);
      data = new uint8[_height*_rowlen];
      memset(data, 0, _height*_rowlen); // fill in between rows
      // move bytes into bmp's weird order
      for (int r=0; r<_height; r++) {
         unsigned char* src = packed_data + _width*RGB*r;
         unsigned char* dest = bgr(r, 0);
         for (int c=0; c<_width; c++) {
            dest[0] = src[2]; // B
            dest[1] = src[1]; // G
            dest[2] = src[0]; // R
            src += RGB;
            dest += RGB;
         }
      }
      free(packed_data);
   }
   else 
      throw runtime_error("Need .bmp or .jpg extension: " + nice(filename));
}

void Picture::save(const char filename[]) {
   int n = strlen(filename);
   if (n<4 || filename[n-4]!='.' || 
      !(filename[n-3]=='B'||filename[n-3]=='b') ||
      !(filename[n-2]=='M'||filename[n-2]=='m') ||
      !(filename[n-1]=='P'||filename[n-1]=='p'))
      throw runtime_error("Filename must end with .bmp: " + nice(filename));

   // write header to a buffer first
   uint8 hdr[54];
   uint8* p = hdr;
   
   // filetype specifier
   p = header_append(p, 1, 'B');
   p = header_append(p, 1, 'M');
   // file size: header + body
   p = header_append(p, 4, BMP_HEADER_SIZE + _height*_rowlen);
   // reserved fields
   p = header_append(p, 2, 0);
   p = header_append(p, 2, 0);
   // header size
   p = header_append(p, 4, BMP_HEADER_SIZE);
   
   // start of DIB subheader
   // subheader size
   p = header_append(p, 4, DIB_SUBHEADER_SIZE);
   // image dimensions
   p = header_append(p, 4, _width);
   p = header_append(p, 4, _height);
   // planes
   p = header_append(p, 2, 1);
   // bits per pixel
   p = header_append(p, 2, 24);
   // compression
   p = header_append(p, 4, 0);
   // pixel data size
   p = header_append(p, 4, _height*_rowlen);
   // x/y pixels per meter
   p = header_append(p, 4, 2400);
   p = header_append(p, 4, 2400);
   // palette information
   p = header_append(p, 4, 0);
   p = header_append(p, 4, 0);

   FILE* file = fopen(filename, "wb");
   // write result bmp file
   if (!file)
      throw runtime_error("Cannot open file: " + nice(filename));
   fwrite(hdr, sizeof(unsigned char), BMP_HEADER_SIZE, file);
   fwrite(data, sizeof(unsigned char), _height*_rowlen, file);
   fclose(file);
}

Picture::uint8* Picture::header_append(uint8* hdr, int bytes, uint32 value) {
   uint8* p = reinterpret_cast<uint8*>(&value);
   for (int i=0; i<bytes; i++)
      hdr[i] = p[i];
   return hdr + bytes;
}

// useful b/c stored row lengths are rounded up to multiples of 4
int Picture::rowlen(int width) {
   int result = RGB*width;
   // round up to a multiple of 4
   if (result%4 != 0)
      result += 4 - (result%4);
   return result;
}

// BMPs are stored in a funny order
Picture::uint8* Picture::bgr(int row, int col) {
   return data + (_height-row-1)*rowlen(_width) + RGB*col;
}

string Picture::itoa(int i) {
   ostringstream oss;
   oss << i;
   return oss.str();
}

string Picture::nice(const char cstring[]) {
   if (cstring != NULL) return cstring; // implicit conversion
   return "NULL";
}

int Picture::shows = 0;
bool Picture::headless = 
#ifdef HEADLESS
   true;
#else
   false;
#endif

void Picture::show(bool new_window) {
   if (headless) {
      save("lastshow.bmp");
      return;
   }
   string filename = "/tmp/picture";
   if (new_window)
      filename += itoa(shows);
   filename += ".bmp";
   
   save(filename.c_str());
   string command = "eog " + filename;
   if (!new_window) command += " --single-window";
   command += " &";
   int result = system(command.c_str());
   
   if (result != 0) throw runtime_error("Picture::show couldn't call eog");

   // wait longer on the first show, OS takes time to start eog
   if (shows == 0) sleep(1);

   // generally, wait 0.2 seconds = 200000 milliseconds
   usleep(200000);
   shows++;
}
