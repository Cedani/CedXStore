#pragma once
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/colour.h>
#include <wx/textctrl.h>
#include "myTimer.hpp"



namespace lau {
    class Label {
        public:
            Label(wxPanel *parent, wxBoxSizer *pBox, const wxString &pla);
            ~Label();

            // void setEnterEvent(std::function<void(wxCommandEvent &)>);
            void setSizerBorder(int flags, int borderSize);
            void setMaxSize(int w, int h);
            void setPanelSizerBorder(int flags, int borderSize);
            void Show(bool);
            wxString getValue();

            // hide elements that should be hidden at the beginning
            virtual void initHideElements() = 0;

        protected:
            // void init(); 

            //parent panel and boxsizer
            wxPanel *_parent;
            wxBoxSizer *_pBox;

            //label elements
            wxPanel *_mPanel;
            wxBoxSizer *_mBox;

            // wxGridSizer *_mGrid;
            // wxStaticText *_label;
            wxString _placeholder;
            wxString _value;
            wxTextCtrl *_input;
            bool _pw;
            bool _show;
            // tmr::myTimer _timer;
            // std::function<void()> _timerFunctor;

            void update();
    };
}