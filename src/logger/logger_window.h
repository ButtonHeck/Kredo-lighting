#pragma once

#include <wx/window.h>
#include <wx/log.h>

class wxToolBar;

namespace Kredo
{

class LoggerController;

class LoggerWindow : public wxWindow
{
public:
    LoggerWindow(wxWindow* parent);

private:
    wxToolBar* MakeControlsToolBar(wxWindow* parent);
    void ChangeFontSize(bool increase);

private:
    LoggerController* _loggerController;
};

}
