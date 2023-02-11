#pragma once

#include <wx/log.h>
#include <wx/ffile.h>
#include <unordered_map>

class wxTextCtrl;

namespace Kredo
{

class LogController : public wxLog
{
public:
    LogController(wxWindow* window);

    wxTextCtrl* TextLog() const;
    void Clear();
    void ChangeFontSize(bool increase);
    void SetLogLevelActive(wxLogLevel level, bool active);

protected:
    void DoLogTextAtLevel(wxLogLevel level, const wxString& msg);

private:
    wxTextCtrl* const _textLog;
    wxFFile _fileLog;
    std::unordered_map<wxLogLevel, bool> _enabledLevels;
};

}
