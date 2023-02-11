#include "log_window.h"
#include "icon_helpers.h"
#include "log_controller.h"

#include <wx/toolbar.h>
#include <wx/confbase.h>

namespace Kredo
{

enum ToolID
{
    ID_ToolClear = wxID_HIGHEST + 1,
    ID_ToolFontDecrease,
    ID_ToolFontIncrease,
    ID_ToolAlwaysOnTop,
    ID_ToolLast
};

LogWindow::LogWindow(wxWindow* parent, int id)
    : wxFrame(parent, id, "Log window", wxDefaultPosition, wxSize(800, 400))
    , _parent(parent)
    , _logController(new LogController(this))
    , _toolBar(new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_DEFAULT_STYLE))
{
    SetMinSize(wxSize(800, 400));
    SetupToolBar();

    Bind(wxEVT_CLOSE_WINDOW, &LogWindow::onWindowClose, this);
    Bind(wxEVT_SHOW, &LogWindow::onWindowShown, this);

    LoadSettings();
}

LogWindow::~LogWindow()
{
    SaveSettings();
}

void LogWindow::SetupToolBar()
{
    _toolBar->AddTool(ID_ToolClear, "Clear", IconHelpers::LoadPngBitmap16("/icons/broom.png"));
    _toolBar->SetToolShortHelp(ID_ToolClear, "Clear");
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { _logController->Clear(); }, ID_ToolClear);

    _toolBar->AddTool(ID_ToolFontDecrease, "Decrease", IconHelpers::LoadPngBitmap16("/icons/minus.png"));
    _toolBar->SetToolShortHelp(ID_ToolFontDecrease, "Decrease font size");
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { _logController->ChangeFontSize(false); }, ID_ToolFontDecrease);

    _toolBar->AddTool(ID_ToolFontIncrease, "Increase", IconHelpers::LoadPngBitmap16("/icons/plus.png"));
    _toolBar->SetToolShortHelp(ID_ToolFontIncrease, "Increase font size");
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { _logController->ChangeFontSize(true); }, ID_ToolFontIncrease);

    _toolBar->AddStretchableSpace();

    _toolBar->AddCheckTool(ID_ToolAlwaysOnTop, "Always on top", IconHelpers::LoadPngBitmap16("/icons/thumbtack.png"));
    _toolBar->SetToolShortHelp(ID_ToolAlwaysOnTop, "Always on top");
    Bind(wxEVT_TOOL, &LogWindow::OnToolAlwaysOnTop, this, ID_ToolAlwaysOnTop);

    _toolBar->Realize();
    SetToolBar(_toolBar);
}

void LogWindow::onWindowClose(wxCloseEvent& event)
{
    _toolBar->ToggleTool(ID_ToolAlwaysOnTop, false);
    SetWindowStyleFlag(wxDEFAULT_FRAME_STYLE);
    Show(false);
    wxPostEvent(_parent, event);
}

void LogWindow::onWindowShown(wxShowEvent& event)
{
    WXUNUSED(event)
    _toolBar->ToggleTool(ID_ToolAlwaysOnTop, false);
    SetWindowStyleFlag(wxDEFAULT_FRAME_STYLE);
}

void LogWindow::OnToolAlwaysOnTop(wxCommandEvent& event)
{
    const auto tool = _toolBar->FindById(event.GetId());
    if (tool)
        SetWindowStyleFlag(tool->IsToggled() ? wxDEFAULT_FRAME_STYLE | wxSTAY_ON_TOP : wxDEFAULT_FRAME_STYLE);
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
