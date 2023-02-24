#include "filesystem.h"

namespace Kredo
{
namespace Filesystem
{

wxString Path(const wxString& path)
{
#ifdef __WXMSW__
    auto msPath = path;
    std::replace(msPath.begin(), msPath.end(), '/', '\\');
    return msPath;
#endif
    return path;
}

}
}
