#include "picture.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;

int main(int argc, char* argv[]) {
   if (argc < 4) {
      cout << "Usage: scale infile.bmp V-factor H-factor [out.bmp]" << endl;
      cout << "E.g. scale fruit.bmp 1.5 0.7" << endl;
      return 1;
   }
   
   double vscale = atof(argv[2]);
   double hscale = atof(argv[3]);

   // open the file and construct a Picture object
   Picture pic(argv[1]);

   // make a new image, initially black
   Picture newpic((int)(pic.height()*vscale), (int)(pic.width()*hscale));
   
   // scale the image
   for (int row=0; row<newpic.height(); row++)
      for (int col=0; col<newpic.width(); col++) {
         newpic.set(row, col, 
                  pic.get((int)(row/vscale), (int)(col/hscale)));
      }
   
   // show the new picture
   newpic.show();
   
   // save it, if the optional command-line argument was provided
   if (argc > 4)
      newpic.save(argv[4]);
}
