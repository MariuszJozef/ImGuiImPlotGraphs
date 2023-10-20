#include <cmath>
#include <algorithm>
#include "implot.h"
#include "ImPlotGraph5.hpp"

namespace Code::ImGuiImPlot
{

Plot5::Plot5()
{
    plotPoints = std::round(plotPointsPerUnitLength * (xMax - xMin));
    x.reserve(plotPoints);
    y.reserve(plotPoints);
}

void Plot5::CalculatePlotCoordinates()
{
    x.clear();
    y.clear();

    plotPoints = std::round(plotPointsPerUnitLength * (xMax - xMin));
    xIncrement = (xMax - xMin) / (plotPoints - 1);

    for (int i = 0; i < plotPoints; ++i) 
    {
        x.push_back(xMin + i * xIncrement);
        y.push_back(std::tanh(x.at(i)));
    }

    const auto [min, max] = std::minmax_element(begin(y), end(y));
    yMin = std::floor(*min);
    yMax = std::ceil(*max);
}

bool Plot5::CheckIsNeedReplot()
{
    static bool isNeedReplot {true};
    static int plotPointsPerUnitLengthOld {plotPointsPerUnitLength};

    if (plotPointsPerUnitLengthOld != plotPointsPerUnitLength)
    {
        plotPointsPerUnitLengthOld = plotPointsPerUnitLength;
        isNeedReplot = true;
    }

    if (isNeedReplot)
    {
        isNeedReplot = false; // So that next time we don't replot unless parameter(s) change
        return true;
    }

    return isNeedReplot;
}

void Plot5::Graph()
{
    if (CheckIsNeedReplot())
    {
        CalculatePlotCoordinates();
    }

    if (ImPlot::BeginPlot("Sinusoid", ImVec2(-1, -1), ImPlotFlags_Crosshairs))
    {
        ImPlot::SetupAxis(ImAxis_X1, "x axis");
        ImPlot::SetupAxis(ImAxis_Y1, "y axis");

        if (isZoom)
        {
            ImPlot::SetupAxesLimits(xMin - gap, xMax + gap, yMin - gap, yMax + gap, ImPlotCond_None);
        }
        else
        {
            ImPlot::SetupAxesLimits(xMin - gap, xMax + gap, yMin - gap, yMax + gap, ImPlotCond_Always);
        }

        // ImPlot::SetupAxisTicks(ImAxis_X1, xTicks.data(), xTicks.size(), xLabels.data(), false);
        // ImPlot::SetupAxisTicks(ImAxis_Y1, yTicks.data(), yTicks.size(), yLabels.data(), false);

        ImPlot::SetNextLineStyle(ImVec4(1, 1, 0, 1), 3.0F);

        ImPlot::PlotLine("y = std::tanh(x)", x.data(), y.data(), plotPoints);
        ImPlot::EndPlot();
    }
}

} // ~namespace Code::ImGuiImPlot
