#include "Login.hpp"
#include "App.hpp"

wxDEFINE_EVENT(EVENT_PSEUDO, wxCommandEvent);
wxDEFINE_EVENT(EVENT_PSEUDO_AVAILABLE, wxCommandEvent);
wxDEFINE_EVENT(EVENT_PSEUDO_UNAVAILABLE, wxCommandEvent);

lau::Login::Login(): wxFrame(NULL, wxID_ANY, "Login/Signup", wxDefaultPosition, wxSize(800, 600)), _login(true)
{
    // _mainPanel->Hide();
    // Bind(wxEVT_MENU, &Window::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &Login::OnExit, this, wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &Login::OnClose, this);
    SetMaxSize(wxSize(300, 500));
    Center();
    init();
}

lau::Login::~Login()
{
    if (_pseudo)
        delete _pseudo;
    // if (_confirmPassword)
    //     delete _confirmPassword;
    // if (_password)
    //     delete _password;
}

void lau::Login::init()
{
    _mainPanel = new wxPanel(this, -1);
    _mainPanel->SetBackgroundColour(wxColor(45, 45, 45));

    _mainBox = new wxBoxSizer(wxVERTICAL);
    _mainPanel->SetSizer(_mainBox);

    // login text
    _loginText = new wxStaticText(_mainPanel, wxID_ANY, _("Login"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    _loginText->SetForegroundColour(*wxWHITE);
    _loginText->SetFont(_loginText->GetFont().Scale(2.5).MakeBold());
    _mainBox->Add(_loginText, 0, wxEXPAND | wxTOP, 100);

    // pseudo input
    _pseudo = new Pseudo(_mainPanel, _mainBox, [this]() {
        if (!_login) {
            sendCustomEvent(EVENT_PSEUDO);
            wxGetApp().callHandlePseudo(_pseudo->getValue());
        }
    });

    // password input
    _password = new Password(_mainPanel, _mainBox);
    // _password->setPanelSizerBorder(wxEXPAND | wxTOP, 10);
    // _password->setSizerBorder(wxEXPAND | wxALL, 5);
    // _password->setMaxSize(-1, 30);

    // // password input
    _confirmPassword = new Password(_mainPanel, _mainBox, _("Confirm password"));
    // _confirmPassword->setPanelSizerBorder(wxEXPAND | wxTOP, 15);
    // _confirmPassword->setPanelSizerBorder(wxEXPAND | wxTOP, 10);
    // _confirmPassword->setSizerBorder(wxEXPAND | wxALL, 5);
    // _confirmPassword->setMaxSize(-1, 30);

    _grid = new wxGridSizer(2, 1, 1, 3);
    _mainBox->Add(_grid, 0, wxEXPAND | wxTOP, 10);
    
    _alrd = new wxStaticText(_mainPanel, wxID_ANY, _("Don't have any account yet? "), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    _alrd->SetForegroundColour(*wxWHITE);
    _alrd->SetFont(_alrd->GetFont().Scale(1));
    _grid->Add(_alrd, 1, wxEXPAND);
    // // _grid->AddSpacer(-10);

    _sigLogLink = new wxHyperlinkCtrl(_mainPanel, -1, _("signup here"), "", wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
    _sigLogLink->Bind(wxEVT_HYPERLINK, [this](wxHyperlinkEvent &evt) {
        handleAlreadySignupLogin(evt);
    });
    _sigLogLink->SetForegroundColour(*wxWHITE);
    _sigLogLink->SetFont(_sigLogLink->GetFont().Scale(1));
    // _grid->AddStretchSpacer(5);
    _grid->Add(_sigLogLink, 1, wxEXPAND | wxLEFT, 110);

    _confirm = new MyButton(_mainPanel, _mainBox, _("Login"), "", false);

    _errorField = new wxStaticText(_mainPanel, wxID_ANY, _("Error Field\n"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    _errorField->SetForegroundColour(*wxRED);
    // // _errorField->SetBackgroundColour(*wxWHITE);
    _errorField->SetFont(_errorField->GetFont().Scale(1.3).MakeSmaller());
    // // _errorField->SetMaxSize(wxSize(100, 30));
    _mainBox->Add(_errorField, 0, wxEXPAND | wxTOP, 10);
    Layout();
    _pseudo->initHideElements();
    // _password->initHideElements();
    _confirmPassword->Show(false);
}

void lau::Login::OnClose(wxCloseEvent &evt)
{
    std::cout << "uiweiuiuweiuwew" << std::endl;
    wxGetApp().exitLogin();
    Destroy();
}

void lau::Login::OnExit(wxCommandEvent &evt)
{
    std::cout << "lkdslksdlksdklsldk" << std::endl;
    wxGetApp().exitLogin();
    // Destroy()
}


void lau::Login::handleAlreadySignupLogin(wxHyperlinkEvent &evt)
{
    // std::cout << "clicked" << std::endl;

    if (_login) {
        _confirm->setLabel(_("Signup"));
        _confirmPassword->Show(true);
        // _mainPanel->H
        _sigLogLink->SetLabel("login here");
        _alrd->SetLabel("Already have an account ?");
        _grid->Layout();
    } else {
        _confirm->setLabel(_("Login"));
        _confirmPassword->Show(false);
        _sigLogLink->SetLabel("signup here");
        // _sigLogLink->Ena
        _alrd->SetLabel("Don't have any account yet?");
        _grid->Layout();
    }
    _login = !_login;
        // Update();
        // Refresh();
        // _mainPanel->Update();
        // _mainPanel->Refresh();
}

void lau::Login::showConfirm(bool sh)
{
    // _confirmPassword->Show(sh);
}

wxString lau::Login::getPseudo()
{
    return _pseudo->getValue();
}

void lau::Login::sendCustomEvent(wxEventType evtType)
{
    wxCommandEvent event(evtType);

    ProcessWindowEvent(event);
}