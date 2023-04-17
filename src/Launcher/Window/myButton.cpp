#include "myButton.hpp"

lau::MyButton::MyButton(wxPanel *parent, wxBoxSizer *box, const wxString &label, const wxString &imgPath): _parent(parent), _vBox(box), _label(label), _imgPath(RESOURCES + imgPath)
{
    init();
    initButton();
}

void lau::MyButton::init()
{
   _panel = new wxPanel(_parent);
   _panel->SetMaxSize(wxSize(-1, 77));
   _panel->SetBackgroundColour(*wxWHITE);
   _vBox->Add(_panel, 1, wxEXPAND | wxALL, 10);

   _mBox = new wxBoxSizer(wxVERTICAL);
   _panel->SetSizer(_mBox);
}

void lau::MyButton::initButton()
{
    _button = new wxButton(_panel, wxID_ANY, _label);
    _button->SetBackgroundColour(wxColor(20, 20, 20));
    _button->SetForegroundColour(*wxWHITE);
    _button->SetBitmap(wxBitmap(_imgPath, wxBITMAP_TYPE_ANY));
    _button->SetMaxSize(wxSize(-1, 75));
    _button->SetWindowStyle(wxBORDER_NONE);

    _button->Bind(wxEVT_ENTER_WINDOW, [this](wxMouseEvent &){
        _button->SetBackgroundColour(wxColor(51, 51, 51));
    });
    _button->Bind(wxEVT_LEAVE_WINDOW, [this](wxMouseEvent &){
        _button->SetBackgroundColour(wxColor(20, 20, 20));
    });

    _mBox->Add(_button, 2, wxEXPAND | wxALL, 2);
}