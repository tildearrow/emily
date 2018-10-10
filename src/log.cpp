#include "toolkit.h"

#ifdef _WIN32
// to be fixed if the user had changed the console color
#define RATTR FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE
#endif

int eLogD(const char* format, ...) {
  va_list va;
  int ret;
  if (eEngine::logLevel<EMILY_LOGLEVEL_DEBUG) return 0;
#ifdef _WIN32
  HANDLE h;
  h=GetStdHandle(STD_OUTPUT_HANDLE);
  if (h==NULL) return 0;
  SetConsoleTextAttribute(h,FOREGROUND_BLUE|FOREGROUND_INTENSITY);
  printf("[debug] ");
  SetConsoleTextAttribute(h,RATTR);
#else
  printf("\x1b[1;34m[debug]\x1b[m ");
#endif
  va_start(va,format);
  ret=vprintf(format,va);
  va_end(va); 
  return ret;
}

int eLogI(const char* format, ...) {
  va_list va;
  int ret;
  if (eEngine::logLevel<EMILY_LOGLEVEL_INFO) return 0;
#ifdef _WIN32
  HANDLE h;
  h=GetStdHandle(STD_OUTPUT_HANDLE);
  if (h==NULL) return 0;
  SetConsoleTextAttribute(h,FOREGROUND_GREEN|FOREGROUND_INTENSITY);
  printf("[info] ");
  SetConsoleTextAttribute(h,RATTR);
#else
  printf("\x1b[1;32m[info]\x1b[m ");
#endif
  va_start(va,format);
  ret=vprintf(format,va);
  va_end(va); 
  return ret;
}

int eLogW(const char* format, ...) {
  va_list va;
  int ret;
  if (eEngine::logLevel<EMILY_LOGLEVEL_WARN) return 0;
#ifdef _WIN32
  HANDLE h;
  h=GetStdHandle(STD_OUTPUT_HANDLE);
  if (h==NULL) return 0;
  SetConsoleTextAttribute(h,FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
  printf("[warning] ");
  SetConsoleTextAttribute(h,RATTR);
#else
  printf("\x1b[1;33m[warning]\x1b[m ");
#endif
  va_start(va,format);
  ret=vprintf(format,va);
  va_end(va); 
  return ret;
}

int eLogE(const char* format, ...) {
  va_list va;
  int ret;
  if (eEngine::logLevel<EMILY_LOGLEVEL_ERROR) return 0;
#ifdef _WIN32
  HANDLE h;
  h=GetStdHandle(STD_OUTPUT_HANDLE);
  if (h==NULL) return 0;
  SetConsoleTextAttribute(h,FOREGROUND_RED|FOREGROUND_INTENSITY);
  printf("[ERROR] ");
  SetConsoleTextAttribute(h,RATTR);
#else
  printf("\x1b[1;33m[ERROR]\x1b[m ");
#endif
  va_start(va,format);
  ret=vprintf(format,va);
  va_end(va); 
  return ret;
}
