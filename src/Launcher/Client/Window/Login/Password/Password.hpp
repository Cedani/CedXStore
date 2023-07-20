#pragma once
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/colour.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include "myTimer.hpp"
#include "Label.hpp"
#include "Config.h"

namespace lau {
    class Password: public Label {
        public:
            Password(wxPanel *parent, wxBoxSizer *pBox, const wxString &plc = "enter your password");
            ~Password();

            void initHideElements() final;
        private:

            void init();
            void initButton();
            void recreateInput();

            wxButton *_shButton;
            wxStaticText *_requirements;
            wxString _toCorrect;
            bool _show;
            // func to check requirements
    };
}