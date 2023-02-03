#pragma once

#include <wx/log.h>
#include <wx/ffile.h>

class wxTextCtrl;

namespace Kredo
{

class LoggerController : public wxLog
{
public:
    LoggerController(wxTextCtrl* textLogger, const wxString& fileLogName);

    wxTextCtrl* TextLogger() const;

protected:
    void DoLogTextAtLevel(wxLogLevel level, const wxString& msg);

private:
    wxTextCtrl* const _textLogger;
    wxFFile _fileLogger;
};

}
