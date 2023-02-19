#include "opengl_canvas.h"
#include "opengl_manager.h"
#include "opengl_window.h"

#include <wx/log.h>
#include <wx/msgdlg.h>

#include <EGL/egl.h>

namespace Kredo
{

OpenGLCanvas::OpenGLCanvas(const wxGLAttributes& canvasAttributes, OpenGLWindow* parent)
    : wxGLCanvas(parent, canvasAttributes, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
    , _context(nullptr)
    , _manager(nullptr)
    , _renderLoop(false)
    , _mouseCapture(0, 0)
{
    Bind(wxEVT_PAINT, &OpenGLCanvas::OnPaint, this);
    Bind(wxEVT_SIZE, &OpenGLCanvas::OnSize, this);
    Bind(wxEVT_KEY_DOWN, &OpenGLCanvas::OnKeyDown, this);
    Bind(wxEVT_KEY_UP, &OpenGLCanvas::OnKeyUp, this);
    Bind(wxEVT_RIGHT_DOWN, &OpenGLCanvas::OnMouseRightDown, this);
    Bind(wxEVT_MOTION, &OpenGLCanvas::OnMouseMove, this);
    Bind(wxEVT_LEAVE_WINDOW, &OpenGLCanvas::OnWindowLeave, this);

    _renderTimer.Bind(wxEVT_TIMER, &OpenGLCanvas::OnTimer, this);

    InitializeContext();
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
        wxMessageBox(_("Kredo requires an OpenGL 4.5 capable driver!"), _("OpenGL canvas error"), wxOK, this);
        _context.release();
    }
}

bool OpenGLCanvas::InitializeManager()
{
    if (!_context)
        return false;

    SetCurrent(*_context);
    eglSwapInterval(GetEGLDisplay(), 1);

    _manager.reset(new OpenGLManager());
    if (!_manager->IsInitialized())
    {
        wxMessageBox(_("Error: OpenGL manager failed to initialize"), _("OpenGL initialization error"), wxOK, this);
        return false;
    }

    wxLogInfo("OpenGL manager and functions successfully initialized");
    return true;
}

void OpenGLCanvas::ActivateRenderLoop(bool on, const wxPoint& capturePosition)
{
    if (on && !_renderLoop)
    {
        _renderLoop = true;
        _mouseCapture = capturePosition;
        _manager->UpdateOrigin();
        _renderTimer.Start(1000.0 / 60.0);
    }
    else if (!on && _renderLoop)
    {
        _renderLoop = false;
        WarpPointer(_mouseCapture.x, _mouseCapture.y);
        _manager->ClearEvents();
        _renderTimer.Stop();
    }
}

void OpenGLCanvas::Render()
{
    SetCurrent(*_context);
    _manager->ProcessEvents();
    _manager->Render();
    SwapBuffers();
}

void OpenGLCanvas::OnPaint(wxPaintEvent& event)
{
    WXUNUSED(event);

    Render();
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
    wxLogDebug("OpenGL canvas onSize [%dx%d]", size.GetWidth(), size.GetHeight());

    SetCurrent(*_context);
    _manager->SetSize(size.x, size.y);
    Refresh(false);
}

void OpenGLCanvas::OnKeyDown(wxKeyEvent& event)
{
    if (_renderLoop)
        _manager->ProcessKeyPressed(event.GetKeyCode());
}

void OpenGLCanvas::OnKeyUp(wxKeyEvent& event)
{
    if (_renderLoop)
        _manager->ProcessKeyReleased(event.GetKeyCode());
}

void OpenGLCanvas::OnMouseRightDown(wxMouseEvent& event)
{
    SetFocus();
    ActivateRenderLoop(!_renderLoop, event.GetPosition());
    SetCursor(_renderLoop ? wxCursor(wxCURSOR_BLANK) : wxCursor(*wxSTANDARD_CURSOR));
}

void OpenGLCanvas::OnMouseMove(wxMouseEvent& event)
{
    WXUNUSED(event)
    if (_renderLoop)
        _manager->ProcessMouseMove();
}

void OpenGLCanvas::OnTimer(wxTimerEvent& event)
{
    WXUNUSED(event)
    if (_renderLoop)
        Refresh(true);
}

void OpenGLCanvas::OnWindowLeave(wxMouseEvent& event)
{
    if (_renderLoop)
    {
        const auto size = GetSize();

        if (event.GetX() <= 0)
        {
            WarpPointer(size.GetWidth(), event.GetY());
            _manager->UpdateOrigin();
        }
        else if (event.GetX() >= size.GetWidth())
        {
            WarpPointer(0, event.GetY());
            _manager->UpdateOrigin();
        }
        else if (event.GetY() <= 0)
        {
            WarpPointer(event.GetX(), size.GetHeight());
            _manager->UpdateOrigin();
        }
        else if (event.GetY() >= size.GetHeight())
        {
            WarpPointer(event.GetX(), 0);
            _manager->UpdateOrigin();
        }
    }
}

}
