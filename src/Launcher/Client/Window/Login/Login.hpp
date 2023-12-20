#pragma once

#include <wx/frame.h>
#include <wx/event.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/hyperlink.h>
#include <wx/display.h>
#include <wx/time.h>
#include <wx/scrolwin.h>
#include "Pseudo.hpp"
#include "Password.hpp"
#include "myButton.hpp"
#include "myUtils.hpp"

namespace lau {
    class Login: public wxFrame {
        
        public:
            Login();
            ~Login();

            void init();

        private:
            void OnExit(wxCommandEvent &);
            void OnClose(wxCloseEvent &);
    
            void bindEvents();
            void initErrorField();
            void handleAlreadySignupLogin(wxHyperlinkEvent &);
            void clickLogin();
            void clickSignup();


        private:
            wxPanel *_contPanel;
            wxBoxSizer *_contBox;
            // wxBoxSizer *_frameSizer;
            // // loading panel
            // wxPanel *_loadingPanel;
            // wxAnimationCtrl *_loading;
            // wxBoxSizer *_boxLoading;

            // scroller
            // wxScrolledWindow *_scrWin;

            //Form
            wxScrolledWindow *_mainPanel;
            wxBoxSizer *_mainBox;


            //Error Field
            wxStaticText *_errorField;
            wxPanel *_errorPanel;
            wxBoxSizer *_errorSizer;

            // Login
            wxStaticText *_loginText;
            Pseudo *_pseudo = nullptr;
            Password *_password = nullptr;

            // Signup
            Password *_confirmPassword = nullptr;

            // text to change to signup
            wxGridSizer *_grid;
            wxStaticText *_alrd;
            wxHyperlinkCtrl *_sigLogLink;

            // wxTextCtrl *_password;]
            MyButton *_confirm;

            bool _login;
    };
}