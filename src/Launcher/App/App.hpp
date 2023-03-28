#pragma once
#include <wx/wx.h>
#include "myWindow.hpp"

namespace lau {
    class App : public wxApp {

        public:
            bool OnInit() override;
            Window *_myWindow;
    };
};