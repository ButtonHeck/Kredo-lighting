#pragma once

#include <wx/frame.h>
#include <wx/log.h>

class wxToolBar;

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

    void onWindowClose(wxCloseEvent& event);
    void onWindowShown(wxShowEvent& event);
    void OnToolAlwaysOnTop(wxCommandEvent& event);

    void SaveSettings();
    void LoadSettings();

private:
    wxWindow* const _parent;
    wxToolBar* const _toolBar;
    LogController* const _logController;
};

}
