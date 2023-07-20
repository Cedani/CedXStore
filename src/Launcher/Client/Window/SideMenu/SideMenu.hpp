#pragma once

#include <wx/frame.h>
#include <wx/event.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <random>
// #include <wx/button.h>
#include <wx/memory.h>
#include "myButton.hpp"
#include "myUtils.hpp"
#include "Config.h"

namespace lau {
    class SideMenu {

        public:
            SideMenu(wxPanel *parent, wxBoxSizer *vBox);
            ~SideMenu();

        private:
            void initPanel();
            void initButtons();

            // void hoverLibraryButton(wxMouseEvent &evt);
            // void initLibraryButton();


        private:
            wxPanel *_parent;

            // panel and sizer
            wxPanel *_panel;
            wxBoxSizer *_vBox;
            wxBoxSizer *_mBox;
            wxButton *_test;

            // button
            wxPanel *_BorderLibrary;
            MyButton *_Library;
            wxPanel *_BorderStore;
            MyButton *_Store;
            wxPanel *_BorderDownloads;
            MyButton *_Downloads;

            // useless panel
            wxPanel *_useless;
    };
}