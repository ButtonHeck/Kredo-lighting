#include "logger_controller.h"

#include <wx/textctrl.h>
#include <filesystem>

namespace Kredo
{

LoggerController::LoggerController(wxTextCtrl* textLogger)
    : _textLogger(textLogger)
{
    const wxString wxPath = wxString::Format("%s/%s", KREDO_CONFIG_DIR, "log.txt");
    std::filesystem::path path(std::string(wxPath.mb_str()));
    std::filesystem::create_directories(path.parent_path());

    if (!_fileLogger.Open(wxPath, "w"))
        wxLogWarning("Could not open file logger");
}

wxTextCtrl* LoggerController::TextLogger() const
{
    return _textLogger;
}

void LoggerController::Clear()
{
    _textLogger->Clear();
}

void LoggerController::DoLogTextAtLevel(wxLogLevel level, const wxString& msg)
{
    WXUNUSED(level)

    _textLogger->AppendText(msg + '\n');

    if (_fileLogger.IsOpened())
        _fileLogger.Write(msg + '\n');
}

}


