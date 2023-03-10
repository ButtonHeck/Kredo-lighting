#include "icons.h"
#include "log_window.h"
#include "log_controller.h"

#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/toolbar.h>
#include <wx/confbase.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>

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

    ID_ToolMove,
    ID_ToolResize,

    ID_ToolLast
};

LogWindow::LogWindow(wxWindow* parent, int id)
    : wxFrame(parent, id, _("Log window"), wxDefaultPosition, wxSize(800, 400), wxSIMPLE_BORDER | wxFRAME_NO_TASKBAR)
    , _parent(parent)
    , _logController(new LogController(this))
    , _toolBar(new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize))
    , _opacitySpin(new wxSpinCtrl(_toolBar, ID_ToolOpacitySpin, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                  wxSP_ARROW_KEYS, 10, 100, 100))
    , _mouseTrack(0, 0)
{
    SetMinSize(wxSize(800, 400));
    SetTransparent(255);

    SetupToolBar();
    SetupLayout();

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
    _toolBar->AddTool(ID_ToolClear, "Clear", Icons::LoadPngBitmap16("icons/clear.png"));
    _toolBar->SetToolShortHelp(ID_ToolClear, _("Clear"));
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { _logController->Clear(); }, ID_ToolClear);

    _toolBar->AddTool(ID_ToolFontDecrease, "Decrease", Icons::LoadPngBitmap16("icons/minus.png"));
    _toolBar->SetToolShortHelp(ID_ToolFontDecrease, _("Decrease font size"));
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { _logController->ChangeFontSize(false); }, ID_ToolFontDecrease);

    _toolBar->AddTool(ID_ToolFontIncrease, "Increase", Icons::LoadPngBitmap16("icons/plus.png"));
    _toolBar->SetToolShortHelp(ID_ToolFontIncrease, _("Increase font size"));
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { _logController->ChangeFontSize(true); }, ID_ToolFontIncrease);

    _toolBar->AddStretchableSpace();
    AddMessageFilterTool(ID_ToolLogError, "Log error", _("Log error enabled"), "icons/error.png", wxLOG_Error);
    AddMessageFilterTool(ID_ToolLogWarning, "Log warning", _("Log warning enabled"), "icons/warning.png", wxLOG_Warning);
    AddMessageFilterTool(ID_ToolLogMessage, "Log message", _("Log message enabled"), "icons/message.png", wxLOG_Message);
    AddMessageFilterTool(ID_ToolLogInfo, "Log info", _("Log info enabled"), "icons/info.png", wxLOG_Info);
    AddMessageFilterTool(ID_ToolLogDebug, "Log debug", _("Log debug enabled"), "icons/bug.png", wxLOG_Debug);
    _toolBar->AddStretchableSpace();

    _toolBar->AddCheckTool(ID_ToolTransparent, "Toggle transparency", Icons::LoadPngBitmap16("icons/transparency.png"));
    _toolBar->SetToolShortHelp(ID_ToolTransparent, _("Toggle transparency"));
    Bind(wxEVT_TOOL, &LogWindow::OnToolTransparency, this, ID_ToolTransparent);

    _opacitySpin->Bind(wxEVT_SPINCTRL, &LogWindow::OnToolOpacitySpin, this);
    _toolBar->AddControl(_opacitySpin, "Opacity");

    _toolBar->AddCheckTool(ID_ToolAlwaysOnTop, "Always on top", Icons::LoadPngBitmap16("icons/thumbtack.png"));
    _toolBar->SetToolShortHelp(ID_ToolAlwaysOnTop, _("Always on top"));
    Bind(wxEVT_TOOL, &LogWindow::OnToolAlwaysOnTop, this, ID_ToolAlwaysOnTop);

    _toolBar->AddTool(ID_ToolClose, "Close", Icons::LoadPngBitmap16("icons/close.png"));
    _toolBar->SetToolShortHelp(ID_ToolClose, _("Close"));
    Bind(wxEVT_TOOL, [=](wxCommandEvent&) { Close(); }, ID_ToolClose);

    _toolBar->Realize();
    SetToolBar(_toolBar);
}

void LogWindow::SetupLayout()
{
    const auto moveButton = new wxButton(this, ID_ToolMove, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_NOTEXT | wxBU_EXACTFIT | wxBORDER_NONE);
    moveButton->SetBitmap(Icons::LoadPngBitmap16("icons/move.png"));
    moveButton->SetToolTip(_("Move"));
    moveButton->Bind(wxEVT_LEFT_DOWN, &LogWindow::OnMouseDown, this);
    moveButton->Bind(wxEVT_MOTION, &LogWindow::OnButtonMove, this);

    const auto resizeButton = new wxButton(this, ID_ToolResize, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_NOTEXT | wxBU_EXACTFIT | wxBORDER_NONE);
    resizeButton->SetBitmap(Icons::LoadPngBitmap16("icons/resize.png"));
    resizeButton->SetToolTip(_("Resize"));
    resizeButton->Bind(wxEVT_LEFT_DOWN, &LogWindow::OnMouseDown, this);
    resizeButton->Bind(wxEVT_MOTION, &LogWindow::OnButtonResize, this);

    const auto bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    bottomSizer->Add(moveButton, 0);
    bottomSizer->Add(resizeButton, 0);

    const auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(_logController->TextLog(), 1, wxEXPAND);
    sizer->Add(bottomSizer, 0, wxALIGN_RIGHT);
    SetSizer(sizer);
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
    _toolBar->AddCheckTool(id, name, Icons::LoadPngBitmap16(iconPath));
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

void LogWindow::OnButtonMove(wxMouseEvent& event)
{
    if (!event.Dragging())
        return;

    const auto currentMousePos = wxGetMousePosition();
    SetPosition(GetPosition() + wxPoint(currentMousePos.x - _mouseTrack.x, currentMousePos.y - _mouseTrack.y));
    _mouseTrack = currentMousePos;
}

void LogWindow::OnButtonResize(wxMouseEvent& event)
{
    if (!event.Dragging())
        return;

    const auto currentMousePos = wxGetMousePosition();
    SetSize(GetSize().x + currentMousePos.x - _mouseTrack.x, GetSize().y + currentMousePos.y - _mouseTrack.y);
    _mouseTrack = currentMousePos;
}

void LogWindow::OnMouseDown(wxMouseEvent& event)
{
    WXUNUSED(event)
    wxGetMousePosition(&_mouseTrack.x, &_mouseTrack.y);
}

void LogWindow::SaveSettings()
{
    const auto config = wxConfigBase::Get();
    config->SetPath("/LogWindow");

    config->Write("Width", GetSize().GetWidth());
    config->Write("Height", GetSize().GetHeight());
    config->Write("Opacity", _opacitySpin->GetValue());
}

void LogWindow::LoadSettings()
{
    const auto config = wxConfigBase::Get();
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
