#include "toolkit.h"

int eBitmap::pitch() {
  return width*(depth/8);
}

int eBitmap::blitFromFTBitmap(FT_Bitmap* data, int x, int y) {
  
}

eBitmap::eBitmap(int inWidth, int inHeight, int inDepth) {
  data=new unsigned char[inWidth*inHeight*(inDepth/8)];
  width=inWidth;
  height=inHeight;
  depth=inDepth;
};

eBitmap::~eBitmap() {
  delete[] data;
}
