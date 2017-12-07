#include "toolkit.h"

string strFormat(const char* format, ...) {
  va_list va;
  char* str;
  string ret;
  va_start(va,format);
  vasprintf(&str,format,va);
  va_end(va);
  ret=str;
  delete[] str;
  return ret;
}
