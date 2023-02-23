#include "icons.h"

namespace Kredo
{
namespace Icons
{

wxBitmap LoadPngBitmap(const wxString& path, int width, int height)
{
    auto fullpath = wxString::Format("%s%s", KREDO_RESOURCES_DIR, path);
#ifdef __WXMSW__
    std::replace(p.begin(), p.end(), '/', '\\');
#endif

    wxBitmap bitmap(fullpath, wxBITMAP_TYPE_PNG);
    if (width > 0 && height > 0)
    {
        wxImage image = bitmap.ConvertToImage().Rescale(width, height);
        return wxBitmap(image);
    }

    return bitmap;
    
}

wxBitmap LoadPngBitmap16(const wxString& path)
{
    return LoadPngBitmap(path, 16, 16);
}

wxIcon LoadPngIcon(const wxString& path)
{
    auto fullpath = wxString::Format("%s%s", KREDO_RESOURCES_DIR, path);
#ifdef __WXMSW__
    std::replace(p.begin(), p.end(), '/', '\\');
#endif

    return wxIcon(fullpath, wxBITMAP_TYPE_PNG);
}

}
}
