#include "icon_helpers.h"

namespace Kredo
{
namespace IconHelpers
{

wxBitmap LoadPngBitmap(const wxString& path, int width, int height)
{
    wxBitmap bitmap(wxString::Format("%s/%s", KREDO_RESOURCES_DIR, path), wxBITMAP_TYPE_PNG);
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
    return wxIcon(wxString::Format("%s/%s", KREDO_RESOURCES_DIR, path), wxBITMAP_TYPE_PNG);
}

}
}
