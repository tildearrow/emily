#include "toolkit.h"

int eBitmap::pitch() {
  return width*4*sizeof(float);
}

void eBitmap::clear() {
  memset(data,0,width*height*4*sizeof(float));
}

void eBitmap::blitOn(eBitmap* src, int x, int y) {
  int tw, th;
  eColor* dataPix;
  eColor* destin;
  tw=src->width;
  th=src->height;
  for (int j=0; j<th; j++) {
    for (int i=0; i<tw; i++) {
      dataPix=(eColor*)&src->data[(i+j*src->width)<<2];
      destin=(eColor*)&data[(x+i+(y+j)*width)<<2];
      if (dataPix->a==0) continue;
      if (dataPix->a==1) {
        *destin=*dataPix;
      } else {
        destin->r+=(dataPix->r-destin->r)*dataPix->a;
        destin->g+=(dataPix->g-destin->g)*dataPix->a;
        destin->b+=(dataPix->b-destin->b)*dataPix->a;
        destin->a=dataPix->a+(destin->a*(1-dataPix->a));
      }
    }
  }
}

void eBitmap::shadeGlowBack(int radius, int passes) {
  float accum, ksize, rksize;
  int bounded;
  float buffer[256];
  int bufpos;
  ksize=1+radius*2;
  rksize=1/ksize;
  // clear bitmap
  for (int i=0; i<width*height; i++) {
    data[i<<2]=1;
    data[1+(i<<2)]=1;
    data[2+(i<<2)]=1;
  }
  if (radius==0) return;
  for (int h=0; h<passes; h++) {
    // horizontal
    for (int j=0; j<height; j++) {
      accum=0;
      for (int i=0; i<256; i++) {
        buffer[i]=data[3+((j*width)<<2)];
      }
      // prepare accumulator
      for (int i=0; i<ksize; i++) {
        bounded=(i-radius<0)?0:(i-radius);
        accum+=data[3+((j*width+bounded)<<2)];
      }
      bufpos=0;
      for (int i=0; i<width-radius; i++) {
        buffer[bufpos]=data[3+((j*width+i)<<2)];
        data[3+((j*width+i)<<2)]=accum*rksize*0.5;
        accum-=buffer[(bufpos-radius)&255];
        accum+=data[3+((j*width+i+radius)<<2)];
        bufpos++;
        data[3+((j*width+i)<<2)]+=accum*rksize*0.5;
      }
      for (int i=width-radius; i<width; i++) {
        buffer[bufpos]=data[3+((j*width+i)<<2)];
        data[3+((j*width+i)<<2)]=accum*rksize*0.5;
        accum-=buffer[(bufpos-radius)&255];
        accum+=data[3+(((j+1)*width-1)<<2)];
        bufpos++;
        data[3+((j*width+i)<<2)]+=accum*rksize*0.5;
      }
    }
    // vertical
    for (int j=0; j<width; j++) {
      accum=0;
      for (int i=0; i<256; i++) {
        buffer[i]=data[3+(j<<2)];
      }
      // prepare accumulator
      for (int i=0; i<ksize; i++) {
        bounded=(i-radius<0)?0:(i-radius);
        accum+=data[3+((j+bounded*width)<<2)];
      }
      bufpos=0;
      for (int i=0; i<height-radius; i++) {
        buffer[bufpos]=data[3+((j+i*width)<<2)];
        data[3+((j+i*width)<<2)]=accum*rksize*0.5;
        accum-=buffer[(bufpos-radius)&255];
        accum+=data[3+((j+(i+radius)*width)<<2)];
        bufpos++;
        data[3+((j+i*width)<<2)]+=accum*rksize*0.5;
      }
      for (int i=height-radius; i<height; i++) {
        buffer[bufpos]=data[3+((j+i*width)<<2)];
        data[3+((j+i*width)<<2)]=accum*rksize*0.5;
        accum-=buffer[(bufpos-radius)&255];
        accum+=data[3+((width*(height-1)+j)<<2)];
        bufpos++;
        data[3+((j+i*width)<<2)]+=accum*rksize*0.5;
      }
    }
  }
}

void eBitmap::shadeColor(eColor c) {
  for (int i=0; i<width*height; i++) {
    data[i<<2]*=c.r;
    data[1+(i<<2)]*=c.g;
    data[2+(i<<2)]*=c.b;
    data[3+(i<<2)]*=c.a;
  }
}

void eBitmap::shadeVGrad(double p1, double p2, eColor c1, eColor c2) {
  if (p1>p2) {
    return;
  }
  for (int j=0; j<p1*height; j++) {
    for (int i=0; i<width; i++) {
      data[(j*width+i)<<2]*=c1.r;
      data[1+((j*width+i)<<2)]*=c1.g;
      data[2+((j*width+i)<<2)]*=c1.b;
      data[3+((j*width+i)<<2)]*=c1.a;
    }
  }
  for (int j=p1*height; j<p2*height; j++) {
    for (int i=0; i<width; i++) {
      data[(j*width+i)<<2]*=c1.r+(c2.r-c1.r)*(((double)j-(p1*height))/((p2-p1)*height));
      data[1+((j*width+i)<<2)]*=c1.g+(c2.g-c1.g)*(((double)j-(p1*height))/((p2-p1)*height));
      data[2+((j*width+i)<<2)]*=c1.b+(c2.b-c1.b)*(((double)j-(p1*height))/((p2-p1)*height));
      data[3+((j*width+i)<<2)]*=c1.a+(c2.a-c1.a)*(((double)j-(p1*height))/((p2-p1)*height));
    }
  }
  for (int j=p2*height; j<height; j++) {
    for (int i=0; i<width; i++) {
      data[(j*width+i)<<2]*=c2.r;
      data[1+((j*width+i)<<2)]*=c2.g;
      data[2+((j*width+i)<<2)]*=c2.b;
      data[3+((j*width+i)<<2)]*=c2.a;
    }
  }
}

void eBitmap::circle(int x, int y, int r, eColor color) {
  int ffd, ax1, ay1, ax2, ay2;
  float* alphaMap;
  float k;
  ffd=ceil((float)r/1.414213562373095);
  ax1=0;
  if (x<0) {
    ax1=-x;
  }
  ay1=0;
  if (y<0) {
    ay1=-y;
  }
  ax2=r*2;
  if (r*2+x>width) {
    ax2=r*2+x-width;
  }
  ay2=r*2;
  if (r*2+y>height) {
    ay2=r*2+y-height;
  }
  alphaMap=new float[r*r*4];
  for (int i=0; i<r*r*4; i++) {
    alphaMap[i]=1;
  }
  ffd=round((float)r/1.414213562373095);
  for (int j=0; j<r; j++) {
    k=r-sqrt(r*r-(r-j)*(r-j));
    for (int i=0; i<k-1; i++) {
      alphaMap[j*r*2+i]=0;
    }
  }
  for (int i=1; i<ffd+1; i++) {
    k=sqrt(r*r-i*i);
    alphaMap[r*2*(r-(int)k-1)+r-i]=(k-(int)k);
    alphaMap[r*2*(r-i)+r-(int)k-1]=(k-(int)k);
  }
  for (int j=0; j<r; j++) {
    for (int i=0; i<r; i++) {
      alphaMap[r*2*j+r*2-1-i]=alphaMap[r*2*j+i];
      alphaMap[r*2*(r*2-1-j)+i]=alphaMap[r*2*j+i];
      alphaMap[r*2*(r*2-1-j)+r*2-1-i]=alphaMap[r*2*j+i];
    }
  }
  for (int j=ay1; j<ay2; j++) {
    for (int i=ax1; i<ax2; i++) {
      data[((j+y-r)*width+x-r+i)<<2]+=(color.r-data[((j+y-r)*width+x-r+i)<<2])*color.a*alphaMap[r*2*j+i];
      data[1+(((j+y-r)*width+x-r+i)<<2)]+=(color.g-data[1+(((j+y-r)*width+x-r+i)<<2)])*color.a*alphaMap[r*2*j+i];
      data[2+(((j+y-r)*width+x-r+i)<<2)]+=(color.b-data[2+(((j+y-r)*width+x-r+i)<<2)])*color.a*alphaMap[r*2*j+i];
      data[3+(((j+y-r)*width+x-r+i)<<2)]=(color.a*alphaMap[r*2*j+i])+(data[3+(((j+y-r)*width+x-r+i)<<2)]*(1-(color.a*alphaMap[r*2*j+i])));
    }
  }
  delete[] alphaMap;
}

void eBitmap::roundRect(int x, int y, int w, int h, int r, eColor color) {
  int ffd, ax1, ay1, ax2, ay2;
  float* alphaMap;
  float k;
  ffd=ceil((float)r/1.414213562373095);
  ax1=0;
  if (x<0) {
    ax1=-x;
  }
  ay1=0;
  if (y<0) {
    ay1=-y;
  }
  ax2=w;
  if (w+x>width) {
    ax2=w+x-width;
  }
  ay2=h;
  if (h+y>height) {
    ay2=h+y-height;
  }
  alphaMap=new float[w*h];
  for (int i=0; i<w*h; i++) {
    alphaMap[i]=1;
  }
  ffd=round((float)r/1.414213562373095);
  for (int j=0; j<r; j++) {
    k=r-sqrt(r*r-(r-j)*(r-j));
    for (int i=0; i<k-1; i++) {
      alphaMap[j*w+i]=0;
    }
  }
  for (int i=1; i<ffd+1; i++) {
    k=sqrt(r*r-i*i);
    alphaMap[w*(r-(int)k-1)+r-i]=(k-(int)k);
    alphaMap[w*(r-i)+r-(int)k-1]=(k-(int)k);
  }
  for (int j=0; j<r; j++) {
    for (int i=0; i<r; i++) {
      alphaMap[w*j+w-1-i]=alphaMap[w*j+i];
      alphaMap[w*(h-1-j)+i]=alphaMap[w*j+i];
      alphaMap[w*(h-1-j)+w-1-i]=alphaMap[w*j+i];
    }
  }
  for (int j=ay1; j<ay2; j++) {
    for (int i=ax1; i<ax2; i++) {
      data[((j+y)*width+x+i)<<2]+=(color.r-data[((j+y)*width+x+i)<<2])*color.a*alphaMap[w*j+i];
      data[1+(((j+y)*width+x+i)<<2)]+=(color.g-data[1+(((j+y)*width+x+i)<<2)])*color.a*alphaMap[w*j+i];
      data[2+(((j+y)*width+x+i)<<2)]+=(color.b-data[2+(((j+y)*width+x+i)<<2)])*color.a*alphaMap[w*j+i];
      data[3+(((j+y)*width+x+i)<<2)]=(color.a*alphaMap[w*j+i])+(data[3+(((j+y)*width+x+i)<<2)]*(1-(color.a*alphaMap[w*j+i])));
    }
  }
  delete[] alphaMap;
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
    ax2=width; rbi=0;
  }
  ay2=(int)(y+h);
  bbi=(y+h)-(int)(y+h);
  if (ay2>=height) {
    ay2=height; bbi=0;
  }
  // left border
  if (lbi>0) {
    for (int j=ay1; j<ay2; j++) {
      data[(j*width+ax1-1)<<2]+=(color.r-data[(j*width+ax1-1)<<2])*(color.a*lbi);
      data[1+((j*width+ax1-1)<<2)]+=(color.g-data[1+((j*width+ax1-1)<<2)])*(color.a*lbi);
      data[2+((j*width+ax1-1)<<2)]+=(color.b-data[2+((j*width+ax1-1)<<2)])*(color.a*lbi);
      data[3+((j*width+ax1-1)<<2)]=(lbi*color.a)+(data[3+((j*width+ax1-1)<<2)]*(1-(color.a*lbi)));
    }
  }
  // right border
  if (rbi>0) {
    for (int j=ay1; j<ay2; j++) {
      data[(j*width+ax2)<<2]+=(color.r-data[(j*width+ax2)<<2])*(color.a*rbi);
      data[1+((j*width+ax2)<<2)]+=(color.g-data[1+((j*width+ax2)<<2)])*(color.a*rbi);
      data[2+((j*width+ax2)<<2)]+=(color.b-data[2+((j*width+ax2)<<2)])*(color.a*rbi);
      data[3+((j*width+ax2)<<2)]=(rbi*color.a)+(data[3+((j*width+ax2)<<2)]*(1-(color.a*rbi)));
    }
  }
  // top border
  if (tbi>0) {
    for (int i=ax1; i<ax2; i++) {
      data[((ay1-1)*width+i)<<2]+=(color.r-data[((ay1-1)*width+i)<<2])*(color.a*tbi);
      data[1+(((ay1-1)*width+i)<<2)]+=(color.g-data[1+(((ay1-1)*width+i)<<2)])*(color.a*tbi);
      data[2+(((ay1-1)*width+i)<<2)]+=(color.b-data[2+(((ay1-1)*width+i)<<2)])*(color.a*tbi);
      data[3+(((ay1-1)*width+i)<<2)]=(color.a*tbi)+(data[3+(((ay1-1)*width+i)<<2)]*(1-(color.a*tbi)));
    }
  }
  // bottom border
  if (bbi>0) {
    for (int i=ax1; i<ax2; i++) {
      data[(ay2*width+i)<<2]+=(color.r-data[(ay2*width+i)<<2])*(color.a*bbi);
      data[1+((ay2*width+i)<<2)]+=(color.g-data[1+((ay2*width+i)<<2)])*(color.a*bbi);
      data[2+((ay2*width+i)<<2)]+=(color.b-data[2+((ay2*width+i)<<2)])*(color.a*bbi);
      data[3+((ay2*width+i)<<2)]=(color.a*bbi)+(data[3+((ay2*width+i)<<2)]*(1-(color.a*bbi)));
    }
  }
  
  // corners
  // left top
  if (lbi>0 && tbi>0) {
    data[((ay1-1)*width+ax1-1)<<2]+=(color.r-data[((ay1-1)*width+ax1-1)<<2])*(color.a*lbi*tbi);
    data[1+(((ay1-1)*width+ax1-1)<<2)]+=(color.g-data[1+(((ay1-1)*width+ax1-1)<<2)])*(color.a*lbi*tbi);
    data[2+(((ay1-1)*width+ax1-1)<<2)]+=(color.b-data[2+(((ay1-1)*width+ax1-1)<<2)])*(color.a*lbi*tbi);
    data[3+(((ay1-1)*width+ax1-1)<<2)]=(color.a*lbi*tbi)+(data[3+(((ay1-1)*width+ax1-1)<<2)]*(1-(color.a*lbi*tbi)));
  }
  // right top
  if (rbi>0 && tbi>0) {
    data[((ay1-1)*width+ax2)<<2]+=(color.r-data[((ay1-1)*width+ax2)<<2])*(color.a*rbi*tbi);
    data[1+(((ay1-1)*width+ax2)<<2)]+=(color.g-data[1+(((ay1-1)*width+ax2)<<2)])*(color.a*rbi*tbi);
    data[2+(((ay1-1)*width+ax2)<<2)]+=(color.b-data[2+(((ay1-1)*width+ax2)<<2)])*(color.a*rbi*tbi);
    data[3+(((ay1-1)*width+ax2)<<2)]=(color.a*rbi*tbi)+(data[3+(((ay1-1)*width+ax2)<<2)]*(1-(color.a*rbi*tbi)));
  }
  // left bottom
  if (lbi>0 && bbi>0) {
    data[(ay2*width+ax1-1)<<2]+=(color.r-data[(ay2*width+ax1-1)<<2])*(color.a*rbi*tbi);
    data[1+((ay2*width+ax1-1)<<2)]+=(color.g-data[1+((ay2*width+ax1-1)<<2)])*(color.a*rbi*tbi);
    data[2+((ay2*width+ax1-1)<<2)]+=(color.b-data[2+((ay2*width+ax1-1)<<2)])*(color.a*rbi*tbi);
    data[3+((ay2*width+ax1-1)<<2)]=(color.a*rbi*tbi)+(data[3+((ay2*width+ax1-1)<<2)]*(1-(color.a*rbi*tbi)));
  }
  // right bottom
  if (rbi>0 && bbi>0) {
    data[(ay2*width+ax2)<<2]+=(color.r-data[(ay2*width+ax2)<<2])*(color.a*rbi*bbi);
    data[1+((ay2*width+ax2)<<2)]+=(color.g-data[1+((ay2*width+ax2)<<2)])*(color.a*rbi*bbi);
    data[2+((ay2*width+ax2)<<2)]+=(color.b-data[2+((ay2*width+ax2)<<2)])*(color.a*rbi*bbi);
    data[3+((ay2*width+ax2)<<2)]=(color.a*rbi*bbi)+(data[3+((ay2*width+ax2)<<2)]*(1-(color.a*rbi*bbi)));
  }
  
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
