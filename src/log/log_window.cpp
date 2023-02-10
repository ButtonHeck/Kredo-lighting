#include "log_window.h"
#include "icon_helpers.h"
#include "log_controller.h"

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
    : wxFrame(parent, id, "Log window", wxDefaultPosition, wxSize(800, 400))
    , _parent(parent)
    , _logController(new LogController(this))
{
    wxLog::SetActiveTarget(_logController);

    const auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(MakeControlsToolBar(this), 0);
    sizer->Add(_logController->TextLog(), 1, wxEXPAND);

    SetMinSize(wxSize(800, 400));
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
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { _logController->ChangeFontSize(false); }, ID_ControlToolFontDecrease);

    controlsToolBar->AddTool(ID_ControlToolFontIncrease, "Increase", IconHelpers::LoadPngBitmap16("/icons/plus.png"));
    controlsToolBar->SetToolShortHelp(ID_ControlToolFontIncrease, "Increase font size");
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { _logController->ChangeFontSize(true); }, ID_ControlToolFontIncrease);

    controlsToolBar->Realize();
    return controlsToolBar;
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
    config->Read("Width", &width, 800);

    int height;
    config->Read("Height", &height, 400);

    SetSize(width, height);
}

}
