#pragma once
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "SDL.h"
#include "ImGuiView.hpp"
#include "ImGuiFonts.hpp"

#include <memory>

namespace Code::ImGuiImPlot
{
   
class OpenGLContext;
class ImPlotFrame;

class ImGuiFrame final
{
public:
    ImGuiFrame(SDL_Window* sdlWindow, OpenGLContext* openGLContext);
    ~ImGuiFrame();

    void PreRender();
    void Render();
    void PostRender();

private:
    void ApplyGenericSettings();
    void PrintImGuiVersion();

private:
    ImGuiFonts imGuiFonts;
    ImGuiView imguiView {imGuiFonts};
    std::unique_ptr<ImPlotFrame> imPlotFrame {nullptr};
};

} // ~namespace Code::ImGuiImPlot
