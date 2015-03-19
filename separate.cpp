#include "picture.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
   if (argc != 2) {
      cout << "Usage: separate infile.bmp" << endl;
      return 1;
   }

   // open the file and construct a Picture object
   Picture pic(argv[1]);
   
   // make 3 copies
   Picture redpic(pic);
   Picture bluepic(pic);
   Picture greenpic(pic);
   
   // invert the colors
   for (int row=0; row<pic.height(); row++)
      for (int col=0; col<pic.width(); col++) {
         Color color;
         color = pic.get(row, col);
         color.red = 0;
         color.green = 0;
         bluepic.set(row, col, color);

         color = pic.get(row, col);
         color.blue = 0;
         color.green = 0;
         redpic.set(row, col, color);

         color = pic.get(row, col);
         color.red = 0;
         color.blue = 0;
         greenpic.set(row, col, color);
      }
   
   // show the new pictures in 3 windows
   redpic.show(true);
   bluepic.show(true);
   greenpic.show(true);
   
}
