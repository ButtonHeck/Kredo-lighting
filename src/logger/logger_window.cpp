#include "logger_window.h"
#include "logger_controller.h"

#include <wx/textctrl.h>
#include <wx/sizer.h>

namespace Kredo
{

LoggerWindow::LoggerWindow(wxWindow* parent)
    : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
    , _loggerController(new LoggerController(new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY)))
{
    wxLog::SetActiveTarget(_loggerController);

    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(_loggerController->TextLogger(), 1, wxEXPAND | wxALL);

    SetSizerAndFit(sizer);
}

}
