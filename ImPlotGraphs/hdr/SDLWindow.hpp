#pragma once
#include <SDL.h>
#include <string>
#include <memory>

namespace Code::ImGuiImPlot
{
    
class OpenGLContext;
class ImGuiFrame;

class SDLWindow final
{
public:
    SDLWindow(const std::string &windowTitle);
    ~SDLWindow();

    void RunFrameLoop();

private:
    void RunEventsLoop(bool& isQuit, bool& isGuiIdle);
    void UpdateScene();
    void DrawScene();
    void PresentScene();
    void PrintSDLVersionInfo();

private:
    const std::string windowSubtitle {" ~~~ F1: toggle max screen ~~~ F2: toggle full screen ~~~ RIGHT-SHIFT+ESC: quit ~~~"};
    const int windowWidth {800};
    const int windowHeight {600};
    SDL_Window *sdlWindow {nullptr};

    std::unique_ptr<OpenGLContext> openGLContext {nullptr};
    std::unique_ptr<ImGuiFrame> imGuiFrame {nullptr};

    class EnforceFrameRateLimit final
    {
    public:
        EnforceFrameRateLimit(unsigned int framesPerSecBusy = 60, unsigned int framesPerSecIdle = 5);
        ~EnforceFrameRateLimit();

        bool& SetViaRefIsGuiIdle() { return isGuiIdle; }

    private:
        bool isGuiIdle {true};
        const unsigned int framesPerSecBusy {60};
        const unsigned int framesPerSecIdle {5};
        unsigned int frameDurationMs {1000 / framesPerSecIdle};
        unsigned int timeStartProcessFrameMs {0};
    };
};

} // ~namespace Code::ImGuiImPlot
