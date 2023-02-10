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
    wxToolBar* MakeControlsToolBar(wxWindow* parent);
    void onWindowClose(wxCloseEvent& event);

    void SaveSettings();
    void LoadSettings();

private:
    wxWindow* const _parent;
    LogController* const _logController;
};

}
