#include "toolkit.h"

int eBitmap::pitch() {
  return width*4*sizeof(float);
}

void eBitmap::clear() {
  memset(data,0,width*height*4*sizeof(float));
}

void eBitmap::rect(double x, double y, double w, double h, eColor color) {
  int ax1, ay1, ax2, ay2;
  float lbi, rbi, tbi, bbi;
  if (color.a==0) return;
  ax1=ceil(x);
  if (x==(int)x) {
    lbi=0;
  } else {
    lbi=1.0f-(x-(int)x);
  }
  if (ax1<0) {
    ax1=0; lbi=0;
  }
  ay1=ceil(y);
  if (y==(int)y) {
    tbi=0;
  } else {
    tbi=1.0f-(y-(int)y);
  }
  if (ay1<0) {
    ay1=0; tbi=0;
  }
  ax2=(int)(x+w);
  rbi=(x+w)-(int)(x+w);
  if (ax2>=width) {
    ax2=width-1; rbi=0;
  }
  ay2=(int)(y+h);
  bbi=(y+h)-(int)(y+h);
  if (ay2>=height) {
    ay2=height-1; bbi=0;
  }
  // smoothing to be completed
  //printf("%f %f %f %f\n",lbi,rbi,tbi,bbi);
  if (color.a>=1) {
    for (int j=ay1; j<ay2; j++) {
      for (int i=ax1; i<ax2; i++) {
        data[(j*width+i)<<2]=color.r;
        data[1+((j*width+i)<<2)]=color.g;
        data[2+((j*width+i)<<2)]=color.b;
        data[3+((j*width+i)<<2)]=color.a;
      }
    }
  } else {
    // blend
    for (int j=ay1; j<ay2; j++) {
      for (int i=ax1; i<ax2; i++) {
        data[(j*width+i)<<2]+=(color.r-data[(j*width+i)<<2])*color.a;
        data[1+((j*width+i)<<2)]+=(color.g-data[1+((j*width+i)<<2)])*color.a;
        data[2+((j*width+i)<<2)]+=(color.b-data[2+((j*width+i)<<2)])*color.a;
        data[3+((j*width+i)<<2)]=color.a+(data[3+((j*width+i)<<2)]*(1-color.a));
      }
    }
  }
}

sf::Texture* eBitmap::toTexture() {
  sf::Texture* temp;
  unsigned char* converted;
  converted=new unsigned char[width*height*4];
  for (int i=0; i<width*height*4; i++) {
    converted[i]=data[i]*255;
  }
  temp=new sf::Texture;
  temp->create(width,height);
  temp->update(converted);
  delete[] converted;
  return temp;
}

eBitmap::eBitmap(int inWidth, int inHeight) {
  data=new float[inWidth*inHeight*4];
  width=inWidth;
  height=inHeight;
};

eBitmap::~eBitmap() {
  delete[] data;
}
