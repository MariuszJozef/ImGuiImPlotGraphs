#include <stdexcept>
#include <glad/glad.h>
#include "SDLWindow.hpp"
#include "OpenGLContext.hpp"
#include "ImGuiFrame.hpp"

namespace Code::ImGuiImPlot
{
    
SDLWindow::SDLWindow(const std::string &windowTitle)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        throw std::runtime_error("Failure: SDL_Init");
    }

    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#ifdef OPENGL_DRAW_PRIMITIVES // Enables OpenGL draw primitives: glBegin(); ... glEnd();
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    
    sdlWindow = SDL_CreateWindow((windowTitle + windowSubtitle).c_str(), 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        windowWidth, windowHeight, 
        SDL_WINDOW_RESIZABLE | 
        SDL_WINDOW_MAXIMIZED | 
        SDL_WINDOW_ALLOW_HIGHDPI  | 
        SDL_WINDOW_OPENGL
    );

    if (!sdlWindow)
    {
        throw std::runtime_error("Failure: SDL_CreateWindow");
    }

    PrintSDLVersionInfo();

    openGLContext = std::make_unique<OpenGLContext>(sdlWindow);
    imGuiFrame = std::make_unique<ImGuiFrame>(sdlWindow, openGLContext.get());
}

SDLWindow::~SDLWindow()
{
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

void SDLWindow::RunFrameLoop()
{
    bool isQuit {false};

    while (!isQuit)
    {
        EnforceFrameRateLimit enforceFrameRateLimit {60u, 5u};
        RunEventsLoop(isQuit, enforceFrameRateLimit.SetViaRefIsGuiIdle());
        UpdateScene();
        DrawScene();
        PresentScene();
    }
}

void SDLWindow::RunEventsLoop(bool& isQuit, bool& isGuiIdle)
{
    static bool isMaximized {true}; // Starts off maximized due to flag SDL_WINDOW_MAXIMIZED
    static bool isFullScreen {false};

    SDL_Event event;
    
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        
        switch (event.type)
        {
        case SDL_QUIT:
            isQuit = true;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_CLOSE
                && event.window.windowID == SDL_GetWindowID(sdlWindow))
            {
                isQuit = true;
            }
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.mod == KMOD_RSHIFT && event.key.keysym.sym == SDLK_ESCAPE)
            {
                isQuit = true;
            }
            else if (event.key.keysym.sym == SDLK_F1)
            {
                if (isFullScreen)
                {
                    SDL_SetWindowFullscreen(sdlWindow, 0);
                    isFullScreen = false;
                }

                if (isMaximized)
                {
                    SDL_SetWindowSize(sdlWindow, windowWidth, windowHeight);
                    SDL_SetWindowPosition(sdlWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                    SDL_RestoreWindow(sdlWindow);
                }
                else
                {
                    SDL_MaximizeWindow(sdlWindow);
                }

                isMaximized = !isMaximized;
            }
            else if (event.key.keysym.sym == SDLK_F2)
            {
                if (isFullScreen)
                {
                    SDL_SetWindowFullscreen(sdlWindow, 0);
                    SDL_SetWindowSize(sdlWindow, windowWidth, windowHeight);
                    SDL_SetWindowPosition(sdlWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                    SDL_RestoreWindow(sdlWindow);
                }
                else
                {
                    SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
                }

                isMaximized = false;
                isFullScreen = !isFullScreen;
            }
            break;
        }
    }
}

void SDLWindow::UpdateScene()
{
    // glViewport(0, 0, windowWidth, windowHeight);
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
    glClearColor(1.0F, 1.0F, 1.0F, 1.0F);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    imGuiFrame->PreRender();
}

void SDLWindow::DrawScene()
{
    imGuiFrame->Render();
}

void SDLWindow::PresentScene()
{
    imGuiFrame->PostRender();
    SDL_GL_SwapWindow(sdlWindow);
}

void SDLWindow::PrintSDLVersionInfo()
{
    SDL_version version;
    SDL_VERSION(&version);
    SDL_Log("SDL version %u.%u.%u \n", version.major, version.minor, version.patch);
}

SDLWindow::EnforceFrameRateLimit::EnforceFrameRateLimit(unsigned int framesPerSecBusy, unsigned int framesPerSecIdle)
    : framesPerSecBusy(framesPerSecBusy)
    , framesPerSecIdle(framesPerSecIdle)
    , timeStartProcessFrameMs(SDL_GetTicks64())
{}

SDLWindow::EnforceFrameRateLimit::~EnforceFrameRateLimit()
{
    frameDurationMs = isGuiIdle ? 1000 / framesPerSecIdle : 1000 / framesPerSecBusy;

    unsigned int timeTakenToProcessFrameMs = SDL_GetTicks64() - timeStartProcessFrameMs;

    if (timeTakenToProcessFrameMs < frameDurationMs)
    {
        SDL_Delay(frameDurationMs - timeTakenToProcessFrameMs);
    }
}

} // ~namespace Code::ImGuiImPlot
