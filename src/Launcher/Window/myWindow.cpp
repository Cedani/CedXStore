#include "myWindow.hpp"


lau::Window::Window(): wxFrame(NULL, wxID_ANY, "Test")
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

    SetStatusText("Wilkommen");

    Bind(wxEVT_MENU, &Window::OnHello, this, ID_HELLO);
    Bind(wxEVT_MENU, &Window::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &Window::OnExit, this, wxID_EXIT);
}

void lau::Window::OnHello(wxCommandEvent &evt)
{
    wxLogMessage("Hello world from wxWidgets!");
}

void lau::Window::OnAbout(wxCommandEvent &evt)
{
    wxMessageBox("This is a wxWidgets Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
}

void lau::Window::OnExit(wxCommandEvent &evt)
{
    Close(true);
}