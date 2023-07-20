#pragma once
#include <wx/event.h>
namespace lau {
    enum {
        ID_HELLO = 1,
        ID_CHANGE_LANGUAGE,
        ID_SHOP,
        ID_CHECK_PSEUDO
    };
}

// events Login
wxDECLARE_EVENT(EVENT_PSEUDO, wxCommandEvent); // this will be emitted whenever the pseudo form is not empty and the user did not add anything for x seconds
wxDECLARE_EVENT(EVENT_PSEUDO_AVAILABLE, wxCommandEvent); // this will be emitted if the pseudo entered is available
wxDECLARE_EVENT(EVENT_PSEUDO_UNAVAILABLE, wxCommandEvent); // this will be emitted if the pseudo entered is unavailable
