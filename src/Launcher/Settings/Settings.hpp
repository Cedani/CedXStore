#pragma once
#include <wx/colour.h>

namespace lau {
    using uchar = unsigned char;
    typedef enum {
        ENG,
        FRA,
        DEU,
        ESP,
        ITA
    }LANG;

    typedef enum {
        DARK,
        WHITE
    }CTHEME;

    

    typedef struct SColor {
        SColor(uchar re = 255, uchar gr = 255, uchar bl = 255): r(re), g(gr), b(bl){};
        uchar r;
        uchar g;
        uchar b;
        float a;
    }color;

    typedef struct STheme {
        color texts;
        color buttons;
        color background;     
    }Theme;

    namespace Helper{
        
    }

    class Settings {
        
    };
}