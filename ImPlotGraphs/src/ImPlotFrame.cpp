#include <iostream>
#include "implot.h"
#include "ImPlotFrame.hpp"

namespace Code::ImGuiImPlot
{

ImPlotFrame::ImPlotFrame()
{
    ImPlot::CreateContext();
    LogImPlotVersion();
}

ImPlotFrame::~ImPlotFrame()
{
    ImPlot::DestroyContext();
}

void ImPlotFrame::LogImPlotVersion()
{
    std::cout << "ImPlot version: " << IMPLOT_VERSION << "\n";
}
        
} // ~namespace Code::ImGuiImPlot
