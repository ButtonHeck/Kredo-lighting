#include "opengl/opengl_canvas.h"
#include "opengl/opengl_manager.h"
#include "opengl/opengl_window.h"

#include <wx/msgdlg.h>
#include <wx/dcclient.h>

namespace Kredo
{

OpenGLCanvas::OpenGLCanvas(const wxGLAttributes& canvasAttributes, OpenGLWindow* parent)
    : wxGLCanvas(parent, canvasAttributes, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
    , _context(nullptr)
    , _manager(nullptr)
{
    Bind(wxEVT_PAINT, &OpenGLCanvas::OnPaint, this);
    Bind(wxEVT_SIZE, &OpenGLCanvas::OnSize, this);

    InitializeContext();
}

void OpenGLCanvas::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    WXUNUSED(dc);
    WXUNUSED(event);

    if (!_manager)
        return;

    SetCurrent(*_context);
    _manager->Render();
    SwapBuffers();
}

void OpenGLCanvas::OnSize(wxSizeEvent& event)
{
    event.Skip();

    if (!IsShownOnScreen())
        return;

    if (!_manager)
    {
        if (!InitializeManager())
            return;
    }

    const auto size = event.GetSize() * GetContentScaleFactor();
    wxLogMessage("OpenGL canvas onSize [%dx%d]", size.GetWidth(), size.GetHeight());

    SetCurrent(*_context);
    _manager->SetSize(size.x, size.y);
    Refresh(false);
}

void OpenGLCanvas::InitializeContext()
{
    const auto openGLMajorVersion = 4;
    const auto openGLMinorVersion = 5;

    wxGLContextAttrs contextAttributes;
    contextAttributes.PlatformDefaults().CoreProfile().OGLVersion(openGLMajorVersion, openGLMinorVersion).EndList();
    _context.reset(new wxGLContext(this, nullptr, &contextAttributes));

    if (!_context->IsOK())
    {
        wxMessageBox("Kredo requires an OpenGL 4.5 capable driver!", "OpenGL canvas error", wxOK, this);
        _context.release();
    }
}

bool OpenGLCanvas::InitializeManager()
{
    if (!_context)
        return false;

    SetCurrent(*_context);

    _manager.reset(new OpenGLManager());
    if (!_manager->IsInitialized())
    {
        wxMessageBox("Error: OpenGL manager failed to initialize", "OpenGL initialization error", wxOK, this);
        return false;
    }

    // TODO: wxLogInfo
    wxLogDebug("OpenGL manager and functions successfully initialized");
    return true;
}

}
