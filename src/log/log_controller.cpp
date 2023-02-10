#include "log_formatter.h"
#include "log_controller.h"

#include <wx/textctrl.h>
#include <wx/font.h>

namespace Kredo
{

LogController::LogController(wxWindow* window)
    : _textLog(new wxTextCtrl(window, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_DONTWRAP))
{
    if (!_fileLog.Open(wxString::Format("%s/%s", KREDO_CONFIG_DIR, "log.txt"), "w"))
        wxLogWarning("Could not open file logger");

    SetFormatter(new LogFormatter);

    auto font = _textLog->GetFont();
    font.SetFamily(wxFONTFAMILY_TELETYPE);
    _textLog->SetFont(font);
}

wxTextCtrl* LogController::TextLog() const
{
    return _textLog;
}

void LogController::Clear()
{
    _textLog->Clear();
}

void LogController::ChangeFontSize(bool increase)
{
    auto font = _textLog->GetFont();
    font.SetFractionalPointSize(font.GetFractionalPointSize() + (increase ? 1.0 : -1.0));
    _textLog->SetFont(font);
}

void LogController::DoLogTextAtLevel(wxLogLevel level, const wxString& msg)
{
    WXUNUSED(level)

    _textLog->AppendText(msg + '\n');

    if (_fileLog.IsOpened())
        _fileLog.Write(msg + '\n');
}

}


