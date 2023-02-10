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
    void ToggleLogWindow(wxCommandEvent& event);
    void OnWindowCreated(wxWindowCreateEvent& event);

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
