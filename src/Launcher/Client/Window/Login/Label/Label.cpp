#include "Label.hpp"

lau::Label::Label(wxPanel *parent, wxBoxSizer *pBox, const wxString &pla): _parent(parent), _pBox(pBox), _placeholder(pla)
{
    // init();
}

lau::Label::~Label()
{
    // _timer.resetTimer();
}

// void lau::Label::init()
// {
//     _mPanel = new wxPanel(_parent);

//     _mBox = new wxBoxSizer(wxHORIZONTAL);
//     _mPanel->SetSizer(_mBox);

//     if (_pw) {
//         _input = new wxTextCtrl(_mPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize
//         , wxTE_PROCESS_ENTER | wxTE_PASSWORD);
//         _timer.stopTimer();
//     }
//     else {
//         _input = new wxTextCtrl(_mPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize
//         , wxTE_PROCESS_ENTER);
//         _input->Bind(wxEVT_TEXT, [this] (wxCommandEvent &) {
//             _value = _input->GetValue();
//             // if (_value)
//             std::cout << "text changed" << std::endl;
//             if (!_value.empty())
//                 _timer.resetTimer(5, [this]() {
//                     std::cout << "value not changed for 5 seconds" << std::endl;
//                 });
//             else
//                 _timer.resetTimer();
//             // _timer.startTimer(10, [this]() {
//             //     std::cout << "value is " << _input->GetValue();
//             // });
//         });
//     }

//     _input->SetHint(_placeholder);

//     // _input.get
//     // _mPanel->SetMinSize(wxSize(-1, 30));
//     // _input->SetForegroundColour(wxColor(255, 0, 0));
// }

void lau::Label::setPanelSizerBorder(int flags, int borderSize)
{
    _pBox->Add(_mPanel, 0, flags, borderSize);
}

void lau::Label::setSizerBorder(int flags, int borderSize)
{
    _mBox->Add(_input, 2, flags, borderSize);
}

void lau::Label::setMaxSize(int w, int h)
{
    _input->SetMaxSize(wxSize(w, h));
    update();
}

void lau::Label::Show(bool sh)
{
    // _show = sh;
    // if (sh)
        // _input->Show(sh);
        _mBox->Show(sh);
    // else
        // _input->Hide();
        // _mBox->Hide();
    // update();
}

void lau::Label::update()
{
    _mPanel->Refresh();
    _mPanel->Update();
}

wxString lau::Label::getValue()
{
    return _input->GetValue();
}
