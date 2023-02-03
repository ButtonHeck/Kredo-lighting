#pragma once

#include <wx/frame.h>

class wxTextCtrl;
class wxSplitterWindow;

namespace Kredo
{

class OpenGLWindow;
class LoggerController;

class MainWindow : public wxFrame
{
public:
    MainWindow();

private:
    void SetupWindow();
    void SetupToolBar();
    void ToggleLogWindow(wxCommandEvent& event);

private:
    OpenGLWindow* _openglWindow;
    LoggerController* _logger;
    wxToolBar* _toolBar;
    wxSplitterWindow* _logSplitter;
    wxSplitterWindow* _mainSplitter;
};

}
