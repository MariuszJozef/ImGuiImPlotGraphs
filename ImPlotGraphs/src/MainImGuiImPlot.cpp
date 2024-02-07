#define SDL_MAIN_HANDLED

#include <iostream>
#include "SDLWindow.hpp"

int main()
{
    try {
        Code::ImGuiImPlot::SDLWindow sdlWindow("ImGui within SDL Window");
        sdlWindow.RunFrameLoop();
    }
    catch (const std::runtime_error& e)
    {
        std::cout << e.what() << "\n";
    }
}
