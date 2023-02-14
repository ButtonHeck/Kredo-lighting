#pragma once

#include <wx/frame.h>

class wxTextCtrl;
class wxSplitterWindow;

namespace Kredo
{

class OpenGLWindow;
class LogWindow;

class MainWindow : public wxFrame
{
public:
    MainWindow();
    ~MainWindow();

private:
    void SetupWindow();
    void SetupToolBar();
    void SetupMenu();

    void OnToolLogWindow(wxCommandEvent& event);
    void OnWindowCreated(wxWindowCreateEvent& event);
    void OnAbout(wxCommandEvent& event);

private:
    void SaveSettings();
    void LoadSettings();

private:
    wxToolBar* const _toolBar;

    wxSplitterWindow* const _mainSplitter;

    OpenGLWindow* const _openglWindow;
    LogWindow* const _logWindow;
};

}
