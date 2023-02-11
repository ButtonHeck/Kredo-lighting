#pragma once

#include <wx/frame.h>

class wxToolBar;
class wxSpinCtrl;

namespace Kredo
{

class LogController;

class LogWindow : public wxFrame
{
public:
    LogWindow(wxWindow* parent, int id);
    ~LogWindow();

private:
    void SetupToolBar();
    void DefaultState();
    void AddMessageFilterTool(int id, const char* name, const char* help, const char* iconPath, int level);

    void onWindowClose(wxCloseEvent& event);
    void onWindowShown(wxShowEvent& event);
    void OnToolAlwaysOnTop(wxCommandEvent& event);
    void OnToolTransparency(wxCommandEvent& event);
    void OnToolOpacitySpin(wxSpinEvent& event);

    void SaveSettings();
    void LoadSettings();

private:
    wxWindow* const _parent;
    wxToolBar* const _toolBar;
    LogController* const _logController;
    wxSpinCtrl* const _opacitySpin;
};

}
