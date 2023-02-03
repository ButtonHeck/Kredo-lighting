#pragma once

#include <wx/frame.h>

class wxTextCtrl;
class wxSplitterWindow;

namespace Kredo
{

class OpenGLWindow;
class LoggerWindow;

class MainWindow : public wxFrame
{
public:
    MainWindow();

private:
    void SetupWindow();
    void SetupToolBar();
    void ToggleLogWindow(wxCommandEvent& event);
    void OnWindowCreated(wxWindowCreateEvent& event);

private:
    wxSplitterWindow* const _logSplitter;
    wxSplitterWindow* const _mainSplitter;

    OpenGLWindow* const _openglWindow;
    LoggerWindow* const _loggerWindow;

    wxToolBar* _toolBar;
};

}
