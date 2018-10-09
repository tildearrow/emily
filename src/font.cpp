#include "toolkit.h"
#ifdef __APPLE__
extern "C" {
#include "nsstub.h"
}
#endif
#include <string.h>

// font names. sorted from best to worst
const char* fontNames[]={
  // try with sans-serif
  "sans-serif",
  // GOT YOU, DEJAVU USER!
  // this time I am not obeying your orders!
  "IBM Plex Sans",
  "Ubuntu",
  "Helvetica",
  "FreeSans",
  // to be honest I didn't want to Android-ize
  "Roboto",
  // wait, you really don't have Roboto? cool.
  "Oxygen Sans",
  "Oxygen-Sans",
  "Cantarell",
  "Noto Sans",
  "Droid Sans",
  "Linux Biolinum",
  "Liberation Sans",
  "Arimo",
  // *sigh* at least you may have windows fonts, huh?
  "Segoe UI",
  "Calibri",
  "Arial",
  // oh come on at least Tahoma but NOT DejaVu, ever
  "Tahoma",
  "Verdana",
  // even comic sans is better
  "Comic Sans",
  "Comic Sans MS",
  "Comic Neue",
  // ARGH I GIVE UP!
  // but I do wanna rant about this font!
  // DejaVu is the worst font in history! it is insanely obese,
  // when compared to the entire rest of fonts in other OS'es.
  // I'm especially sad how Purism is using it for their Librem
  // phone, since it is a waste of scarce horizontal space!!!
  // please do *NOT* use DejaVu in your system or next project.
  // otherwise it will look dated as hell and ugly.
  //
  // I tried to not swear...
  "DejaVu Sans",
  NULL
};

const char* winFontNames[]={
  "Segoe UI",
  "MS Shell Dlg",
  "Tahoma",
  // at least this one but we want to show text
  "System"
};

const char* macFonts[]={
  "/System/tobedone.",
  "/System/Library/Fonts/Helvetica.dfont"
};

int eFont::loaddef(int variant) {
  string path;
#if defined(HAVE_FONTCONFIG)
  FcPattern* fmpattern;
  FcPattern* match;
  FcResult fmresult;
  FcValue fontName;
  FcValue regular;
  FcValue resval;
#endif
  switch (variant) {
    case eFontDefault:
    case eFontLarge:
#if defined(_WIN32)
      // windows font code here
      path="C:\\Windows\\Fonts\\segoeui.ttf";
#elif defined(__APPLE__)
      // apple font code here
      // San Francisco to be done
      path="/System/Library/Fonts/Helvetica.dfont";
#elif defined(__unix__)
      // fontconfig font code here
#if defined(HAVE_FONTCONFIG)
      fontName.type=FcTypeString;
      regular.type=FcTypeString;
      regular.u.s=(const FcChar8*)"Regular";
      if (!FcInit()) {
        eLogE("Error while initializing fontconfig.\n");
      }
      
      for (int i=0; fontNames[i]!=NULL; i++) {
        fontName.u.s=(const FcChar8*)fontNames[i];
        fmpattern=FcPatternCreate();
        FcPatternAdd(fmpattern,FC_FAMILY,fontName,FcTrue);
        FcPatternAdd(fmpattern,FC_STYLE,regular,FcTrue);
        FcConfigSubstitute(NULL,fmpattern,FcMatchPattern);
        match=FcFontMatch(NULL,fmpattern,&fmresult);
        if (match==NULL) continue;
        FcPatternGet(match,FC_FILE,0,&resval);
        path=(const char*)resval.u.s;
	if (getenv("DEJAVU_OBESITY")!=NULL) {
          if (atoi(getenv("DEJAVU_OBESITY"))>9000) {
            printf("********WARNING!********\n"
"DejaVu is the worst font in history! it is insanely obese,\n"
"when compared to the entire rest of fonts in other OS'es,\n"
"and makes things look ugly and dated.\n"
"if a widget's label looks out of place, it is NOT my fault!\n");
            FcPatternDestroy(fmpattern);
            FcPatternDestroy(match);
            break;
          }
	}
        if (strstr(path.c_str(),"DejaVu")!=NULL && fontNames[i+1]!=NULL) {
          printf("caught you usin' DejaVu!\n");
          FcPatternDestroy(fmpattern);
          FcPatternDestroy(match);
          continue;
        }
        FcPatternDestroy(fmpattern);
        FcPatternDestroy(match);
        break;
      }
#else
      eLogE("Wow! Really? No fontconfig library!\n");
#endif
      break;
#else
      // assume /font.ttf
      path="/font.ttf";
#endif
      break;
    default:
      return 0;
  }
  if (!inst.loadFromFile(path)) {
    return 0;
  }
  return 1;
}

eFont::eFont(eEngine* e): engine(e) {
  inst.setDPI(engine->scale*96);
}
