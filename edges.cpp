#include "picture.h"
#include <iostream>
#include <cmath>
using namespace std;

int main(int argc, char* argv[]) {
   if (argc < 2) {
      cout << "Usage: edges infile.bmp [outfile.bmp]" << endl;
      return 1;
   }

   // open the file and construct a Picture object
   Picture pic(argv[1]);

   // make a copy, initially black
   Picture newpic(pic.height(), pic.width());
   
   // edge-finding
   for (int row=1; row<pic.height(); row++)
      for (int col=1; col<pic.width(); col++) {
         Picture::Color color1 = pic.get(row, col);
         Picture::Color color2 = pic.get(row-1, col-1);
         Picture::Color newcolor;
         newcolor.red = (int) abs(color1.red - color2.red);
         newcolor.blue = (int) abs(color1.blue - color2.blue);
         newcolor.green = (int) abs(color1.green - color2.green);
         newpic.set(row, col, newcolor);
      }
   
   // show the new picture
   newpic.show();
   
   // save it, if the optional command-line argument was provided
   if (argc > 2)
      newpic.save(argv[2]);
}
