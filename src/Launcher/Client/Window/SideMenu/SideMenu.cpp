#include "SideMenu.hpp"

lau::SideMenu::SideMenu(wxPanel *parent, wxBoxSizer *vBox): _parent(parent), _vBox(vBox)
{
    initPanel();
    initButtons();
}

lau::SideMenu::~SideMenu()
{
    if (_Library)
        delete _Library;
    if (_Store)
        delete _Store;
    if (_Downloads)
        delete _Downloads;
}

void lau::SideMenu::initPanel()
{
    _panel = new wxPanel(_parent, wxID_ANY, wxDefaultPosition, wxSize(100, 100));
    _vBox->Add(_panel, 1, wxALIGN_LEFT | wxEXPAND, 10);
    _panel->SetMaxSize(wxSize(150, -1));
    _panel->SetBackgroundColour(wxColor(45, 45, 45));
    _mBox = new wxBoxSizer(wxVERTICAL);
    _panel->SetSizer(_mBox);
}

void lau::SideMenu::initButtons()
{
    _Library = new MyButton(_panel, _mBox, _("Library"), _("Images/library.png"));
    _Store = new MyButton(_panel, _mBox, _("Store"), _("Images/shop.png"));
    _Downloads = new MyButton(_panel, _mBox, _("Download"), _("Images/download.png"));


    // useless
    _useless = new wxPanel(_panel, wxID_ANY);
    _mBox->Add(_useless, 2);
}