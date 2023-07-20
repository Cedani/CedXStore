#include "Password.hpp"

lau::Password::Password(wxPanel *parent, wxBoxSizer *pBox, const wxString &plc): Label(parent, pBox, plc), _show(false)
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
}

void lau::Password::initButton()
{
    _shButton = new wxButton(_mPanel, wxID_ANY);
    _shButton->SetBitmap(wxBitmap(RESOURCES + _("Images/show.png"), wxBITMAP_TYPE_ANY));
    _shButton->SetMaxSize(wxSize(30, 21));
    _shButton->SetBackgroundColour(wxColor(45, 45, 45));
    _shButton->SetWindowStyle(wxBORDER_NONE);
    _shButton->Bind(wxEVT_BUTTON, [this] (wxCommandEvent &evt) {
        if (_show)
            _shButton->SetBitmap(wxBitmap(RESOURCES + _("Images/show.png"), wxBITMAP_TYPE_ANY));
        else
            _shButton->SetBitmap(wxBitmap(RESOURCES + _("Images/hide.png"), wxBITMAP_TYPE_ANY));
        _show = !_show;
        recreateInput();
    });
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
    _input->Bind(wxEVT_TEXT, [this] (wxCommandEvent &) {
        wxString value = _input->GetValue();
    
    });

    _input->SetHint(_placeholder);


    setMaxSize(-1, 30);
    setPanelSizerBorder(wxEXPAND | wxBOTTOM, 15);
    setSizerBorder(wxLEFT | wxALIGN_CENTER, 15);

    initButton();
}