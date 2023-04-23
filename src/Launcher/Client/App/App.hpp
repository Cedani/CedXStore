#pragma once
// #include <wx/wx.h>
#include <wx/app.h>
#include <wx/image.h>
#include "myWindow.hpp"

namespace lau {
    class App : public wxApp {

        public:
            bool OnInit() override;
            void printer();
            Window *_myWindow;
    };
};
wxDECLARE_APP(lau::App);
