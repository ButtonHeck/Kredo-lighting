#pragma once

#include <wx/bitmap.h>
#include <wx/icon.h>

namespace Kredo
{
namespace IconHelpers
{

wxBitmap LoadPngBitmap(const wxString& path, int width = -1, int height = -1);
wxIcon LoadPngIcon(const wxString& path);

}
}
