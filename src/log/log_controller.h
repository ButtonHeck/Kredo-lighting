#pragma once

#include <wx/log.h>
#include <wx/ffile.h>

class wxTextCtrl;

namespace Kredo
{

class LogController : public wxLog
{
public:
    LogController(wxTextCtrl* textLogger);

    wxTextCtrl* TextLog() const;
    void Clear();

protected:
    void DoLogTextAtLevel(wxLogLevel level, const wxString& msg);

private:
    wxTextCtrl* const _textLog;
    wxFFile _fileLog;
};

}
