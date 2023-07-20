#include "myButton.hpp"

lau::MyButton::MyButton(wxPanel *parent, wxBoxSizer *box, const wxString &label, const wxString &imgPath, bool img): _parent(parent), _vBox(box), _label(label), _imgPath(RESOURCES + imgPath), _backgroundColor(20, 20, 20), _textColor(255, 255, 255) ,_hoverBackgroundColor(51, 51, 51), _hoverTextColor(255, 255, 255), _borderHover(true), _border(true), _borderColor(255, 255, 255), _hoverBorderColor(255, 255, 255), _img(img)
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
    _button->SetBackgroundColour(_backgroundColor);
    _button->SetForegroundColour(_textColor);

    if (_img)
        _button->SetBitmap(wxBitmap(_imgPath, wxBITMAP_TYPE_ANY));
    _button->SetMaxSize(wxSize(-1, 75));
    _button->SetWindowStyle(wxBORDER_NONE);

    _button->Bind(wxEVT_ENTER_WINDOW, [this](wxMouseEvent &){
        _button->SetForegroundColour(_hoverTextColor);
        _button->SetBackgroundColour(_hoverBackgroundColor);
    });
    _button->Bind(wxEVT_LEAVE_WINDOW, [this](wxMouseEvent &){
        _button->SetForegroundColour(_textColor);
        _button->SetBackgroundColour(_backgroundColor);
    });

    _mBox->Add(_button, 2, wxEXPAND | wxALL, 2);
}

void lau::MyButton::setDefaultBackgroundColor(wxColour color)
{
    _backgroundColor = color;
    _button->SetBackgroundColour(_backgroundColor);
    update();
}

void lau::MyButton::setHoverBackgroundColor(wxColour color)
{
    _hoverBackgroundColor = color;
}

void lau::MyButton::setHoverTextColor(wxColour color)
{
    _hoverTextColor = color;
}

void lau::MyButton::bindClickFunction(std::function<void(wxCommandEvent &)> func)
{
    _button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, func);
    update();
}

void lau::MyButton::setBorder(wxColour color)
{
    _borderColor = color;
    _panel->SetBackgroundColour(_borderColor);
    update();
}

// void lau::MyButton::se

// void lau::MyButton::setBorder(bool show)
// {
//     _border = show;
//     _panel->Show(show);
//     // update();
//     // _panel->L
// }

void lau::MyButton::setHoverBorderColor(bool show, wxColour colour)
{
    _borderHover = show;
    _hoverBorderColor = colour;
}

void lau::MyButton::refreshGraphicsEvent()
{
   _button->Bind(wxEVT_ENTER_WINDOW, [this](wxMouseEvent &){
        _button->SetForegroundColour(_hoverTextColor);
        _button->SetBackgroundColour(_hoverBackgroundColor);
        // _panel->Show(_borderHover);
        _panel->SetBackgroundColour(_hoverBorderColor);
    });
    _button->Bind(wxEVT_LEAVE_WINDOW, [this](wxMouseEvent &){
        _button->SetForegroundColour(_textColor);
        _button->SetBackgroundColour(_backgroundColor);
        // _panel->Show(_border);
        _panel->SetBackgroundColour(_borderColor);
    });
}

void lau::MyButton::update()
{
    _panel->Refresh();
    _panel->Update();
}

void lau::MyButton::setLabel(wxString lab)
{
    _button->SetLabel(lab);
    update();
}