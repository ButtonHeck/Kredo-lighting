#include "core/logger_controller.h"
#include "core/main_window.h"
#include "opengl/opengl_window.h"

#include <wx/splitter.h>
#include <wx/panel.h>
#include <wx/toolbar.h>
#include <wx/textctrl.h>

#include <wx/wx.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
#include "rc/log.xpm"
#pragma GCC diagnostic push

namespace Kredo
{

enum ToolBarID
{
    ID_ToolLog = wxID_HIGHEST + 1,
};

MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, "Kredo Lighting Sandbox", wxDefaultPosition, wxSize(1920, 1080), wxDEFAULT_FRAME_STYLE | wxSTAY_ON_TOP)
    , _openglWindow(nullptr)
    , _logger(nullptr)
    , _toolBar(nullptr)
    , _logSplitter(new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE))
    , _mainSplitter(new wxSplitterWindow(_logSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE))
{
    SetupWindow();

    _logger = new LoggerController(new wxTextCtrl(_logSplitter, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY), "log.txt");
    wxLog::SetActiveTarget(_logger);

    SetupToolBar();


    _openglWindow = new OpenGLWindow(_mainSplitter);

    wxPanel* testPanel = new wxPanel(_mainSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    testPanel->SetBackgroundColour(wxColor(255,128,128));

    _mainSplitter->SetSashGravity(0.5);
    _mainSplitter->SplitVertically(_openglWindow, testPanel);

    _logSplitter->SetSashGravity(0.8);
    _logSplitter->SplitHorizontally(_mainSplitter, _logger->TextLogger());
}

void MainWindow::SetupWindow()
{
    SetMinSize(wxSize(1024, 768));
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
            _logSplitter->SplitHorizontally(_mainSplitter, _logger->TextLogger());
        else
            _logSplitter->Unsplit();

        _logSplitter->SetSashPosition(GetClientSize().GetHeight() * GetContentScaleFactor() * 0.8, true);
        _logSplitter->Update();
    }
}

}
