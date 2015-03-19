#include "picture.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
   if (argc < 2) {
      cout << "Usage: instagram infile.bmp [outfile.bmp]" << endl;
      return 1;
   }

   // open the file and construct a Picture object
   Picture pic(argv[1]);
   
   // shift the color components
   for (int row=0; row<pic.height(); row++)
      for (int col=0; col<pic.width(); col++) {
         Color color = pic.get(row, col);
         Color newcolor;
         newcolor.red = color.green;
         newcolor.green = color.blue;
         newcolor.blue = color.red;
         pic.set(row, col, newcolor);
      }
   
   // show the new picture
   pic.show();
   
   // save it, if the optional command-line argument was provided
   if (argc > 2)
      pic.save(argv[2]);
}
