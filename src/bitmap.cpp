#include "toolkit.h"

//#define OP_PROF

#ifdef OP_PROF

#define OP_BEGIN \
  int opBegin, opEnd; \
  opBegin=perfCount();

#define OP_END(name) \
  opEnd=perfCount(); \
  eLogD("%p "\
        name\
        ": %dns\n",this,opEnd-opBegin);
  
#else
  
#define OP_BEGIN ;
#define OP_END(name) ;
  
#endif

int eBitmap::pitch() {
  return width*4*sizeof(float);
}

void eBitmap::clear() { OP_BEGIN
  memset(data,0,width*height*4*sizeof(float));
  OP_END("clear")
}

void eBitmap::clearToColor(eColor c) { OP_BEGIN
  for (int i=0; i<width*height; i++) {
    data[(i<<2)]=c.r;
    data[1+(i<<2)]=c.g;
    data[2+(i<<2)]=c.b;
    data[3+(i<<2)]=c.a;
  }
  OP_END("clearToColor")
}

void eBitmap::copyBlitOn(eBitmap* src, int x, int y) { OP_BEGIN
  int tw, th;
  if (src!=NULL) {
    tw=src->width;
    th=src->height;
    for (int j=0; j<th; j++) {
      memcpy(&data[(x+(y+j)*width)<<2],&src->data[(j*src->width)<<2],tw*4*sizeof(float));
    }
  }
  OP_END("copyBlitOn")
}

void eBitmap::blitOn(eBitmap* src, int x, int y) { OP_BEGIN
  int tw, th;
  eColor* dataPix;
  eColor* destin;
  if (src!=NULL) {
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
  OP_END("blitOn")
}

void eBitmap::shadeGlowBack(int radius, int passes, eColor col) { OP_BEGIN
  float accum, ksize, rksize;
  float buffer[256];
  int bufpos;
  ksize=1+radius*2;
  rksize=1/ksize;
  // clear bitmap
  for (int i=0; i<width*height; i++) {
    data[i<<2]=col.r;
    data[1+(i<<2)]=col.g;
    data[2+(i<<2)]=col.b;
  }
  if (radius==0) return;
  for (int h=0; h<passes; h++) {
    // horizontal
    for (int j=0; j<height; j++) {
      accum=0;
      memset(buffer,0,256*sizeof(float));
      // prepare accumulator
      for (int i=0; i<radius+1; i++) {
        accum+=data[3+((j*width+i)<<2)];
      }
      bufpos=0;
      for (int i=0; i<width-radius; i++) {
        buffer[(unsigned char)bufpos]=data[3+((j*width+i)<<2)];
        data[3+((j*width+i)<<2)]=accum*rksize*0.5;
        accum-=buffer[(unsigned char)(bufpos-radius)];
        accum+=data[3+((j*width+i+radius)<<2)];
        bufpos++;
        data[3+((j*width+i)<<2)]+=accum*rksize*0.5;
      }
      for (int i=width-radius; i<width; i++) {
        buffer[(unsigned char)bufpos]=data[3+((j*width+i)<<2)];
        data[3+((j*width+i)<<2)]=accum*rksize*0.5;
        accum-=buffer[(unsigned char)(bufpos-radius)];
        bufpos++;
        data[3+((j*width+i)<<2)]+=accum*rksize*0.5;
      }
    }
    // vertical
    for (int j=0; j<width; j++) {
      accum=0;
      memset(buffer,0,256*sizeof(float));
      // prepare accumulator
      for (int i=0; i<radius+1; i++) {
        accum+=data[3+((j+(i)*width)<<2)];
      }
      bufpos=0;
      for (int i=0; i<height-radius; i++) {
        buffer[(unsigned char)bufpos]=data[3+((j+i*width)<<2)];
        data[3+((j+i*width)<<2)]=accum*rksize*0.5;
        accum-=buffer[(unsigned char)(bufpos-radius)];
        accum+=data[3+((j+(i+radius)*width)<<2)];
        bufpos++;
        data[3+((j+i*width)<<2)]+=accum*rksize*0.5;
      }
      for (int i=height-radius; i<height; i++) {
        buffer[(unsigned char)bufpos]=data[3+((j+i*width)<<2)];
        data[3+((j+i*width)<<2)]=accum*rksize*0.5;
        accum-=buffer[(unsigned char)(bufpos-radius)];
        bufpos++;
        data[3+((j+i*width)<<2)]+=accum*rksize*0.5;
      }
    }
  }
  shadeAlpha(1+((float)passes/(float)radius));
  if (col.a!=1) {
    shadeAlpha(col.a);
  }
  OP_END("shadeGlowBack")
}

void eBitmap::shadeColor(eColor c) { OP_BEGIN
  if (c.r==0 && c.g==0 && c.b==0 && c.a==0) {
    clear();
    OP_END("shadeColor")
    return;
  }
  if (c.r==1 && c.g==1 && c.b==1 && c.a==1) {
    OP_END("shadeColor")
    return;
  }
  for (int i=0; i<width*height; i++) {
    data[i<<2]*=c.r;
    data[1+(i<<2)]*=c.g;
    data[2+(i<<2)]*=c.b;
    data[3+(i<<2)]*=c.a;
  }
  OP_END("shadeColor")
}

void eBitmap::shadeAlpha(float a) { OP_BEGIN
  if (a==1) {
    OP_END("shadeAlpha");
    return;
  }
  for (int i=0; i<width*height; i++) {
    data[3+(i<<2)]*=a;
  }
  OP_END("shadeAlpha");
}

void eBitmap::shadePower(float val) { OP_BEGIN
  if (val==0) {
    clearToColor({1,1,1,1});
    OP_END("shadePower");
    return;
  }
  if (val==1) {
    OP_END("shadePower");
    return;
  }
  for (int i=0; i<width*height; i++) {
    data[i<<2]=pow(data[i<<2],val);
    data[1+(i<<2)]=pow(data[1+(i<<2)],val);
    data[2+(i<<2)]=pow(data[2+(i<<2)],val);
    data[3+(i<<2)]=pow(data[3+(i<<2)],val);
  }
  OP_END("shadePower")
}

void eBitmap::shadeHMGrad(eColor c1, eColor c2) { OP_BEGIN
  double gpos;
  eColor toUse;
  for (int i=0; i<width/2; i++) {
    gpos=sin(M_PI*0.5*((double)i*2/width));
    toUse.r=c1.r+(c2.r-c1.r)*gpos;
    toUse.g=c1.g+(c2.g-c1.g)*gpos;
    toUse.b=c1.b+(c2.b-c1.b)*gpos;
    toUse.a=c1.a+(c2.a-c1.a)*gpos;
    for (int j=0; j<height; j++) {
      data[(j*width+i)<<2]*=toUse.r;
      data[1+((j*width+i)<<2)]*=toUse.g;
      data[2+((j*width+i)<<2)]*=toUse.b;
      data[3+((j*width+i)<<2)]*=toUse.a;
      
      data[(j*width+(width-i-1))<<2]*=toUse.r;
      data[1+((j*width+(width-i-1))<<2)]*=toUse.g;
      data[2+((j*width+(width-i-1))<<2)]*=toUse.b;
      data[3+((j*width+(width-i-1))<<2)]*=toUse.a;
    }
  }
  OP_END("shadeHMGrad")
}

void eBitmap::shadeVMGrad(eColor c1, eColor c2) { OP_BEGIN
  double gpos;
  eColor toUse;
  for (int i=0; i<height/2; i++) {
    gpos=sin(M_PI*0.5*((double)i*2/height));
    toUse.r=c1.r+(c2.r-c1.r)*gpos;
    toUse.g=c1.g+(c2.g-c1.g)*gpos;
    toUse.b=c1.b+(c2.b-c1.b)*gpos;
    toUse.a=c1.a+(c2.a-c1.a)*gpos;
    for (int j=0; j<width; j++) {
      data[(i*width+j)<<2]*=toUse.r;
      data[1+((i*width+j)<<2)]*=toUse.g;
      data[2+((i*width+j)<<2)]*=toUse.b;
      data[3+((i*width+j)<<2)]*=toUse.a;
      
      data[(j+width*(height-i-1))<<2]*=toUse.r;
      data[1+((j+width*(height-i-1))<<2)]*=toUse.g;
      data[2+((j+width*(height-i-1))<<2)]*=toUse.b;
      data[3+((j+width*(height-i-1))<<2)]*=toUse.a;
    }
  }
  OP_END("shadeVMGrad")
}

void eBitmap::shadeVGrad(double p1, double p2, eColor c1, eColor c2) { OP_BEGIN
  double gpos;
  eColor delta;
  eColor toUse;
  delta.r=c2.r-c1.r;
  delta.g=c2.g-c1.g;
  delta.b=c2.b-c1.b;
  delta.a=c2.a-c1.a;
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
    gpos=(((double)j-(p1*height))/((p2-p1)*height));
    toUse.r=c1.r+delta.r*gpos;
    toUse.g=c1.g+delta.g*gpos;
    toUse.b=c1.b+delta.b*gpos;
    toUse.a=c1.a+delta.a*gpos;
    for (int i=0; i<width; i++) {
      data[(j*width+i)<<2]*=toUse.r;
      data[1+((j*width+i)<<2)]*=toUse.g;
      data[2+((j*width+i)<<2)]*=toUse.b;
      data[3+((j*width+i)<<2)]*=toUse.a;
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
  OP_END("shadeVGrad")
}

void eBitmap::shadeHGrad(double p1, double p2, eColor c1, eColor c2) { OP_BEGIN
  double gpos;
  eColor delta;
  eColor toUse;
  delta.r=c2.r-c1.r;
  delta.g=c2.g-c1.g;
  delta.b=c2.b-c1.b;
  delta.a=c2.a-c1.a;
  if (p1>p2) {
    return;
  }
  for (int j=0; j<p1*width; j++) {
    for (int i=0; i<height; i++) {
      data[(i*width+j)<<2]*=c1.r;
      data[1+((i*width+j)<<2)]*=c1.g;
      data[2+((i*width+j)<<2)]*=c1.b;
      data[3+((i*width+j)<<2)]*=c1.a;
    }
  }
  for (int j=p1*width; j<p2*width; j++) {
    gpos=(((double)j-(p1*width))/((p2-p1)*width));
    toUse.r=c1.r+delta.r*gpos;
    toUse.g=c1.g+delta.g*gpos;
    toUse.b=c1.b+delta.b*gpos;
    toUse.a=c1.a+delta.a*gpos;
    for (int i=0; i<height; i++) {
      data[(j+i*width)<<2]*=toUse.r;
      data[1+((j+i*width)<<2)]*=toUse.g;
      data[2+((j+i*width)<<2)]*=toUse.b;
      data[3+((j+i*width)<<2)]*=toUse.a;
    }
  }
  for (int j=p2*width; j<width; j++) {
    for (int i=0; i<height; i++) {
      data[(j+i*width)<<2]*=c2.r;
      data[1+((j+i*width)<<2)]*=c2.g;
      data[2+((j+i*width)<<2)]*=c2.b;
      data[3+((j+i*width)<<2)]*=c2.a;
    }
  }
  OP_END("shadeHGrad")
}

void eBitmap::circle(int x, int y, int r, eColor color) { OP_BEGIN
  int ax1, ay1, ax2, ay2;
  float* alphaMap;
  if (r<=0) return;
  //ffd=ceil((float)r/1.414213562373095);
  ax1=0;
  if (x<0) {
    ax1=-x;
  }
  ay1=0;
  if (y<0) {
    ay1=-y;
  }
  ax2=r*2;
  if (r+x>width) {
    ax2=r+x-width;
  }
  ay2=r*2;
  if (r+y>height) {
    ay2=r+y-height;
  }
  alphaMap=new float[r*r*4];
  for (int i=0; i<r*r*4; i++) {
    alphaMap[i]=0;
  }
  for (int j=0; j<r*2*16; j++) {
    for (int i=0; i<r*2*16; i++) {
      alphaMap[(j>>4)*r*2+(i>>4)]+=(((i-r*16)*(i-r*16)+(j-r*16)*(j-r*16))<r*r*256)?(1.0/256.0):(0);
    }
  }
  /*
  ffd=round((float)r/1.414213562373095);
  for (int j=0; j<r; j++) {
    k=r-sqrt(r*r-(r-j)*(r-j));
    for (int i=0; i<k-1; i++) {
      alphaMap[j*r*2+i]=0;
    }
  }
  for (int i=1; i<ffd+1; i++) {
    k=sqrt(r*r-i*i);
    alphaMap[r*2*(r-(int)k-1)+r-i]=k-(int)k;
    alphaMap[r*2*(r-i)+r-(int)k-1]=k-(int)k;
  }
  for (int j=0; j<r; j++) {
    for (int i=0; i<r; i++) {
      alphaMap[r*2*j+r*2-1-i]=alphaMap[r*2*j+i];
      alphaMap[r*2*(r*2-1-j)+i]=alphaMap[r*2*j+i];
      alphaMap[r*2*(r*2-1-j)+r*2-1-i]=alphaMap[r*2*j+i];
    }
  }
  */
  for (int j=ay1; j<ay2; j++) {
    for (int i=ax1; i<ax2; i++) {
      data[((j+y-r)*width+x-r+i)<<2]+=(color.r-data[((j+y-r)*width+x-r+i)<<2])*color.a*alphaMap[r*2*j+i];
      data[1+(((j+y-r)*width+x-r+i)<<2)]+=(color.g-data[1+(((j+y-r)*width+x-r+i)<<2)])*color.a*alphaMap[r*2*j+i];
      data[2+(((j+y-r)*width+x-r+i)<<2)]+=(color.b-data[2+(((j+y-r)*width+x-r+i)<<2)])*color.a*alphaMap[r*2*j+i];
      data[3+(((j+y-r)*width+x-r+i)<<2)]=(color.a*alphaMap[r*2*j+i])+(data[3+(((j+y-r)*width+x-r+i)<<2)]*(1-(color.a*alphaMap[r*2*j+i])));
    }
  }
  delete[] alphaMap;
  OP_END("circle")
}

void eBitmap::roundRect(int x, int y, int w, int h, int r, eColor color) { OP_BEGIN
  int ffd, ax1, ay1, ax2, ay2;
  int off;
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
  if (color.a==1) {
    for (int j=ay1; j<ay2; j++) {
      for (int i=ax1; i<ax2; i++) {
        off=((j+y)*width+x+i);
        if (alphaMap[w*j+i]==1) {
          data[off<<2]=color.r;
          data[1+(off<<2)]=color.g;
          data[2+(off<<2)]=color.b;
          data[3+(off<<2)]=1;
          continue;
        }
        data[off<<2]+=(color.r-data[off<<2])*alphaMap[w*j+i];
        data[1+(off<<2)]+=(color.g-data[1+(off<<2)])*alphaMap[w*j+i];
        data[2+(off<<2)]+=(color.b-data[2+(off<<2)])*alphaMap[w*j+i];
        data[3+(off<<2)]=(alphaMap[w*j+i])+(data[3+(off<<2)]*(1-(alphaMap[w*j+i])));
      }
    }
  } else {
    for (int j=ay1; j<ay2; j++) {
      for (int i=ax1; i<ax2; i++) {
        off=((j+y)*width+x+i);
        data[off<<2]+=(color.r-data[off<<2])*color.a*alphaMap[w*j+i];
        data[1+(off<<2)]+=(color.g-data[1+(off<<2)])*color.a*alphaMap[w*j+i];
        data[2+(off<<2)]+=(color.b-data[2+(off<<2)])*color.a*alphaMap[w*j+i];
        data[3+(off<<2)]=(color.a*alphaMap[w*j+i])+(data[3+(off<<2)]*(1-(color.a*alphaMap[w*j+i])));
      }
    }
  }
  delete[] alphaMap;
  OP_END("roundRect")
}

void eBitmap::rect(double x, double y, double w, double h, eColor color) { OP_BEGIN
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
  OP_END("rect")
}

sf::Texture* eBitmap::toTexture() {
  sf::Texture* temp;
  unsigned char* converted;
  converted=new unsigned char[width*height*4];
  for (int i=0; i<width*height*4; i++) {
    converted[i]=(data[i]>1)?255:(data[i]*255);
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
