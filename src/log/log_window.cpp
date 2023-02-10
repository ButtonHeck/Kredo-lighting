#include "log_window.h"
#include "log_controller.h"
#include "icon_helpers.h"

#include <wx/font.h>
#include <wx/sizer.h>
#include <wx/toolbar.h>
#include <wx/textctrl.h>
#include <wx/confbase.h>

namespace Kredo
{

enum ControlToolID
{
    ID_ControlToolClear = wxID_HIGHEST + 1,
    ID_ControlToolFontDecrease,
    ID_ControlToolFontIncrease,
    ID_ControlToolLast
};

LogWindow::LogWindow(wxWindow* parent, int id)
    : wxFrame(parent, id, "Log window", wxDefaultPosition, wxSize(480, 240))
    , _parent(parent)
    , _logController(new LogController(new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY)))
{
    wxLog::SetActiveTarget(_logController);

    const auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(MakeControlsToolBar(this), 0);
    sizer->Add(_logController->TextLog(), 1, wxEXPAND);

    SetMinSize(wxSize(480, 240));
    SetSizer(sizer);

    Bind(wxEVT_CLOSE_WINDOW, &LogWindow::onWindowClose, this);

    LoadSettings();
}

LogWindow::~LogWindow()
{
    SaveSettings();
}

wxToolBar* LogWindow::MakeControlsToolBar(wxWindow* parent)
{
    auto controlsToolBar = new wxToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);

    controlsToolBar->AddTool(ID_ControlToolClear, "Clear", IconHelpers::LoadPngBitmap16("/icons/broom.png"));
    controlsToolBar->SetToolShortHelp(ID_ControlToolClear, "Clear");
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { _logController->Clear(); }, ID_ControlToolClear);

    controlsToolBar->AddTool(ID_ControlToolFontDecrease, "Decrease", IconHelpers::LoadPngBitmap16("/icons/minus.png"));
    controlsToolBar->SetToolShortHelp(ID_ControlToolFontDecrease, "Decrease font size");
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { ChangeFontSize(false); }, ID_ControlToolFontDecrease);

    controlsToolBar->AddTool(ID_ControlToolFontIncrease, "Increase", IconHelpers::LoadPngBitmap16("/icons/plus.png"));
    controlsToolBar->SetToolShortHelp(ID_ControlToolFontIncrease, "Increase font size");
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { ChangeFontSize(true); }, ID_ControlToolFontIncrease);

    controlsToolBar->Realize();
    return controlsToolBar;
}

void LogWindow::ChangeFontSize(bool increase)
{
    auto font = _logController->TextLog()->GetFont();
    font.SetFractionalPointSize(font.GetFractionalPointSize() + (increase ? 1.0 : -1.0));
    _logController->TextLog()->SetFont(font);
}

void LogWindow::onWindowClose(wxCloseEvent& event)
{
    Show(false);
    wxPostEvent(_parent, event);
}

void LogWindow::SaveSettings()
{
    wxConfigBase* config = wxConfigBase::Get();
    config->SetPath("/LogWindow");

    config->Write("Width", GetSize().GetWidth());
    config->Write("Height", GetSize().GetHeight());
}

void LogWindow::LoadSettings()
{
    wxConfigBase* config = wxConfigBase::Get();
    config->SetPath("/LogWindow");

    int width;
    config->Read("Width", &width, 480);

    int height;
    config->Read("Height", &height, 240);

    SetSize(width, height);
}

}
