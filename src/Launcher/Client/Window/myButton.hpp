#pragma once

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/colour.h>
#include "Config.h"

namespace lau {
    class MyButton {
        public:
            MyButton(wxPanel *parent, wxBoxSizer *vBox, const wxString &label, const wxString &imgPath, bool img = true);

            // void setSize();
            void setDefaultBackgroundColor(wxColour);
            void setHoverBackgroundColor(wxColour);
            void setHoverTextColor(wxColour);
            
            void bindClickFunction(std::function<void(wxCommandEvent &)>);


            void refreshGraphicsEvent();
            
            void setBorder(wxColour);
            // void setBorder(bool);

            void setHoverBorderColor(bool, wxColour);

            void update();

            void setLabel(wxString lab);

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

            wxColour _backgroundColor;
            wxColour _textColor;
            wxColour _hoverBackgroundColor;
            wxColour _hoverTextColor;
            wxColour _hoverBorderColor;
            wxColour _borderColor;
            bool _border;
            bool _borderHover;
            bool _img;
    };
}
