#pragma once

#include <wx/window.h>

namespace Kredo
{

class OpenGLCanvas;

class OpenGLWindow : public wxWindow
{
public:
    explicit OpenGLWindow(wxWindow* parent = nullptr);

private:
    void OnSize(wxSizeEvent& event);
    void InitializeCanvas();

private:
    OpenGLCanvas* _openglCanvas;
};

}
