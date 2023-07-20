#pragma once

#include <wx/frame.h>
#include <wx/event.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/hyperlink.h>
// #include "myButton.hpp"
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
            void showConfirm(bool);
            wxString getPseudo();

        private:
            void OnExit(wxCommandEvent &);
            void OnClose(wxCloseEvent &);
            // void OnChange(wxCommandEvent &);
    
            void sendCustomEvent(wxEventType);
            void handleAlreadySignupLogin(wxHyperlinkEvent &);
            // void showLoginOrSignup();


        private:
            // Menus


            //Form
            wxPanel *_mainPanel;
            wxBoxSizer *_mainBox;
            // wxTextCtrl *_username;

            //Error Field
            wxStaticText *_errorField;

            // Login
            wxStaticText *_loginText;
            Label *_pseudo;
            Label *_password;

            // Signup
            Label *_confirmPassword;

            // text to change to signup
            wxGridSizer *_grid;
            wxStaticText *_alrd;
            wxHyperlinkCtrl *_sigLogLink;

            // wxTextCtrl *_password;]
            MyButton *_confirm;

            bool _login;
    };
}