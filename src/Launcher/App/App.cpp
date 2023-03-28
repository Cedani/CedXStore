#include "App.hpp"

bool lau::App::OnInit()
{
    _myWindow = new Window();
    _myWindow->Show(true);
    return true;
}

// wxIMPLEMENT_APP(lau::App);