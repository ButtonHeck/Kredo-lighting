#include "icons.h"
#include "log_window.h"
#include "main_window.h"
#include "opengl_window.h"
#include "Kredo-lighting-Config.h"

#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/toolbar.h>
#include <wx/splitter.h>
#include <wx/confbase.h>
#include <wx/aboutdlg.h>

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

enum MenuID
{
    ID_MenuExit = ID_ToolLast + 1,
    ID_MenuAbout,
    ID_MenuLast
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
    SetupMenu();

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
    SetIcon(Icons::LoadPngIcon("/icons/kredo_logo.png"));
}

void MainWindow::SetupToolBar()
{
    _toolBar->AddCheckTool(ID_ToolLog, "Log", Icons::LoadPngBitmap16("/icons/notebook.png"), wxBitmapBundle(), _("Log window"));
    Bind(wxEVT_TOOL, &MainWindow::OnToolLogWindow, this, ID_ToolLog);
    Bind(wxEVT_CLOSE_WINDOW, [=](wxCloseEvent&) { _toolBar->ToggleTool(ID_ToolLog, false); }, ID_WindowLog);

    _toolBar->Realize();
    SetToolBar(_toolBar);
}

void MainWindow::SetupMenu()
{
    wxMenu* menuFile = new wxMenu();

    menuFile->Append(ID_MenuExit, _("Exit"));
    Bind(wxEVT_MENU, [=](wxCommandEvent&) { Close(true); }, ID_MenuExit);

    wxMenu* menuAbout = new wxMenu();
    menuAbout->Append(ID_MenuAbout, _("About"));
    Bind(wxEVT_MENU, &MainWindow::OnAbout, this, ID_MenuAbout);

    wxMenuBar* menu = new wxMenuBar();
    menu->Append(menuFile, _("File"));
    menu->Append(menuAbout, _("About"));

    SetMenuBar(menu);
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

void MainWindow::OnAbout(wxCommandEvent&)
{
    wxAboutDialogInfo aboutInfo;
    aboutInfo.SetName("Kredo lighting sandbox");
    aboutInfo.SetVersion(wxString::Format(_("Version %d.%d"), KREDO_LIGHTING_VERSION_MAJOR, KREDO_LIGHTING_VERSION_MINOR));
    aboutInfo.SetDescription(_("OpenGL 4.5 demo sandbox using wxWidgets 3.3.0"));
    aboutInfo.SetCopyright("(C) 2023");
    aboutInfo.SetWebSite("https://github.com/ButtonHeck/Kredo-lighting");
    aboutInfo.AddDeveloper(_("Ilya Malgin"));
    aboutInfo.SetIcon(Icons::LoadPngIcon("/icons/kredo_logo.png"));

    wxAboutBox(aboutInfo);
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
