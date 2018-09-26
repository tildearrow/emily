#include "toolkit.h"

string strFormat(const char* format, ...) {
  va_list va;
  char* str;
  string ret;
  va_start(va,format);
  if (vasprintf(&str,format,va)<0) {
    va_end(va);
    return string("");
  }
  va_end(va);
  ret=str;
  delete[] str;
  return ret;
}
