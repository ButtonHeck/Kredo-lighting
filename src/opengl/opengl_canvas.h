#pragma once

#include "pointers.h"

#include <wx/timer.h>
#include <wx/glcanvas.h>

namespace Kredo
{

class Scene;
class OpenGLWindow;
class OpenGLLoadEvent;

class OpenGLCanvas : public wxGLCanvas
{
public:
    explicit OpenGLCanvas(const wxGLAttributes& canvasAttributes, OpenGLWindow* parent = nullptr);
    ~OpenGLCanvas();

private:
    void InitializeContext();
    void InitializeOpenGL(wxSizeEvent& event);
    void InitializeScene();

    void ActivateRenderLoop(bool on, const wxPoint& capturePosition);
    void Render(wxDC& dc);
    wxPoint WarpPoint(const wxPoint& point);

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
    Scene* _scene;
    bool _renderLoop;
    wxPoint _mouseCapture;
    wxTimer _renderTimer;
};

}
