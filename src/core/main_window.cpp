#include "log_window.h"
#include "main_window.h"
#include "opengl_window.h"
#include "icon_helpers.h"
#include "Kredo-lighting-Config.h"

#include <wx/splitter.h>
#include <wx/panel.h>
#include <wx/toolbar.h>
#include <wx/confbase.h>

namespace Kredo
{

enum SplitterID
{
    ID_SplitterMain = wxID_HIGHEST + 1,
    ID_SplitterLast
};

enum WindowID
{
    ID_WindowLog = ID_SplitterLast + 1,
    ID_WindowMain,
    ID_WindowLast
};

enum ToolBarID
{
    ID_ToolLog = ID_WindowLast + 1,
    ID_ToolLast
};

MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(1920, 1080), wxDEFAULT_FRAME_STYLE)
    , _toolBar(new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize))
    , _mainSplitter(new wxSplitterWindow(this, ID_SplitterMain, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE))
    , _openglWindow(new OpenGLWindow(_mainSplitter, ID_WindowMain))
    , _logWindow(new LogWindow(this, ID_WindowLog))
{
    SetupWindow();
    SetupToolBar();

    wxPanel* testPanel = new wxPanel(_mainSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    testPanel->SetBackgroundColour(wxColor(255,128,128));

    _mainSplitter->SplitVertically(_openglWindow, testPanel, GetSize().GetWidth() * 0.75);

    LoadSettings();
}

MainWindow::~MainWindow()
{
    SaveSettings();
}

void MainWindow::SetupWindow()
{
    Bind(wxEVT_CREATE, &MainWindow::OnWindowCreated, this);

    SetLabel(wxString::Format("Kredo Lighting Sandbox %d.%d", KREDO_LIGHTING_VERSION_MAJOR, KREDO_LIGHTING_VERSION_MINOR));
    SetMinSize(wxSize(1024, 768));
    SetIcon(IconHelpers::LoadPngIcon("icons/kredo_logo.png"));
}

void MainWindow::SetupToolBar()
{
    _toolBar->AddCheckTool(ID_ToolLog, "Log", IconHelpers::LoadPngBitmap16("icons/notebook.png"), wxBitmapBundle(), _("Log window"));
    Bind(wxEVT_TOOL, &MainWindow::OnToolLogWindow, this, ID_ToolLog);
    Bind(wxEVT_CLOSE_WINDOW, [=](wxCloseEvent&) { _toolBar->ToggleTool(ID_ToolLog, false); }, ID_WindowLog);

    _toolBar->Realize();
    SetToolBar(_toolBar);
}

void MainWindow::OnToolLogWindow(wxCommandEvent& event)
{
    const auto tool = _toolBar->FindById(event.GetId());
    if (tool)
        _logWindow->Show(tool->IsToggled());
}

void MainWindow::OnWindowCreated(wxWindowCreateEvent& event)
{
    event.Skip();

    _mainSplitter->SetSashGravity(1.0);

    SetFocus();
}

void MainWindow::SaveSettings()
{
    wxConfigBase* config = wxConfigBase::Get();
    config->SetPath("/MainWindow");

    config->Write("Width", GetSize().GetWidth());
    config->Write("Height", GetSize().GetHeight());
    config->Write("MainSpliterSashRatio", float(_mainSplitter->GetSashPosition()) / GetSize().GetWidth());
}

void MainWindow::LoadSettings()
{
    wxConfigBase* config = wxConfigBase::Get();
    config->SetPath("/MainWindow");

    int width;
    config->Read("Width", &width, 1920);

    int height;
    config->Read("Height", &height, 1080);

    SetSize(width, height);

    float mainSplitterSashRatio;
    config->Read("MainSpliterSashRatio", &mainSplitterSashRatio, 0.75);

    _mainSplitter->SetSashPosition(GetSize().GetWidth() * mainSplitterSashRatio);
}

}
