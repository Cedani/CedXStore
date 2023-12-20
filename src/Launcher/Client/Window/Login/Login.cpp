#include "Login.hpp"
#include "App.hpp"

wxDEFINE_EVENT(EVENT_PSEUDO, wxCommandEvent);
wxDEFINE_EVENT(EVENT_PASSWORD_FOCUS, wxCommandEvent);
wxDEFINE_EVENT(EVENT_PSEUDO_AVAILABLE, wxCommandEvent);
wxDEFINE_EVENT(EVENT_PSEUDO_UNAVAILABLE, wxCommandEvent);
wxDEFINE_EVENT(EVENT_PASSWORD_CONFIRM_FOCUS, wxCommandEvent);

lau::Login::Login(): wxFrame(NULL, wxID_ANY, "Login/Signup", wxDefaultPosition, wxSize(300, 500)), _login(true)
{
    Bind(wxEVT_MENU, &Login::OnExit, this, wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &Login::OnClose, this);
    // SetMaxSize(wxSize(300, 500));
    Center();
    // std::cout << "check 1" << std::endl;
    init();
    // std::cout << "check 2" << std::endl;
}

lau::Login::~Login()
{
    if (_pseudo)
        delete _pseudo;
    if (_confirmPassword)
        delete _confirmPassword;
    if (_password)
        delete _password;
}

void lau::Login::init()
{
    _contPanel = new wxPanel(this, wxID_ANY);
    _contBox = new wxBoxSizer(wxVERTICAL);
    _contPanel->SetScrollbar(0, 0, 200, 500);
    // _contPanel->SetBackgroundColour(RBLUE);
    _contPanel->SetSizer(_contBox);

    _mainPanel = new wxScrolledWindow(_contPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
    // _mainPanel->SetBackgroundColour(wxColor(45, 45, 45));
    _mainPanel->SetScrollRate(0, 5);
    _mainPanel->SetVirtualSize(wxSize(300, 500));
    _mainBox = new wxBoxSizer(wxVERTICAL);

    _contBox->Add(_mainPanel, 2, wxEXPAND);
    _mainPanel->SetSizer(_mainBox);

    // _mainBox->FitInside(_mainPanel);
    // _mainPanel->SetScrollbar(0, 0, 700, 0);

    // std::cout << "here" << std::endl;

    // _scrWin = new wxScrolledWindow(_mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
    // _scrWin->SetScrollRate(0,5);
    // _scrWin->SetVirtualSize(wxSize(300, 700));


    // login text
    _loginText = new wxStaticText(_mainPanel, wxID_ANY, _("Login"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    _loginText->SetForegroundColour(*wxWHITE);
    _loginText->SetFont(_loginText->GetFont().Scale(2.5).MakeBold());
    _mainBox->Add(_loginText, 0, wxEXPAND | wxTOP, 50);

    // pseudo input
    _pseudo = new Pseudo(_mainPanel, _mainBox, [this]() {
        if (!_login) {
            // wxGetApp().callHandlePseudo(_pseudo->getValue());
        }
    });

    // password input
    _password = new Password(_mainPanel, _mainBox);
    _password->setFocusEvent(EVENT_PASSWORD_CONFIRM_FOCUS);

    // comfirm password input
    _confirmPassword = new Password(_mainPanel, _mainBox, _("Confirm password"));

    _grid = new wxGridSizer(2, 1, 1, 3);
    _mainBox->Add(_grid, 0, wxEXPAND | wxTOP, 10);
    
    _alrd = new wxStaticText(_mainPanel, wxID_ANY, _("Don't have any account yet? "), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    _alrd->SetForegroundColour(*wxWHITE);
    _alrd->SetFont(_alrd->GetFont().Scale(1));
    _grid->Add(_alrd, 1, wxEXPAND);

    _sigLogLink = new wxHyperlinkCtrl(_mainPanel, -1, _("signup here"), "", wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
    _sigLogLink->Bind(wxEVT_HYPERLINK, [this](wxHyperlinkEvent &evt) {
        handleAlreadySignupLogin(evt);
    });
    _sigLogLink->SetForegroundColour(*wxWHITE);
    _sigLogLink->SetFont(_sigLogLink->GetFont().Scale(1));
    _grid->Add(_sigLogLink, 1, wxEXPAND | wxLEFT, 110);

    _confirm = new MyButton(_mainPanel, _mainBox, _("Login"), "", false);

    _mainBox->FitInside(_mainPanel);
    initErrorField();

    // _mainBox->Fit(_scrWin);
    Layout();
    _pseudo->initHideElements();
    _confirmPassword->Show(false);
    _errorField->Show(false);
    bindEvents();
}

void lau::Login::initErrorField()
{
    _errorPanel = new wxPanel(_mainPanel);
    _errorSizer = new wxBoxSizer(wxHORIZONTAL);
    _errorPanel->SetSizer(_errorSizer);

    _errorField = new wxStaticText(_errorPanel, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    _errorField->SetForegroundColour(*wxRED);
    _errorField->SetFont(_errorField->GetFont().Scale(1.3).MakeSmaller());
    _mainBox->Add(_errorPanel, 1, wxEXPAND | wxTOP, 10);
    _errorSizer->Add(_errorField, 1, wxEXPAND | wxCENTER);
}

void lau::Login::OnClose(wxCloseEvent &evt)
{
    // std::cout << "uiweiuiuweiuwew" << std::endl;
    wxGetApp().exitLogin();
    Destroy();
}

void lau::Login::OnExit(wxCommandEvent &evt)
{
    // std::cout << "lkdslksdlksdklsldk" << std::endl;
    wxGetApp().exitLogin();
    Destroy();
}

void lau::Login::handleAlreadySignupLogin(wxHyperlinkEvent &evt)
{

    if (_login) {
        _confirm->setLabel(_("Signup"));
        _confirmPassword->Show(true);
        _sigLogLink->SetLabel("login here");
        _alrd->SetLabel("Already have an account ?");
        // _grid->Layout();
    } else {
        _confirm->setLabel(_("Login"));
        _confirmPassword->Show(false);
        _sigLogLink->SetLabel("signup here");
        _alrd->SetLabel("Don't have any account yet?");
    }
    _grid->Layout();
    _errorField->Show(false);
    _login = !_login;
}

void lau::Login::clickLogin()
{
    if (_pseudo->getValue().length() && _password->getValue().length()) {
        std::cout << "launching function login" << std::endl;
        return;
    }
    _errorField->SetLabel(_("Pseudo and password should not be empty"));
    _errorField->Show(true);
    _errorPanel->Layout();
}

void lau::Login::clickSignup()
{
    wxString error = _pseudo->checkRequirements();
    error += (error.Last() == '\n' || error.empty()) ? _password->checkRequirements() : _("\n") + _password->checkRequirements();
    error += (error.Last() == '\n' || error.empty()) ? _confirmPassword->checkRequirements(_password->getValue()) : _("\n") + _confirmPassword->checkRequirements(_password->getValue());

    if (!error.empty()) {
        _errorField->SetLabel(error);
        _errorField->Show(true);
        _errorPanel->Layout();
    } else {
        std::cout << "Launching signup function" << std::endl;
    }
}

void lau::Login::bindEvents()
{
    // event when pseudo is available
    // Bind(EVENT_PSEUDO_AVAILABLE, [this] (wxCommandEvent &evt) {
    //     _pseudo->handlePseudoAvailable(evt);
    // });

    // // event when pseudo is unavailable
    // Bind(EVENT_PSEUDO_UNAVAILABLE, [this] (wxCommandEvent &evt) {
    //     _pseudo->handlePseudoUnavailable(evt);
    // });

    Bind(EVENT_PASSWORD_FOCUS, [this](wxCommandEvent &evt) {
        _password->takeFocus();
    });

    Bind(EVENT_PASSWORD_CONFIRM_FOCUS, [this](wxCommandEvent &evt) {
        if (!_login)
            _confirmPassword->takeFocus();
    });

    _confirm->bindClickFunction([this](wxCommandEvent & evt) {
        std::cout << "here" << std::endl;
        _errorField->Show(false);
        if (_login)
            clickLogin();
        else
            clickSignup();
    });
}