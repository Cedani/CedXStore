#pragma once

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/colour.h>
#include "config.h"

namespace lau {
    class MyButton {
        public:
            MyButton(wxPanel *parent, wxBoxSizer *vBox, const wxString &label, const wxString &imgPath);

            // void setSize();
            // void setDefaultBackgroundColor(wxColour);
            // void setHoverBackgroundColor(wxColour);
            // void setHoverTextColor(wxColour);

        private:
            void init();
            void initButton();
            wxPanel *_parent;
            wxBoxSizer *_vBox;

            //Button
            wxPanel *_panel;
            wxButton *_button;
            wxBoxSizer *_mBox;
            int _width;
            int _height;

            //Label name
            wxString _label;
            wxString _imgPath;
    };
}