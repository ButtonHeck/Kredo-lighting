#include "icons.h"
#include "filesystem.h"

namespace Kredo
{
namespace Icons
{

wxBitmap LoadPngBitmap(const wxString& path, int width, int height)
{
    const auto fullpath = Filesystem::Path(wxString::Format("%s%s", KREDO_RESOURCES_DIR, path));
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
    const auto fullpath = Filesystem::Path(wxString::Format("%s%s", KREDO_RESOURCES_DIR, path));
    return wxIcon(fullpath, wxBITMAP_TYPE_PNG);
}

}
}
