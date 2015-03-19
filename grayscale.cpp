#include "picture.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
   if (argc < 2) {
      cout << "Usage: grayscale infile.bmp [outfile.bmp]" << endl;
      return 1;
   }

   // open the file and construct a Picture object
   Picture pic(argv[1]);
   
   // convert the colors to grayscale
   for (int row=0; row<pic.height(); row++)
      for (int col=0; col<pic.width(); col++) {
         Color color = pic.get(row, col);
         double luminance = 0.299*color.red 
            + 0.587*color.green + 0.114*color.blue;
         color.red = (int)luminance;
         color.blue = (int)luminance;
         color.green = (int)luminance;
         pic.set(row, col, color);
      }
   
   // show the new picture
   pic.show();
   
   // save it, if the optional command-line argument was provided
   if (argc > 2)
      pic.save(argv[2]);
}
