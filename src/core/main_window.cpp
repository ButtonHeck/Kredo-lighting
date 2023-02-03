#include "logger/logger_window.h"
#include "core/main_window.h"
#include "opengl/opengl_window.h"

#include <wx/splitter.h>
#include <wx/panel.h>
#include <wx/toolbar.h>

#include <wx/wx.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
#include "rc/log.xpm"
#include "rc/kredo_logo.xpm"
#pragma GCC diagnostic push

namespace Kredo
{

enum ToolBarID
{
    ID_ToolLog = wxID_HIGHEST + 1,
};

MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, "Kredo Lighting Sandbox", wxDefaultPosition, wxSize(1920, 1080), wxDEFAULT_FRAME_STYLE)
    , _logSplitter(new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE | wxSP_PERMIT_UNSPLIT))
    , _mainSplitter(new wxSplitterWindow(_logSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE))
    , _openglWindow(new OpenGLWindow(_mainSplitter))
    , _loggerWindow(new LoggerWindow(_logSplitter))
    , _toolBar(nullptr)
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

    SetMinSize(wxSize(1024, 768));
    SetIcon(kredo_logo_xpm);
}

void MainWindow::SetupToolBar()
{
    _toolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_DEFAULT_STYLE);
    _toolBar->AddCheckTool(ID_ToolLog, "Log", log_xpm, wxBitmapBundle(), "Log");

    _toolBar->Realize();
    SetToolBar(_toolBar);

    Bind(wxEVT_TOOL, &MainWindow::ToggleLogWindow, this, ID_ToolLog);
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
    SetFocus();
}

}
