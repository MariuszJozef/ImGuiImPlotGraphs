#pragma once

namespace Code::ImGuiImPlot
{

class OpenGLContext;
class ImGuiFonts;
class ImPlotFrame;

class ImGuiView final
{
public:
    ImGuiView(ImGuiFonts& imGuiFonts);

    void DrawView1(ImPlotFrame* imPlotFrame);

private:
    void MaximiseView1();
    void SetInitialLayoutView1();
    void MenuBarForView1();
    void DrawSubView1ForPlot1(ImPlotFrame* imPlotFrame);
    void DrawSubView1ForPlot2(ImPlotFrame* imPlotFrame);
    void DrawSubView1ForPlot3(ImPlotFrame* imPlotFrame);
    void DrawSubView1ForPlot4(ImPlotFrame* imPlotFrame);

    void DrawSettingsView2();
    
    void HandleActionsOfView1();
    void HandleActionsOfSettingsView2();

private:
    bool isShowImGuiDemo {false};
    bool isShowImPlotDemo {false};
    bool isShowSettingsView {false};
    ImGuiFonts& imGuiFonts;
};

} // ~namespace Code::ImGuiImPlot
