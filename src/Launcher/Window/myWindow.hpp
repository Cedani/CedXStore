#pragma once
// #include <gtkmm/window.h>
#include <wx/wx.h>

namespace lau {

    class Window: public wxFrame {
        enum {
            ID_HELLO = 1
        };

        public:
            Window();

        private: //Functions
            void OnHello(wxCommandEvent &);
            void OnAbout(wxCommandEvent &);
            void OnExit(wxCommandEvent &);


        private:
            wxMenuBar *_menuBar;
            wxMenu *_menuFile;
            wxMenu *_menuHelp;
    };
}