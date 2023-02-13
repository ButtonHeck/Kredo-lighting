#pragma once

#include <wx/bitmap.h>
#include <wx/icon.h>

namespace Kredo
{
namespace Icons
{

wxBitmap LoadPngBitmap(const wxString& path, int width = -1, int height = -1);
wxBitmap LoadPngBitmap16(const wxString& path);

wxIcon LoadPngIcon(const wxString& path);

}
}
