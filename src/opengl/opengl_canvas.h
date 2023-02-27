#pragma once

#include "pointers.h"

#include <wx/glcanvas.h>
#include <wx/timer.h>

namespace Kredo
{

class OpenGLWindow;
class OpenGLManager;

class OpenGLCanvas : public wxGLCanvas
{
public:
    OpenGLCanvas(const wxGLAttributes& canvasAttributes, OpenGLWindow* parent = nullptr);

private:
    void InitializeContext();
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

private:
    Unique<wxGLContext> _context;
    Unique<OpenGLManager> _manager;
    bool _renderLoop;
    wxPoint _mouseCapture;
    wxTimer _renderTimer;
};

}
