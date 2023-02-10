#include "log_formatter.h"

#include <wx/datetime.h>

namespace Kredo
{

wxString LogFormatter::Format(wxLogLevel level, const wxString& msg, const wxLogRecordInfo& info) const
{
    auto output = wxDateTime(wxLongLong(info.timestampMS)).Format("[%H:%M:%S.%l] ");

    switch (level)
    {
    case wxLOG_FatalError:
        output += "F: ";
        break;

    case wxLOG_Error:
        output += "E: ";
        break;

    case wxLOG_Warning:
        output += "W: ";
        break;

    case wxLOG_Message:
        output += "-: ";
        break;

    case wxLOG_Info:
        output += "I: ";
        break;

    case wxLOG_Debug:
        output += "D: ";
        break;

    case wxLOG_Trace:
        output += "T: ";
        break;

    default:
        output += "*: ";
        break;
    }

    return output + msg;
}

}
