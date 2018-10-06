#include "toolkit.h"

string strFormat(const char* format, ...) {
  va_list va;
  char* str;
  string ret;
  va_start(va,format);
#ifdef _WIN32
  str=new char[32768];
  if (vsnprintf(str,32767,format,va)<0) {
    va_end(va);
    return string("");
  }
#else
  if (vasprintf(&str,format,va)<0) {
    va_end(va);
    return string("");
  }
#endif
  va_end(va);
  ret=str;
  delete[] str;
  return ret;
}
