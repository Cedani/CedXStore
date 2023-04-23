#include "App.hpp"

bool lau::App::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler);
    _myWindow = new Window();
    _myWindow->Show(true);
    SetTopWindow(_myWindow);
    auto tmp = wxColor(255, 255, 255, 0);
    return true;
}

void lau::App::printer()
{
    
    // std::cout << "test" << std::endl;
    // _myWindow->printAbout();
    wxMessageBox("Test get app", "Test get App", wxOK | wxICON_INFORMATION);
}

wxIMPLEMENT_APP(lau::App);