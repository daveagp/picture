#include "picture.h"
#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char* argv[]) {
   if (argc < 2) {
      cout << "Usage: glass infile.bmp [outfile.bmp]" << endl;
      return 1;
   }

   // open the file and construct a Picture object
   Picture pic(argv[1]);
   
   // make a copy
   Picture newpic(pic);
   
   // glass effect
   for (int row=0; row<pic.height(); row++) {
      for (int col=0; col<pic.width(); col++) {
         int srcrow, srccol;
         do {
            srcrow = row + rand()%5-2;
         }
         while (srcrow < 0 || srcrow >= pic.height());
         
         do {
            srccol = col + rand()%5-2;
         }
         while (srccol < 0 || srccol >= pic.width());
         newpic.set(row, col, pic.get(srcrow, srccol));
      }
   }      
   
   // show the new picture
   newpic.show();
   
   // save it, if the optional command-line argument was provided
   if (argc > 2)
      newpic.save(argv[2]);
}
