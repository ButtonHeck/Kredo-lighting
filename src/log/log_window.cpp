#include "log_window.h"
#include "icon_helpers.h"
#include "log_controller.h"

#include <wx/toolbar.h>
#include <wx/confbase.h>
#include <wx/spinctrl.h>

namespace Kredo
{

enum ToolID
{
    ID_ToolClear = wxID_HIGHEST + 1,
    ID_ToolFontDecrease,
    ID_ToolFontIncrease,

    ID_ToolLogError,
    ID_ToolLogWarning,
    ID_ToolLogMessage,
    ID_ToolLogInfo,
    ID_ToolLogDebug,

    ID_ToolTransparent,
    ID_ToolOpacitySpin,
    ID_ToolAlwaysOnTop,
    ID_ToolClose,
    ID_ToolLast
};

LogWindow::LogWindow(wxWindow* parent, int id)
    : wxFrame(parent, id, "Log window", wxDefaultPosition, wxSize(800, 400), wxFRAME_NO_TASKBAR)
    , _parent(parent)
    , _logController(new LogController(this))
    , _toolBar(new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize))
    , _opacitySpin(new wxSpinCtrl(_toolBar, ID_ToolOpacitySpin, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                  wxSP_ARROW_KEYS, 10, 100, 100))
{
    SetMinSize(wxSize(800, 400));
    SetTransparent(255);
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
    _toolBar->AddTool(ID_ToolClear, "Clear", IconHelpers::LoadPngBitmap16("/icons/clear.png"));
    _toolBar->SetToolShortHelp(ID_ToolClear, "Clear");
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { _logController->Clear(); }, ID_ToolClear);

    _toolBar->AddTool(ID_ToolFontDecrease, "Decrease", IconHelpers::LoadPngBitmap16("/icons/minus.png"));
    _toolBar->SetToolShortHelp(ID_ToolFontDecrease, "Decrease font size");
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { _logController->ChangeFontSize(false); }, ID_ToolFontDecrease);

    _toolBar->AddTool(ID_ToolFontIncrease, "Increase", IconHelpers::LoadPngBitmap16("/icons/plus.png"));
    _toolBar->SetToolShortHelp(ID_ToolFontIncrease, "Increase font size");
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { _logController->ChangeFontSize(true); }, ID_ToolFontIncrease);

    _toolBar->AddStretchableSpace();

    AddMessageFilterTool(ID_ToolLogError, "Log error", "Log error enabled", "icons/error.png", wxLOG_Error);
    AddMessageFilterTool(ID_ToolLogWarning, "Log warning", "Log warning enabled", "icons/warning.png", wxLOG_Warning);
    AddMessageFilterTool(ID_ToolLogMessage, "Log message", "Log message enabled", "icons/message.png", wxLOG_Message);
    AddMessageFilterTool(ID_ToolLogInfo, "Log info", "Log info enabled", "icons/info.png", wxLOG_Info);
    AddMessageFilterTool(ID_ToolLogDebug, "Log debug", "Log debug enabled", "icons/bug.png", wxLOG_Debug);

    _toolBar->AddStretchableSpace();

    _toolBar->AddCheckTool(ID_ToolTransparent, "Toggle transparency", IconHelpers::LoadPngBitmap16("icons/transparency.png"));
    _toolBar->SetToolShortHelp(ID_ToolTransparent, "Toggle transparency");
    Bind(wxEVT_TOOL, &LogWindow::OnToolTransparency, this, ID_ToolTransparent);

    _opacitySpin->Bind(wxEVT_SPINCTRL, &LogWindow::OnToolOpacitySpin, this);
    _toolBar->AddControl(_opacitySpin, "Opacity");

    _toolBar->AddCheckTool(ID_ToolAlwaysOnTop, "Always on top", IconHelpers::LoadPngBitmap16("/icons/thumbtack.png"));
    _toolBar->SetToolShortHelp(ID_ToolAlwaysOnTop, "Always on top");
    Bind(wxEVT_TOOL, &LogWindow::OnToolAlwaysOnTop, this, ID_ToolAlwaysOnTop);

    _toolBar->AddTool(ID_ToolClose, "Close", IconHelpers::LoadPngBitmap16("/icons/close.png"));
    _toolBar->SetToolShortHelp(ID_ToolClose, "Close");
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { Close(); }, ID_ToolClose);

    _toolBar->Realize();
    SetToolBar(_toolBar);
}

void LogWindow::DefaultState()
{
    _toolBar->ToggleTool(ID_ToolAlwaysOnTop, false);
    _toolBar->ToggleTool(ID_ToolTransparent, false);
    SetTransparent(255);
    SetWindowStyleFlag(wxDEFAULT_FRAME_STYLE);
}

void LogWindow::AddMessageFilterTool(int id, const char* name, const char* help, const char* iconPath, int level)
{
    _toolBar->AddCheckTool(id, name, IconHelpers::LoadPngBitmap16(iconPath));
    _toolBar->ToggleTool(id, true);
    _toolBar->SetToolShortHelp(id, help);
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) {
        const auto tool = _toolBar->FindById(id);
        if (tool)
            _logController->SetLogLevelActive(level, tool->IsToggled());
    }, id);
}

void LogWindow::onWindowClose(wxCloseEvent& event)
{
    DefaultState();
    Show(false);
    wxPostEvent(_parent, event);
}

void LogWindow::onWindowShown(wxShowEvent& event)
{
    WXUNUSED(event)
    DefaultState();
}

void LogWindow::OnToolAlwaysOnTop(wxCommandEvent& event)
{
    const auto tool = _toolBar->FindById(event.GetId());
    if (tool)
        SetWindowStyleFlag(tool->IsToggled() ? wxDEFAULT_FRAME_STYLE | wxSTAY_ON_TOP : wxDEFAULT_FRAME_STYLE);
}

void LogWindow::OnToolTransparency(wxCommandEvent& event)
{
    const auto tool = _toolBar->FindById(event.GetId());
    if (tool)
        SetTransparent(tool->IsToggled() ? _opacitySpin->GetValue() * 255.0 / 100.0 : 255);
}

void LogWindow::OnToolOpacitySpin(wxSpinEvent& event)
{
    const auto transparentTool = _toolBar->FindById(ID_ToolTransparent);
    if (transparentTool && transparentTool->IsToggled())
        SetTransparent(event.GetValue() * 255.0 / 100.0);
}

void LogWindow::SaveSettings()
{
    wxConfigBase* config = wxConfigBase::Get();
    config->SetPath("/LogWindow");

    config->Write("Width", GetSize().GetWidth());
    config->Write("Height", GetSize().GetHeight());
    config->Write("Opacity", _opacitySpin->GetValue());
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

    int opacity;
    config->Read("Opacity", &opacity, 100);
    _opacitySpin->SetValue(opacity);
}

}
