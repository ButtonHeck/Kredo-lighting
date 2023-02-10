#include "log_controller.h"

#include <wx/textctrl.h>

namespace Kredo
{

LogController::LogController(wxTextCtrl* textLog)
    : _textLog(textLog)
{
    if (!_fileLog.Open(wxString::Format("%s/%s", KREDO_CONFIG_DIR, "log.txt"), "w"))
        wxLogWarning("Could not open file logger");
}

wxTextCtrl* LogController::TextLog() const
{
    return _textLog;
}

void LogController::Clear()
{
    _textLog->Clear();
}

void LogController::DoLogTextAtLevel(wxLogLevel level, const wxString& msg)
{
    WXUNUSED(level)

    _textLog->AppendText(msg + '\n');

    if (_fileLog.IsOpened())
        _fileLog.Write(msg + '\n');
}

}


