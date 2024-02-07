#pragma once
#include <SDL.h>

namespace Code::ImGuiImPlot
{

class ImGuiView;

class OpenGLContext final
{
public:
    OpenGLContext(SDL_Window* sdlWindow);
    ~OpenGLContext();

private:
    void PrintOpenGLVersion();

private:
    SDL_GLContext glContext {nullptr};
};

} // ~namespace Code::ImGuiImPlot
