#include "myWindow.hpp"
#include "App.hpp"


lau::Window::Window(): wxFrame(NULL, wxID_ANY, "Test", wxDefaultPosition, wxSize(800, 600))
{
    initMenu();
    initPanel();

    SetMinSize(wxSize(800, 600));
    Bind(wxEVT_MENU, &Window::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &Window::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &Window::OnChange, this, ID_HELLO);
    Center();
    // SetTransparent(1);
}

lau::Window::~Window()
{
    if (_sideMenu)
        delete _sideMenu;
}

void lau::Window::initPanel()
{
    _mainPanel = new wxPanel(this, -1);
    _mainPanel->SetBackgroundColour(wxColor(20, 20, 20));

    _testPanel = new wxPanel(_mainPanel, -1, wxDefaultPosition, wxSize(400, 400));
    //SideMenu
    _mainBox = new wxBoxSizer(wxHORIZONTAL);
    _sideMenu = new SideMenu(_mainPanel, _mainBox);
    _mainBox->Add(_testPanel, 2, wxALIGN_CENTER_VERTICAL, 10);
    _mainPanel->SetSizer(_mainBox);
}


void lau::Window::OnAbout(wxCommandEvent &evt)
{
    // wxGetApp().printer();
    
}

void lau::Window::OnExit(wxCommandEvent &evt)
{
    Close(true);
}

void lau::Window::initMenu()
{
    _menuFile = new wxMenu;
    _menuFile->Append(ID_HELLO, "&Hello...\tCtrl-h",
    "Help string shown in status bar for this menu item");
    _menuFile->AppendSeparator();
    _menuFile->Append(wxID_EXIT);

    _menuHelp = new wxMenu;
    _menuHelp->Append(wxID_ABOUT);

    _menuBar = new wxMenuBar;
    _menuBar->Append(_menuFile, "&File");
    _menuBar->Append(_menuHelp, "&Help");

    SetMenuBar(_menuBar);

    CreateStatusBar();

}

void lau::Window::OnChange(wxCommandEvent &)
{
    std::cout << "ca marche bordel de mierda" << std::endl;
}