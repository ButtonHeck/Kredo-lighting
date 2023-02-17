#pragma once

#include <wx/glcanvas.h>
#include <wx/timer.h>
#include <memory>

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
    void Render();

    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnKeyUp(wxKeyEvent& event);
    void OnMouseRightDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnTimer(wxTimerEvent& event);

private:
    std::unique_ptr<wxGLContext> _context;
    std::unique_ptr<OpenGLManager> _manager;
    bool _renderLoop;
    wxPoint _mouseCapture;
    wxTimer _renderTimer;
};

}
