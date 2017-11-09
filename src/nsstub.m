#include <Cocoa/Cocoa.h>
#include "nsstub.h"

// this may need to be replaced
double nsStubDPI() {
  CGFloat val;
  val=[mainScreen backingScaleFactor];
  return (double)val;
}
