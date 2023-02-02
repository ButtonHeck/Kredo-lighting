#include "core/main_window.h"
#include "opengl/opengl_window.h"

#include <wx/splitter.h>
#include <wx/panel.h>
#include <wx/toolbar.h>
#include <wx/textctrl.h>
#include <wx/log.h>

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
    , _log(nullptr)
{
    SetupWindow();

    wxSplitterWindow* logSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE);
    _log = new wxTextCtrl(logSplitter, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    wxLog::SetActiveTarget(new wxLogTextCtrl(_log));

    wxToolBar* toolbar = CreateToolBar();
    SetToolBar(toolbar);



    wxSplitterWindow* mainSplitter = new wxSplitterWindow(logSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE);

    _openglWindow = new OpenGLWindow(mainSplitter);

    wxPanel* testPanel = new wxPanel(mainSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    testPanel->SetBackgroundColour(wxColor(255,128,128));

    mainSplitter->SetSashGravity(0.5);
    mainSplitter->SplitVertically(_openglWindow, testPanel);



    logSplitter->SetSashGravity(0.8);
    logSplitter->SplitHorizontally(mainSplitter, _log);
}

void MainWindow::SetupWindow()
{
    SetMinSize(wxSize(1024, 768));
}

wxToolBar* MainWindow::CreateToolBar()
{
    wxToolBar* toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_DEFAULT_STYLE);
    toolbar->SetToolSeparation(20);
    toolbar->SetToolPacking(2);

    toolbar->AddCheckTool(ID_ToolLog, "Log", log_xpm, wxBitmapBundle(), "Log");
    Bind(wxEVT_TOOL, [=](wxCommandEvent& e) {
        const auto tool = toolbar->FindById(e.GetId());
        if (tool)
            tool->IsToggled() ? _log->Hide() : _log->Show();
    }, ID_ToolLog);

    toolbar->Realize();
    return toolbar;
}

}
