#include "Loading.hpp"
#include "App.hpp"

lau::Loading::Loading(): wxFrame(NULL, wxID_ANY, "Connecting to Server", wxDefaultPosition, wxSize(400, 400))
{
    Center();
    init();
    SetMaxSize(wxSize(400, 400));
    bindEvents();
}

lau::Loading::~Loading()
{

}

void lau::Loading::init()
{
    // SetWindowStyle(0);

    _mainPanel = new wxPanel(this, wxID_ANY);
    _mainBox = new wxBoxSizer(wxVERTICAL);

    _mainPanel->SetBackgroundColour(RBLUE);
    _mainPanel->SetSizer(_mainBox);

    _mainBox->AddStretchSpacer();

    _loadingGif = new wxAnimationCtrl(_mainPanel, wxID_ANY);
    _loadingGif->LoadFile(RESOURCES + _("Images/loading_gif.gif"), wxANIMATION_TYPE_GIF);
    _mainBox->Add(_loadingGif, 1, wxALIGN_CENTER_HORIZONTAL);

    _loadingGif->Play();

    _errorNoConnectionText = new wxStaticText(_mainPanel, wxID_ANY, _("Connecting to remote Server"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
    _errorNoConnectionText->SetForegroundColour(PEACH);
    _errorNoConnectionText->SetFont(_errorNoConnectionText->GetFont().Scale(2).MakeBold());
    _mainBox->Add(_errorNoConnectionText, 2, wxTOP | wxEXPAND, 10);
    // _mainBox->AddStretchSpacer();
    Layout();
}

void lau::Loading::bindEvents()
{
    // Bind(EVENT_CONNECTED, [this] (wxCommandEvent &) {
    //     wxGetApp().exitLoading(true);
    //     // Destroy();
    //     Close(true);
    // });

    // Bind(EVENT_NOT_CONNECTED, [this] (wxCommandEvent &) {
    //     _errorNoConnectionText->SetLabel(_("Could not connect to server closing...."));
    //     _mainBox->Layout();
    //     // wxSleep(5);
    //     wxGetApp().exitLoading(true);
    //     Close(true);
    // });

    Bind(wxEVT_CLOSE_WINDOW, &Loading::OnClose, this);
}

void lau::Loading::OnClose(wxCloseEvent &evt)
{
    Destroy();
}

void lau::Loading::handleNotConnected()
{
    _errorNoConnectionText->SetLabel(_("Could not connect to server closing...."));
    _mainBox->Layout();
    // wxSleep(5);
    // wxGetApp().exitLoading(true);
    // Close(true);
}