#include "log_controller.h"

#include <wx/textctrl.h>
#include <filesystem>

namespace Kredo
{

LogController::LogController(wxTextCtrl* textLog)
    : _textLog(textLog)
{
    const wxString wxPath = wxString::Format("%s/%s", KREDO_CONFIG_DIR, "log.txt");
    std::filesystem::path path(std::string(wxPath.mb_str()));
    std::filesystem::create_directories(path.parent_path());

    if (!_fileLog.Open(wxPath, "w"))
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


