#include <glad/glad.h>

#include "scene.h"
#include "opengl_canvas.h"
#include "opengl_window.h"
// temp
#include "test_layer.h"

#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/dcclient.h>

#ifndef __WXMSW__
#include <EGL/egl.h>
#endif

namespace Kredo
{

class OpenGLLoadEvent : public wxEvent
{
public:
    OpenGLLoadEvent(wxEventType type, int windowID)
        : wxEvent(windowID, type)
    {}

    virtual wxEvent* Clone() const override
    {
        return new OpenGLLoadEvent(*this);
    }
};

wxDECLARE_EVENT(OPENGL_LOAD_EVENT, OpenGLLoadEvent);
wxDEFINE_EVENT(OPENGL_LOAD_EVENT, OpenGLLoadEvent);


OpenGLCanvas::OpenGLCanvas(const wxGLAttributes& canvasAttributes, OpenGLWindow* parent)
    : wxGLCanvas(parent, canvasAttributes, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
    , _context(nullptr)
    , _scene(nullptr)
    , _renderLoop(false)
    , _mouseCapture(0, 0)
{
    Bind(wxEVT_SIZE, &OpenGLCanvas::InitializeOpenGL, this);
    Bind(OPENGL_LOAD_EVENT, &OpenGLCanvas::OnOpenGLLoaded, this);

    InitializeContext();
}

OpenGLCanvas::~OpenGLCanvas()
{
    delete _scene;
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

void OpenGLCanvas::InitializeOpenGL(wxSizeEvent& event)
{
    WXUNUSED(event);

    wxLogDebug("OpenGLCanvas: OpenGL initialization...");
    SetCurrent(*_context);
    if (gladLoadGL() != 0)
    {
        wxLogInfo("OpenGLCanvas: OpenGL successfully initialized");
#ifndef __WXMSW__
        eglSwapInterval(GetEGLDisplay(), 1);
#endif
        OpenGLLoadEvent loadEvent(OPENGL_LOAD_EVENT, GetId());
        ProcessWindowEvent(loadEvent);
        InitializeScene();
    }
}

void OpenGLCanvas::InitializeScene()
{
    // TODO: load default or last used
    _scene = new Scene();

    // temp
    _scene->AddLayer(new TestLayer(_scene));

    wxLogInfo("OpenGLCanvas: scene successfully initialized");
}

void OpenGLCanvas::ActivateRenderLoop(bool on, const wxPoint& capturePosition)
{
    if (on && !_renderLoop)
    {
        _renderLoop = true;
        _mouseCapture = capturePosition;
        _scene->ProcessMouseMove(capturePosition, true);
        _renderTimer.Start(1000.0 / 60.0);
    }
    else if (!on && _renderLoop)
    {
        _renderLoop = false;
        WarpPointer(_mouseCapture.x, _mouseCapture.y);
        _scene->ClearKeyboard();
        _renderTimer.Stop();
    }
}

void OpenGLCanvas::Render(wxDC& dc)
{
    WXUNUSED(dc);

    _scene->ProcessKeyboard();
    _scene->Render();
    SwapBuffers();
}

wxPoint OpenGLCanvas::WarpPoint(const wxPoint& point)
{
    const auto size = GetSize() * GetContentScaleFactor();

    if (point.x <= 0)
        return wxPoint(size.GetWidth(), point.y);
    else if (point.x >= size.GetWidth())
        return wxPoint(0, point.y);
    else if (point.y <= 0)
        return wxPoint(point.x, size.GetHeight());
    else
        return wxPoint(point.x, 0);
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

    const auto size = event.GetSize() * GetContentScaleFactor();
    wxLogDebug("OpenGLCanvas: onSize [%dx%d]", size.GetWidth(), size.GetHeight());

    _scene->SetSize(size.x, size.y);
    Refresh();
}

void OpenGLCanvas::OnKeyDown(wxKeyEvent& event)
{
    if (_renderLoop)
        _scene->ProcessKeyPressed(event.GetKeyCode());
}

void OpenGLCanvas::OnKeyUp(wxKeyEvent& event)
{
    if (_renderLoop)
        _scene->ProcessKeyReleased(event.GetKeyCode());
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
        _scene->ProcessMouseMove(event.GetPosition());
}

void OpenGLCanvas::OnTimer(wxTimerEvent& event)
{
    WXUNUSED(event);
    if (_renderLoop)
        Refresh();
}

void OpenGLCanvas::OnWindowLeave(wxMouseEvent& event)
{
    if (_renderLoop)
    {
        const auto warped = WarpPoint(event.GetPosition());
        _scene->ProcessMouseMove(warped, true);
        WarpPointer(warped.x, warped.y);
    }
}

void OpenGLCanvas::OnOpenGLLoaded(OpenGLLoadEvent& event)
{
    WXUNUSED(event);

    Unbind(wxEVT_SIZE, &OpenGLCanvas::InitializeOpenGL, this);
    Bind(wxEVT_SIZE, &OpenGLCanvas::OnSize, this);

    Bind(wxEVT_PAINT, &OpenGLCanvas::OnPaint, this);
    Bind(wxEVT_KEY_DOWN, &OpenGLCanvas::OnKeyDown, this);
    Bind(wxEVT_KEY_UP, &OpenGLCanvas::OnKeyUp, this);
    Bind(wxEVT_RIGHT_DOWN, &OpenGLCanvas::OnMouseRightDown, this);
    Bind(wxEVT_MOTION, &OpenGLCanvas::OnMouseMove, this);
    Bind(wxEVT_LEAVE_WINDOW, &OpenGLCanvas::OnWindowLeave, this);

    _renderTimer.Bind(wxEVT_TIMER, &OpenGLCanvas::OnTimer, this);
}

}
