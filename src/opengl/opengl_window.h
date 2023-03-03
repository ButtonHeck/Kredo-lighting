#pragma once

#include "pointers.h"

#include <wx/window.h>

namespace Kredo
{

class OpenGLCanvas;

class OpenGLWindow : public wxWindow
{
public:
    OpenGLWindow(wxWindow* parent, int id);

private:
    void OnSize(wxSizeEvent& event);

private:
    Unique<OpenGLCanvas> _openglCanvas;
};

}
