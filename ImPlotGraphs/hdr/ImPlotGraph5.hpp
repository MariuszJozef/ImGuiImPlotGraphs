#pragma once
#include <vector>
#include "ImPlotGeneric.hpp"

namespace Code::ImGuiImPlot
{
    
class Plot5 : public PlotGeneric
{
public:
    Plot5();
    void Graph() override;

private:
    void CalculatePlotCoordinates() override;
    bool CheckIsNeedReplot() override;

private:
    std::vector<float> x;
    std::vector<float> y;
    std::vector<double> xTicks;
    std::vector<double> yTicks;
    std::vector<const char*> xLabels;
    std::vector<const char*> yLabels;

    float xMin {-3.0F};
    float xMax {5.0F};
    float yMin {0.0F};
    float yMax {0.0F};
};

} // ~namespace Code::ImGuiImPlot
