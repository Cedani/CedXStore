#include "Password.hpp"

lau::Password::Password(wxPanel *parent, wxBoxSizer *pBox, const wxString &plc): Label(parent, pBox, plc), _show(false), _majCount(0), _minCount(0), _numCount(0), _size(0), _specChar(0), _unallowedCharCount(0) ,_specialChars("! #$%&*,+-*/:;<=>?@[]^_|{}~"), _eventFocus(0)
{
    init();
}

lau::Password::~Password()
{
}

void lau::Password::recreateInput()
{
    // wxString value = _input->GetValue();
    wxTextCtrl *newInput =  new wxTextCtrl(_mPanel, _input->GetId(), _input->GetValue(), wxDefaultPosition, wxDefaultSize
        , (_show) ? wxTE_PROCESS_ENTER : wxTE_PROCESS_ENTER | wxTE_PASSWORD);

    _input->GetContainingSizer()->Replace(_input, newInput);
    _input->Destroy();
    _input = newInput;
    _mBox->Layout();
    _input->SetHint(_placeholder);
    bindEvent();
    wxString val = _input->GetValue();
    resetCounts(val);
}

void lau::Password::initButton()
{
    _shButton = new wxButton(_mPanel, wxID_ANY);
    _shButton->SetBitmap(wxBitmap(RESOURCES + _("Images/show.png"), wxBITMAP_TYPE_ANY));
    _shButton->SetMaxSize(wxSize(30, 21));
    _shButton->SetBackgroundColour(wxColor(45, 45, 45));
    _shButton->SetWindowStyle(wxBORDER_NONE);
    _mBox->Add(_shButton, 0, wxEXPAND | wxLEFT, 5);
}

void lau::Password::initHideElements()
{

}

void lau::Password::init()
{
    _mPanel = new wxPanel(_parent);

    _mBox = new wxBoxSizer(wxHORIZONTAL);
    _mPanel->SetSizer(_mBox);

    _input = new wxTextCtrl(_mPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize
        , wxTE_PROCESS_ENTER | wxTE_PASSWORD);

    _input->SetHint(_placeholder);


    setMaxSize(-1, 30);
    setPanelSizerBorder(wxEXPAND | wxBOTTOM, 15);
    setSizerBorder(wxLEFT | wxALIGN_CENTER, 15);

    initButton();
    bindEvent();
}

void lau::Password::checkCharacter(wxUniCharRef c, bool dec)
{
    if (wxIsdigit(c)) {
        _numCount += (dec) ? -1 : 1;
        std::cout << "digit char [" << wxString(c) << "]" << std::endl;
    }
    else if (_specialChars.Contains(c)) {
        _specChar += (dec) ? -1 : 1;
        std::cout << "specChar char [" << wxString(c) << "]" << std::endl;
    }
    else if (wxIsupper(c)) {
        _majCount += (dec) ? -1 : 1;
        std::cout << "upper char [" << wxString(c) << "]" << std::endl;
    }
    else if (wxIslower(c)) {
        _minCount += (dec) ? -1 : 1;
        std::cout << "min char [" << wxString(c) << "]" << std::endl;
    }
    else {
        _unallowedCharCount += (dec) ? -1 : 1;
        std::cout << "unallowed char [" << wxString(c) << "]" << std::endl;
    }
}

void lau::Password::resetCounts(wxString &value)
{
    _majCount = 0;
    _minCount = 0;
    _numCount = 0;
    _specChar = 0;
    _unallowedCharCount = 0;
    for (auto c: value)
        checkCharacter(c);
}

void lau::Password::takeFocus()
{
    _input->SetFocus();
}

void lau::Password::setFocusEvent(wxEventType evtType)
{
    _eventFocus = evtType;
    _input->Bind(wxEVT_TEXT_ENTER, [this, evtType] (wxCommandEvent &evt) {
        wxCommandEvent toEmit(evtType);
        _input->ProcessWindowEvent(toEmit);
    });
}

wxString lau::Password::checkRequirements()
{
    wxString error;
    wxString value = _input->GetValue();

    if (value.size() < 6)
        error += _("The password should be at least 6 characters\n");
    if (_majCount == 0)
        error += _("The password should contain at least 1 uppercase letter\n");
    if (_minCount == 0)
        error += _("The password should contain at least 1 lowercase letter\n");
    if (_numCount == 0)
        error += _("The password should contain at least 1 number\n");
    if (_specChar == 0)
        error += _("The password should contain at least 1 special character\n");
    if (_unallowedCharCount > 0)
        error += _("The password contains unallowed character.\n the only character autorhized are alphanumerical and these: ") + _specialChars;
    // std::cout << error << std::endl;
    return (error);
}

wxString lau::Password::checkRequirements(const wxString &pWord)
{
    return ((pWord != _input->GetValue()) ? _("The passwords does not correspond") : _(""));
}

void lau::Password::bindEvent()
{
    setFocusEvent(_eventFocus);

    _shButton->Bind(wxEVT_BUTTON, [this] (wxCommandEvent &evt) {
        if (_show)
            _shButton->SetBitmap(wxBitmap(RESOURCES + _("Images/show.png"), wxBITMAP_TYPE_ANY));
        else
            _shButton->SetBitmap(wxBitmap(RESOURCES + _("Images/hide.png"), wxBITMAP_TYPE_ANY));
        _show = !_show;
        recreateInput();
    });

    _input->Bind(wxEVT_TEXT, [this] (wxCommandEvent &evt) {
        wxString value = _input->GetValue();
        _size = value.length();
        (value.length() > _lastValue.length()) ? checkCharacter(value.Last()) : checkCharacter(_lastValue.Last(), true);
        _lastValue = value;
    });
}