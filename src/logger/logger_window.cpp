#include "logger_window.h"
#include "logger_controller.h"
#include "icon_helpers.h"

#include <wx/font.h>
#include <wx/sizer.h>
#include <wx/toolbar.h>
#include <wx/textctrl.h>

namespace Kredo
{

enum ControlToolID
{
    ID_ControlToolClear = wxID_HIGHEST + 1,
    ID_ControlToolFontDecrease,
    ID_ControlToolFontIncrease,
    ID_ControlToolLast
};

LoggerWindow::LoggerWindow(wxWindow* parent, int id)
    : wxFrame(parent, id, "Log window", wxDefaultPosition, wxSize(480, 240))
    , _parent(parent)
    , _loggerController(new LoggerController(new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY)))
{
    wxLog::SetActiveTarget(_loggerController);

    const auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(MakeControlsToolBar(this), 0);
    sizer->Add(_loggerController->TextLogger(), 1, wxEXPAND);

    SetMinSize(wxSize(480, 240));
    SetSizer(sizer);

    Bind(wxEVT_CLOSE_WINDOW, &LoggerWindow::onWindowClose, this);
}

wxToolBar* LoggerWindow::MakeControlsToolBar(wxWindow* parent)
{
    auto controlsToolBar = new wxToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);

    controlsToolBar->AddTool(ID_ControlToolClear, "Clear", IconHelpers::LoadPngBitmap("/icons/broom.png", 16, 16));
    controlsToolBar->SetToolShortHelp(ID_ControlToolClear, "Clear");
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { _loggerController->Clear(); }, ID_ControlToolClear);

    controlsToolBar->AddTool(ID_ControlToolFontDecrease, "Decrease", IconHelpers::LoadPngBitmap("/icons/minus.png", 16, 16));
    controlsToolBar->SetToolShortHelp(ID_ControlToolFontDecrease, "Decrease font size");
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { ChangeFontSize(false); }, ID_ControlToolFontDecrease);

    controlsToolBar->AddTool(ID_ControlToolFontIncrease, "Increase", IconHelpers::LoadPngBitmap("/icons/plus.png", 16, 16));
    controlsToolBar->SetToolShortHelp(ID_ControlToolFontIncrease, "Increase font size");
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { ChangeFontSize(true); }, ID_ControlToolFontIncrease);

    controlsToolBar->Realize();
    return controlsToolBar;
}

void LoggerWindow::ChangeFontSize(bool increase)
{
    auto font = _loggerController->TextLogger()->GetFont();
    font.SetFractionalPointSize(font.GetFractionalPointSize() + (increase ? 1.0 : -1.0));
    _loggerController->TextLogger()->SetFont(font);
}

void LoggerWindow::onWindowClose(wxCloseEvent& event)
{
    Show(false);
    wxPostEvent(_parent, event);
}

}
