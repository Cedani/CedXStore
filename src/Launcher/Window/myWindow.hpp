#pragma once

#include <wx/frame.h>
#include <wx/event.h>
#include <wx/menu.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include <iostream>
#include <memory>
#include "SideMenu.hpp"


namespace lau {

    class Window: public wxFrame {

        public:
            Window();
            ~Window();

        private: //Functions event
            void OnAbout(wxCommandEvent &);
            void OnExit(wxCommandEvent &);
            void OnChange(wxCommandEvent &);

        private: //memeber
            void initMenu();
            void initPanel();


        private:

            // Menus
            wxMenuBar *_menuBar;
            wxMenu *_menuFile;
            wxMenu *_menuHelp;

            //central panel
            wxPanel *_mainPanel;
            wxPanel *_testPanel;

            // SideMenu
            SideMenu *_sideMenu = nullptr;
            wxBoxSizer *_mainBox;
    };
}