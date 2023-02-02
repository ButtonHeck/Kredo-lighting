#pragma once

#include <wx/frame.h>

class wxTextCtrl;

namespace Kredo
{

class OpenGLWindow;

class MainWindow : public wxFrame
{
public:
    MainWindow();

private:
    void SetupWindow();
    wxToolBar* CreateToolBar();

private:
    OpenGLWindow* _openglWindow;
    wxTextCtrl* _log;
};

}
