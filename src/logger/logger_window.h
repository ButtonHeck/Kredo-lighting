#pragma once

#include <wx/frame.h>
#include <wx/log.h>

class wxToolBar;

namespace Kredo
{

class LoggerController;

class LoggerWindow : public wxFrame
{
public:
    LoggerWindow(wxWindow* parent, int id);

private:
    wxToolBar* MakeControlsToolBar(wxWindow* parent);
    void ChangeFontSize(bool increase);
    void onWindowClose(wxCloseEvent& event);

private:
    wxWindow* const _parent;
    LoggerController* const _loggerController;
};

}
