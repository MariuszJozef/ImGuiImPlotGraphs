#pragma once
#include "ImPlotGraph1.hpp"
#include "ImPlotGraph2.hpp"
#include "ImPlotGraph3.hpp"
#include "ImPlotGraph4.hpp"

namespace Code::ImGuiImPlot
{

class ImPlotFrame
{
public:
    ImPlotFrame();
    ~ImPlotFrame();

private:
    void LogImPlotVersion();

public:

    Plot1 plot1;
    Plot2 plot2;
    Plot3 plot3;
    Plot4 plot4;
};

} // ~namespace Code::ImGuiImPlot
