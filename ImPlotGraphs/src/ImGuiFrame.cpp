#include <iostream>
#include "ImGuiFrame.hpp"
#include "OpenGLContext.hpp"
#include "ImPlotFrame.hpp"

namespace Code::ImGuiImPlot
{

ImGuiFrame::ImGuiFrame(SDL_Window* sdlWindow, OpenGLContext* openGLContext)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(sdlWindow, openGLContext);
    ImGui_ImplOpenGL3_Init("#version 410");

    ApplyGenericSettings();
    PrintImGuiVersion();

    imPlotFrame = std::make_unique<ImPlotFrame>();
}

ImGuiFrame::~ImGuiFrame()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiFrame::PreRender()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void ImGuiFrame::Render()
{
    imguiView.DrawView1(imPlotFrame.get());
}

void ImGuiFrame::PostRender()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 
}

void ImGuiFrame::ApplyGenericSettings()
{
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle& style{ ImGui::GetStyle() };
    style.WindowRounding = 5.3F;
    style.GrabRounding = style.FrameRounding = 2.3F;
    style.ScrollbarRounding = 5.0F;
    style.FrameBorderSize = 1.0F;
    style.ItemSpacing.y = 6.5F;

    imGuiFonts.RetrieveAndStoreFontFiles();
    imGuiFonts.StoreUsableFontsIntoTwoIndexMap();
    imGuiFonts.FillFontWidgetInfo();
}

void ImGuiFrame::PrintImGuiVersion()
{
    std::cout << "ImGui version: " << ImGui::GetVersion() << "\n";
}

} // ~namespace Code::ImGuiImPlot
