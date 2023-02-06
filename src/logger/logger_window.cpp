#include "logger_window.h"
#include "logger_controller.h"

#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/button.h>

namespace Kredo
{

enum ButtonID
{
    ID_ButtonClear = wxID_HIGHEST + 1,
    ID_ButtonFont,
};

LoggerWindow::LoggerWindow(wxWindow* parent)
    : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
    , _loggerController(new LoggerController(new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY)))
{
    wxLog::SetActiveTarget(_loggerController);

    wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    wxPanel* controlsPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    wxSizer* controlsSizer = new wxBoxSizer(wxVERTICAL);

    wxButton* clearButton = new wxButton(controlsPanel, ID_ButtonClear, "Clear");
    clearButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { _loggerController->Clear(); });

    wxButton* fontButton = new wxButton(controlsPanel, ID_ButtonFont, "Font");

    controlsSizer->Add(clearButton, 1, wxALL, 2);
    controlsSizer->Add(fontButton, 1, wxALL, 2);
    controlsPanel->SetSizerAndFit(controlsSizer);


    sizer->Add(controlsPanel, 0, wxRIGHT, 10);
    sizer->Add(_loggerController->TextLogger(), 1, wxEXPAND);

    SetSizerAndFit(sizer);
}

}
