#pragma once
// #include <wx/wx.h>
#include <wx/app.h>
#include <wx/image.h>
#include "myWindow.hpp"
#include "Login.hpp"
#include "ThreadPool.hpp"
#include "Manager.hpp"

namespace lau {
    class App : public wxApp {
        public:
            bool OnInit() override;
            void printer();
            void exitLogin();
            Window *_myWindow;
            Login *_login;

            void callHandlePseudo(const char *);


        private:
            thp::ThreadPool _threadpool = thp::ThreadPool(5);
            Manager _sender;
            
            //handler
            void handlePseudoAvailability(const char *);
    };
};

wxDECLARE_APP(lau::App);