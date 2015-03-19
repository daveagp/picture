#include "picture.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
   if (argc < 2) {
      cout << "Usage: invert infile.bmp [outfile.bmp]" << endl;
      return 1;
   }

   // open the file and construct a Picture object
   Picture pic(argv[1]);
   
   // invert the colors
   for (int row=0; row<pic.height(); row++)
      for (int col=0; col<pic.width(); col++) {
         Color color = pic.get(row, col);
         color.red = 255 - color.red;
         color.blue = 255 - color.blue;
         color.green = 255 - color.green;
         pic.set(row, col, color);
      }
   
   // show the new picture
   pic.show();
   
   // save it, if the optional command-line argument was provided
   if (argc > 2)
      pic.save(argv[2]);
}
