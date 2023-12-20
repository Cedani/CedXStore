#pragma once
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/colour.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include "myUtils.hpp"
#include "myTimer.hpp"
#include "Label.hpp"
#include "Config.h"
// #define MAX_PASSWOR_LENGTH 32

namespace lau {
    class Password: public Label {
        public:
            Password(wxPanel *parent, wxBoxSizer *pBox, const wxString &plc = "enter your password");
            ~Password();

            void initHideElements() final;
            void takeFocus();
            void setFocusEvent(wxEventType);
            wxString checkRequirements();
            wxString checkRequirements(const wxString &);
        private:

            void init();
            void initButton();
            void recreateInput();
            void bindEvent();

            void checkCharacter(wxUniCharRef, bool dec = false);
            void resetCounts(wxString &value);
            int _majCount;
            int _minCount;
            int _numCount;
            int _size;
            int _specChar;
            int _unallowedCharCount;
            int _eventFocus;
            wxString _specialChars;
            wxString _lastValue;
            wxButton *_shButton;
            wxStaticText *_requirements;
            wxString _toCorrect;
            bool _show;
            // func to check requirements
    };
}