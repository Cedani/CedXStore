#pragma once
// #include <wx/wx.h>
#include <wx/app.h>
#include <wx/image.h>
#include "myWindow.hpp"
#include "Login.hpp"
#include "ThreadPool.hpp"
#include "Manager.hpp"
#include "Loading.hpp"

namespace lau {
    class App : public wxApp {
        public:
            bool OnInit() override;
            void printer();
            void exitLogin();
            void exitLoading(bool);
            Window *_myWindow;
            Login *_login;
            Loading *_loading;

            void callHandlePseudo(const char *);
            void connectToServer();

        private:
            void bindEvents();
            void handlePseudoAvailability(const char *);




        private:
            thp::ThreadPool _threadpool = thp::ThreadPool(5);
            Manager _sender;
            
            //handler
    };
};

wxDECLARE_APP(lau::App);