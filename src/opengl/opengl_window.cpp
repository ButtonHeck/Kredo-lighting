#include "opengl_window.h"
#include "opengl_canvas.h"

#include <wx/glcanvas.h>

namespace Kredo
{

OpenGLWindow::OpenGLWindow(wxWindow* parent, int id)
    : wxWindow(parent, id, wxDefaultPosition, wxDefaultSize)
    , _openglCanvas(nullptr)
{
    Bind(wxEVT_SIZE, &OpenGLWindow::OnSize, this);

    InitializeCanvas();
}

void OpenGLWindow::OnSize(wxSizeEvent& event)
{
    const auto size = event.GetSize() * GetContentScaleFactor();
    wxLogDebug("OpenGL window onSize [%dx%d]", size.GetWidth(), size.GetHeight());

    _openglCanvas->SetSize(size);
}

void OpenGLWindow::InitializeCanvas()
{
    wxGLAttributes openglAttributes;
    openglAttributes.PlatformDefaults().RGBA().DoubleBuffer().Depth(16).EndList();
    bool accepted = wxGLCanvas::IsDisplaySupported(openglAttributes);

    if (accepted)
        _openglCanvas = new OpenGLCanvas(openglAttributes, this);
}

}

