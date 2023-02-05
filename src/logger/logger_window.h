#pragma once

#include <wx/window.h>
#include <wx/log.h>

namespace Kredo
{

class LoggerController;

class LoggerWindow : public wxWindow
{
public:
    LoggerWindow(wxWindow* parent);

private:
    LoggerController* _loggerController;
};

}
