#pragma once
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/colour.h>
#include <wx/textctrl.h>
#include <wx/animate.h>
#include <wx/stattext.h>
#include "myTimer.hpp"
#include "Label.hpp"
#include "Config.h"
#include "myUtils.hpp"
#define MAX_STR_LENGTH 14

namespace lau {
    class Pseudo: public Label {
        public:
            Pseudo(wxPanel *parent, wxBoxSizer *pBox, std::function<void()> func);
            ~Pseudo();

            void initHideElements() final;
            void showLoading(bool);
            void handlePseudoAvailable(wxCommandEvent &);
            void handlePseudoUnavailable(wxCommandEvent &);
            wxString checkRequirements();
        private:

            bool _isAvailable;
            void init();
            void initLoading();
            void initLabelAvailable();
            void bindEvents();

            tmr::myTimer _timer;
            std::function<void()> _timerFunctor;

            // pseudo text and animations
            wxAnimationCtrl *_loading;
            wxStaticText *_available;
            



            // function to check requirements

            // Gif image for loading animation

            // display text stating whether the pseudo is available or not
    };
}