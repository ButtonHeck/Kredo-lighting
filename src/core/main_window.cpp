#include "logger_window.h"
#include "main_window.h"
#include "opengl_window.h"
#include "icon_helpers.h"
#include "Kredo-lighting-Config.h"

#include <wx/splitter.h>
#include <wx/panel.h>
#include <wx/toolbar.h>

namespace Kredo
{

enum SplitterID
{
    ID_SplitterLog = wxID_HIGHEST + 1,
    ID_SplitterMain,
    ID_SplitterLast
};

enum ToolBarID
{
    ID_ToolLog = ID_SplitterLast + 1,
    ID_ToolLast
};

MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(1920, 1080), wxDEFAULT_FRAME_STYLE)
    , _toolBar(new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize))
    , _logSplitter(new wxSplitterWindow(this, ID_SplitterLog, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE | wxSP_PERMIT_UNSPLIT))
    , _mainSplitter(new wxSplitterWindow(_logSplitter, ID_SplitterMain, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE))
    , _openglWindow(new OpenGLWindow(_mainSplitter))
    , _loggerWindow(new LoggerWindow(_logSplitter))
{
    SetupWindow();
    SetupToolBar();

    wxPanel* testPanel = new wxPanel(_mainSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    testPanel->SetBackgroundColour(wxColor(255,128,128));

    _mainSplitter->SplitVertically(_openglWindow, testPanel, GetSize().GetWidth() * 0.75);
    _logSplitter->SplitHorizontally(_mainSplitter, _loggerWindow, GetSize().GetHeight() * GetContentScaleFactor() * 0.75);
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
    _toolBar->AddCheckTool(ID_ToolLog, "Log", IconHelpers::LoadPngBitmap("icons/notebook.png", 16, 16), wxBitmapBundle(), "Log");

    _toolBar->Realize();
    SetToolBar(_toolBar);

    Bind(wxEVT_TOOL, &MainWindow::ToggleLogWindow, this, ID_ToolLog);
    Bind(wxEVT_SPLITTER_UNSPLIT, [=](wxCommandEvent&) { _toolBar->ToggleTool(ID_ToolLog, false); }, ID_SplitterLog);
}

void MainWindow::ToggleLogWindow(wxCommandEvent& event)
{
    const auto tool = _toolBar->FindById(event.GetId());
    if (tool)
    {
        if (tool->IsToggled())
            _logSplitter->SplitHorizontally(_mainSplitter, _loggerWindow, GetSize().GetHeight() * GetContentScaleFactor() * 0.75);
        else
            _logSplitter->Unsplit();

        _logSplitter->Update();
    }
}

void MainWindow::OnWindowCreated(wxWindowCreateEvent& event)
{
    event.Skip();

    _mainSplitter->SetSashGravity(1.0);
    _logSplitter->SetSashGravity(1.0);

    _toolBar->ToggleTool(ID_ToolLog, _logSplitter->IsSplit());

    SetFocus();
}

}
