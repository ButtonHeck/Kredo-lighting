#pragma once

namespace Kredo
{

class OpenGLManager
{
public:
    OpenGLManager();
    ~OpenGLManager();

    bool IsInitialized() const;

    void SetSize(int width, int height);
    void Render();

private:
    bool _initialized;
};

}
