#include "App.hpp"

bool lau::App::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxGIFHandler);
    _login = new Login();
    _login->Show(true);
    // _sender.initClientLogin();
    // _myWindow = new Window();
    // _myWindow->Show(true);
    // _myWindow->Close();
    SetTopWindow(_login);
    // auto tmp = wxColor(255, 255, 255, 0);
    return true;
}

void lau::App::printer()
{
    // wxPostEvent(_login->GetEventHandler(), );
    // std::cout << "test" << std::endl;
    // _myWindow->printAbout();
    wxMessageBox("Test get app", "Test get App", wxOK | wxICON_INFORMATION);
}

void lau::App::exitLogin()
{
    _myWindow = new Window();
    _myWindow->Show(true);
    SetTopWindow(_myWindow);
    std::cout << "oieoiwoiekjsiow" << std::endl;
    
}

void lau::App::handlePseudoAvailability(const char *value)
{
    nlohmann::json result = _sender.checkPseudoAvailability(value);

    if (result["code"] != 200)
        return;
    bool isAvailable = result["isAvailable"];
    wxCommandEvent eventPseudo;

    if (isAvailable) {
        eventPseudo.SetEventType(EVENT_PSEUDO_AVAILABLE);
        eventPseudo.SetId(_login->GetId());
    } else {
        eventPseudo.SetEventType(EVENT_PSEUDO_UNAVAILABLE);
        eventPseudo.SetId(_login->GetId());
    }
    eventPseudo.SetEventObject(_login);
    _login->ProcessWindowEvent(eventPseudo);
}

void lau::App::callHandlePseudo(const char *pseudo)
{
    _threadpool.addNewTask([this, pseudo]() {
        handlePseudoAvailability(pseudo);
    });
}

wxIMPLEMENT_APP(lau::App);