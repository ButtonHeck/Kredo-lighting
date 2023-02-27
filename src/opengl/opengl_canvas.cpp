#include "opengl_canvas.h"
#include "opengl_window.h"
#include "opengl_manager.h"

#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/dcclient.h>

#ifndef __WXMSW__
#include <EGL/egl.h>
#endif

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
    wxGLContextAttrs contextAttributes;
    contextAttributes.PlatformDefaults().CoreProfile().OGLVersion(4, 5).EndList();
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

#ifndef __WXMSW__
    eglSwapInterval(GetEGLDisplay(), 1);
#endif

    _manager.reset(new OpenGLManager());
    if (!_manager->IsInitialized())
    {
        wxMessageBox(_("Error: OpenGL manager failed to initialize"), _("OpenGL initialization error"), wxOK, this);
        return false;
    }

    wxLogInfo("OpenGLCanvas: OpenGL manager and functions successfully initialized");
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

void OpenGLCanvas::Render(wxDC& dc)
{
    WXUNUSED(dc)
    if (!_manager)
        return;

    _manager->ProcessEvents();
    _manager->Render();
    SwapBuffers();
}

void OpenGLCanvas::OnPaint(wxPaintEvent& event)
{
    WXUNUSED(event);
    wxPaintDC dc(this);

    Render(dc);
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
    wxLogDebug("OpenGLCanvas: onSize [%dx%d]", size.GetWidth(), size.GetHeight());

    _manager->SetSize(size.x, size.y);
    Refresh();
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
    SetCursor(wxCursor(_renderLoop ? wxCURSOR_BLANK : *wxSTANDARD_CURSOR));
}

void OpenGLCanvas::OnMouseMove(wxMouseEvent& event)
{
    event.Skip();

    if (_renderLoop)
        _manager->ProcessMouseMove();
}

void OpenGLCanvas::OnTimer(wxTimerEvent& event)
{
    WXUNUSED(event)
    if (_renderLoop)
        Refresh();
}

void OpenGLCanvas::OnWindowLeave(wxMouseEvent& event)
{
    if (_renderLoop)
    {
        const auto size = GetSize() * GetContentScaleFactor();

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
