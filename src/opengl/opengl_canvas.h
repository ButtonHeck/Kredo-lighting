#pragma once

#include "pointers.h"
#include "opengl_manager.h"

#include <wx/timer.h>
#include <wx/glcanvas.h>

namespace Kredo
{

class OpenGLWindow;
class OpenGLLoadEvent;

class OpenGLCanvas : public wxGLCanvas
{
public:
    explicit OpenGLCanvas(const wxGLAttributes& canvasAttributes, OpenGLWindow* parent = nullptr);

private:
    void InitializeContext();
    void InitializeOpenGL(wxSizeEvent& event);
    bool InitializeManager();

    void ActivateRenderLoop(bool on, const wxPoint& capturePosition);
    void Render(wxDC& dc);

    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnKeyUp(wxKeyEvent& event);
    void OnMouseRightDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnWindowLeave(wxMouseEvent& event);
    void OnOpenGLLoaded(OpenGLLoadEvent& event);

private:
    Unique<wxGLContext> _context;
    Unique<OpenGLManager> _manager;
    bool _renderLoop;
    wxPoint _mouseCapture;
    wxTimer _renderTimer;
};

}
