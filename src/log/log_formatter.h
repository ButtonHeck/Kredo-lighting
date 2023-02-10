#pragma once

#include <wx/log.h>

namespace Kredo
{

class LogFormatter : public wxLogFormatter
{
public:
    wxString Format(wxLogLevel level, const wxString& msg, const wxLogRecordInfo& info) const;
};

}
