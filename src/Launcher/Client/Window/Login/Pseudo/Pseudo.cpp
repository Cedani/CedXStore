#include "Pseudo.hpp"

// #ifdef _WIN32
//     #include <windows.h>
// #endif

lau::Pseudo::Pseudo(wxPanel *parent, wxBoxSizer *pBox, std::function<void()> func): Label(parent, pBox, _("enter your pseudo")), _timerFunctor(func), _isAvailable(false)
{
    init();
}

lau::Pseudo::~Pseudo()
{
    _timer.stopTimer();
}

void lau::Pseudo::initLabelAvailable()
{
    _available = new wxStaticText(_parent, -1, _(""));
    // _available->SetForegroundColour(*wxGREEN);
    _available->SetFont(_available->GetFont().Scale(1).Bold());
    _pBox->Add(_available, 0, wxEXPAND | wxLEFT, 15);
}

void lau::Pseudo::initLoading()
{
    _loading = new wxAnimationCtrl(_mPanel, wxID_ANY);
    if (!_loading->LoadFile(RESOURCES + _("Images/loading.gif"), wxANIMATION_TYPE_GIF))
        std::cout << "could not load gif" << std::endl;
    _loading->SetMinSize(wxSize(-1, 21));
    _mBox->Add(_loading, 0, wxALIGN_CENTER | wxLEFT, 5);
}

void lau::Pseudo::init()
{
    _mPanel = new wxPanel(_parent);

    _mBox = new wxBoxSizer(wxHORIZONTAL);
    _mPanel->SetSizer(_mBox);

    _input = new wxTextCtrl(_mPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize
        , wxTE_PROCESS_ENTER);
    
    bindEvents();

    _input->SetHint(_placeholder);
    setMaxSize(-1, 30);
    setPanelSizerBorder(wxEXPAND | wxTOP, 20);
    setSizerBorder(wxLEFT | wxALIGN_CENTER, 15);

    initLoading();
    initLabelAvailable();
}

void lau::Pseudo::initHideElements()
{
    _loading->Show(false);
    _available->Show(false);
}

void lau::Pseudo::handlePseudoAvailable(wxCommandEvent &)
{
    _available->Show(true);
    _available->SetLabel(_input->GetValue() + _("is available"));
    _available->SetForegroundColour(*wxGREEN);
}

void lau::Pseudo::handlePseudoUnavailable(wxCommandEvent &)
{
    showLoading(false);
    _available->Show(true);
    _available->SetLabel(_input->GetValue() + _("is unavailable"));
    _available->SetForegroundColour(*wxRED);
}

void lau::Pseudo::showLoading(bool sh)
{
    _loading->Show(sh);
}

void lau::Pseudo::bindEvents()
{
    _input->Bind(wxEVT_TEXT, [this] (wxCommandEvent &) {
        wxString value = _input->GetValue();

        if (value.length() > MAX_STR_LENGTH) {
            _input->ChangeValue(value.substr(0, MAX_STR_LENGTH));
            _input->SetInsertionPointEnd();
        }

        if (_value.empty())
            _timer.resetTimer();
        else {
            _timer.resetTimer(5, _timerFunctor);
            _available->Show(false);
        }
    });

    _input->Bind(wxEVT_TEXT_ENTER, [this] (wxCommandEvent& event) {
        wxCommandEvent evt(EVENT_PASSWORD_FOCUS);
        _input->ProcessWindowEvent(evt);
    });
}

wxString lau::Pseudo::checkRequirements()
{
    wxString error;
    wxString value = _input->GetValue();

    if (!_isAvailable && !value.empty())
        error +=  value + _(" is not available\n");
    if (value.length() < 3)
        error += "The pseudo should have at least 4 characters";
    return error;
}

