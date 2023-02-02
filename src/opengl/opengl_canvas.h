#pragma once

#include <wx/glcanvas.h>
#include <memory>

namespace Kredo
{

class OpenGLWindow;
class OpenGLManager;

class OpenGLCanvas : public wxGLCanvas
{
public:
    OpenGLCanvas(const wxGLAttributes& canvasAttributes, OpenGLWindow* parent = nullptr);

    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);

private:
    void InitializeContext();
    bool InitializeManager();

private:
    std::unique_ptr<wxGLContext> _context;
    std::unique_ptr<OpenGLManager> _manager;
};

}
