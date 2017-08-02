#include "toolkit.h"

int eWidget::draw() {
  ((eEngine*)engine)->line(x,y,x+w,y+h);
  return 0;
}
