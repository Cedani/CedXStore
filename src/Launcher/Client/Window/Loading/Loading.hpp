#pragma once

#include <wx/frame.h>
#include <wx/event.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/animate.h>
#include <wx/time.h>
#include "Config.h"
#include "myUtils.hpp"


namespace lau {
    class Loading : public wxFrame {
        public:
            Loading();
            ~Loading();

            void init();
            // void handleConnected();
            void handleNotConnected();
        private:
            wxPanel *_mainPanel;
            wxBoxSizer *_mainBox;

            void OnClose(wxCloseEvent &);
            
            wxAnimationCtrl *_loadingGif;
            wxStaticText *_errorNoConnectionText;

        private:
            void bindEvents();
    };
}