#include "logger_controller.h"

#include <wx/textctrl.h>

namespace Kredo
{

LoggerController::LoggerController(wxTextCtrl* textLogger, const wxString& fileLogName)
    : _textLogger(textLogger)
    , _fileLogger(fileLogName, "w")
{
}

wxTextCtrl* LoggerController::TextLogger() const
{
    return _textLogger;
}

void LoggerController::DoLogTextAtLevel(wxLogLevel level, const wxString& msg)
{
    WXUNUSED(level)

    _textLogger->AppendText(msg + '\n');

    if (_fileLogger.IsOpened())
        _fileLogger.Write(msg + '\n');
}

}


