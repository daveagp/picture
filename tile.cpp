#include "picture.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
   if (argc < 2) {
      cout << "Usage: tile infile.bmp [outfile.bmp]" << endl;
      return 1;
   }

   // open the file and construct a Picture object
   Picture pic(argv[1]);

   // make a copy
   Picture newpic(pic);
   
   // tile the pixels
   for (int row=0; row<pic.height(); row++)
      for (int col=0; col<pic.width(); col++) {
         Color color = pic.get(row*2%pic.height(), col*2%pic.width());
         newpic.set(row, col, color);
      }
   
   // show the new picture
   newpic.show();
   
   // save it, if the optional command-line argument was provided
   if (argc > 2)
      newpic.save(argv[2]);
}
