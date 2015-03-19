/*
 Color separation exercise.
 Compile: make colsep
 Run: ./colsep fruit.bmp
*/

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

   int height = pic.height();
   int width = pic.width();
   
   // create 3 more Pictures
   // FILL in the constructor arguments
   Picture redpic(____);
   Picture bluepic(____);
   Picture greenpic(____);
   
   // separate the colors
   for (int row = 0; row < height; row++) {
      for (int col = 0; col < width; col++) {
         Color color = pic.get(row, col);

         int r = color.red;
         int g = ____;
         int b = ____;

         redpic.set(row, col, Color(r, 0, 0));
         greenpic._____;
         bluepic._____;
      }
   }
   
   // show the new pictures in 3 windows
   redpic.show(true);
   bluepic.show(true);
   greenpic.show(true);
   
}
