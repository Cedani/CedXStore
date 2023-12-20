#include "App.hpp"

wxDEFINE_EVENT(EVENT_CONNECTED, wxCommandEvent);
wxDEFINE_EVENT(EVENT_NOT_CONNECTED, wxCommandEvent);
wxDEFINE_EVENT(EVENT_CLOSE_LOADING, wxCommandEvent);

bool lau::App::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxGIFHandler);
    _loading = new Loading();
    _loading->Show(true);
    SetTopWindow(_loading);
    bindEvents();
    _threadpool.init();
    // connectToServer();
    // _threadpool.addClassTask(&App::connectToServer, this);
    _threadpool.addNewTask([this] () {
        connectToServer();
    });
    return true;
}

void lau::App::connectToServer()
{
    std::cout << "how many time" << std::endl;
    wxCommandEvent evt(((_sender.initClientLogin()) ? EVENT_CONNECTED : EVENT_NOT_CONNECTED));

    
    // evt.set
    // _loading->ProcessWindowEvent(evt);
    // GetTopWindow()->ProcessWindowEvent(evt);
    // ProcessEvent(evt);
    // ProcessThreadEvent(evt);
    QueueEvent(new wxCommandEvent(evt));
    // _loading->GetEventHandler()->QueueEvent(new wxCommandEvent(evt));
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

void lau::App::exitLoading(bool connected)
{
    if (!connected)
        return;
    _login = new Login();
    _login->Show(true);
    SetTopWindow(_login);
    // std::cout << "wtf" << std::endl;
}

void lau::App::bindEvents()
{
    Bind(EVENT_CONNECTED, [this] (wxCommandEvent &) {
        _loading->Close(true);
        exitLoading(true);
    });

    Bind(EVENT_NOT_CONNECTED, [this] (wxCommandEvent &) {
        _loading->handleNotConnected();
        _threadpool.addNewTask([this] () {
            std::this_thread::sleep_for(std::chrono::milliseconds(2500));
            QueueEvent(new wxCommandEvent(EVENT_CLOSE_LOADING));
        });
    });

    Bind(EVENT_CLOSE_LOADING, [this] (wxCommandEvent &) {
        exitLoading(true);
        _loading->Close(true);
    });
}

wxIMPLEMENT_APP(lau::App);