#pragma once
#include <wx/event.h>
#include <wx/colour.h>
namespace lau {
    enum {
        ID_HELLO = 1,
        ID_CHANGE_LANGUAGE,
        ID_SHOP,
        ID_CHECK_PSEUDO
    };
}

////////////////////
// color for apps //
////////////////////
#define RBLUE wxColor(0, 83, 156)
#define PEACH wxColor(238, 164, 127)

/////////////////////////////
// event for loading panel //
/////////////////////////////
wxDECLARE_EVENT(EVENT_NOT_CONNECTED, wxCommandEvent); // this will be emitted if cant connect to remote server
wxDECLARE_EVENT(EVENT_CONNECTED, wxCommandEvent); // this will be emitted if succesfully connected to remote server
wxDECLARE_EVENT(EVENT_CLOSE_LOADING, wxCommandEvent);


//////////////////////////
// event for signing up //
//////////////////////////
wxDECLARE_EVENT(EVENT_PSEUDO, wxCommandEvent); // this will be emitted whenever the pseudo form is not empty and the user did not add anything for x seconds
wxDECLARE_EVENT(EVENT_PSEUDO_AVAILABLE, wxCommandEvent); // this will be emitted if the pseudo entered is available
wxDECLARE_EVENT(EVENT_PSEUDO_UNAVAILABLE, wxCommandEvent); // this will be emitted if the pseudo entered is unavailable


//////////////////////
// events graphical //
//////////////////////
wxDECLARE_EVENT(EVENT_PASSWORD_FOCUS, wxCommandEvent); // password field takes focus
wxDECLARE_EVENT(EVENT_PASSWORD_CONFIRM_FOCUS, wxCommandEvent); // confirm password field takes focus

